#include "autopcoffdata.h"

#include <model/configdata/configdata.h>

#include <common/tool/utils.h>

#include "../../ui.h"

AutoPcOffData::AutoPcOffData(QObject *parent)
	: QObject(parent), _jspnpath(""), _checkTimer(Q_NULLPTR), _alertflag(false), _pcoffflag(false)
{
    _checkTimer = std::make_shared<QTimer>();
    connect(_checkTimer.get(), &QTimer::timeout, this, &AutoPcOffData::timerCallbackFunc);

	QString filepath = QString("%1/cfg/").arg(qApp->applicationDirPath()); //test.json
	if (!QDir().exists(filepath)) {
		QDir().mkdir(filepath);
	}

	_jspnpath = QString("%1pcoff.json").arg(filepath);
	if (!updateData()) { //항목 없으면 json 초기화
        setJsonData();
	}

    if (_autoPcffInfo.use) {//예약 항목 있으면 시작프로그램 등록 확인
        timerStart();
        //ConfigData::inst()->setStartupOptOnly(true);
    }
        
}

AutoPcOffData::~AutoPcOffData()
{
    if (_checkTimer) {
        if (_checkTimer->isActive()) {
            _checkTimer->stop();
        }

        _checkTimer->deleteLater();
    }


        
}

void AutoPcOffData::timerCallbackFunc()
{
    qDebug() << "timerCallbackFunc";

    _mutex.lock();

    AutoPCOffStatus type = autoPcoffCheck();
    _mutex.unlock();

    if (type == AutoPCOffStatus::NOTUSE)
        timerStop();
    else if (type == AutoPCOffStatus::ALERT) {
        qDebug() << "AutoPcff Alert";
        if (!_alertflag) {
            _alertflag = true;
            emit callPcOffAlert();
        }

        
    }
    else if (type == AutoPCOffStatus::PCOFF) {
        qDebug() << "AutoPcff ShutDown";
        if (!_pcoffflag) {
            _pcoffflag = true;
            emit callPCOff();
        }
        
        //utils::pcShutDown();
    }

}

void AutoPcOffData::timerStart()
{
    if (_checkTimer) {
        if(!_checkTimer->isActive())
            _checkTimer->start(10000);
    }
        
}

void AutoPcOffData::timerStop()
{
    if (_checkTimer) {
        if (_checkTimer->isActive())
            _checkTimer->stop();
    }
}

AutoPCOffStatus AutoPcOffData::autoPcoffCheck()
{
    if (!_autoPcffInfo.use)
        return AutoPCOffStatus::NOTUSE;

    QTime currentTime = QTime::currentTime();
    QString now = currentTime.toString("hh:mm");
    //QString reservetime = QString("%1:%2").arg(_autoPcffInfo.hour).arg(_autoPcffInfo.min);
    QString reservetime = QString("%1:%2").arg(QStringLiteral("%1").arg(_autoPcffInfo.hour, 2, 10, QLatin1Char('0'))).
        arg(QStringLiteral("%1").arg(_autoPcffInfo.min, 2, 10, QLatin1Char('0')));

    int alertHour = 0;
    int alertMin = 0;
    if (_autoPcffInfo.min < 5) {
        alertMin = _autoPcffInfo.min + 60 - 5;
        if (_autoPcffInfo.hour == 0) { //24
            alertHour = 23;
        }
        else {
            alertHour = _autoPcffInfo.hour - 1;
        }
    }
    else {
        alertHour = _autoPcffInfo.hour;
        alertMin = _autoPcffInfo.min - 5;
    }
    QString hourtext = QStringLiteral("%1").arg(alertHour, 2, 10, QLatin1Char('0'));
    QString mintext = QStringLiteral("%1").arg(alertMin, 2, 10, QLatin1Char('0'));

    QString alerttime = QString("%1:%2").arg(hourtext).arg(mintext);

    if (_autoPcffInfo.type == ReservType::DAY) {
        if(alerttime == now)
            return AutoPCOffStatus::ALERT;
        else if (reservetime == now) 
            return AutoPCOffStatus::PCOFF;
    }
    else if (_autoPcffInfo.type == ReservType::WEEK) {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        int currentDayNumber = currentDateTime.date().dayOfWeek();
        if ((int)_autoPcffInfo.weektype == currentDayNumber) {
            if (alerttime == now)
                return AutoPCOffStatus::ALERT;
            else if (reservetime == now)
                return AutoPCOffStatus::PCOFF;
        }
    }
    else if (_autoPcffInfo.type == ReservType::MONTH) {
        //month 는 마지막 달 계산 하자
        int checkday = 0;
        if (_autoPcffInfo.monthtype == ReservMonthType::FIRST)
            checkday = 1;
        else if (_autoPcffInfo.monthtype == ReservMonthType::FIFTEEN)
            checkday = 15;
        else if (_autoPcffInfo.monthtype == ReservMonthType::THIRTY) {
            QDate currentDate = QDate::currentDate();
            QDate lastDayOfMonth = currentDate.addMonths(1).addDays(-currentDate.day());
            checkday = lastDayOfMonth.day();
        }
        else
            return AutoPCOffStatus::NOTUSE;

        QDateTime currentDateTime = QDateTime::currentDateTime();
        if (checkday == currentDateTime.date().month()) {
            if (alerttime == now)
                return AutoPCOffStatus::ALERT;
            else if (reservetime == now)
                return AutoPCOffStatus::PCOFF;
        }
    }
    return AutoPCOffStatus::NONE;
}


bool AutoPcOffData::updateData()
{
    QFile loadFile(_jspnpath);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open  file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    if (saveData.isEmpty()) {
        loadFile.close();
        return false;
    }

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    parseJsonData(loadDoc.object());
    loadFile.close();
    return true;

}


void AutoPcOffData::setJsonData()
{
    QJsonArray jsonArray;
    QJsonObject savedata;
    
    QJsonObject jsonObject;
    jsonObject.insert("type", (int)_autoPcffInfo.type);
    jsonObject.insert("weektype", (int)_autoPcffInfo.weektype);
    jsonObject.insert("monthtype", (int)_autoPcffInfo.monthtype);
    jsonObject.insert("hour", (int)_autoPcffInfo.hour);
    jsonObject.insert("min", (int)_autoPcffInfo.min);
    jsonObject.insert("use", (bool)_autoPcffInfo.use);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);

    QFile file(_jspnpath);
    if (!file.open(QIODevice::WriteOnly))
        return;

    file.write(jsonDoc.toJson());
    file.close();
}


void AutoPcOffData::parseJsonData(const QJsonObject& json)
{
    if (json["use"].isBool())
        _autoPcffInfo.use = json["use"].toBool();

    if (json["type"].isDouble())
        _autoPcffInfo.type = (ReservType)json["type"].toInt();

    if (_autoPcffInfo.type == ReservType::WEEK) {
        if (json["weektype"].isDouble())
            _autoPcffInfo.weektype = (ReservWeekType)json["weektype"].toInt();
    }
    else if (_autoPcffInfo.type == ReservType::MONTH) {
        if (json["monthtype"].isDouble())
            _autoPcffInfo.monthtype = (ReservMonthType)json["monthtype"].toInt();
    }

    if (json["hour"].isDouble())
        _autoPcffInfo.hour = json["hour"].toInt();

    if (json["min"].isDouble())
        _autoPcffInfo.min = json["min"].toInt();

}

bool AutoPcOffData::isAutoPcOffUse() const
{
    return _autoPcffInfo.use;
}

ReserveData AutoPcOffData::getPcoffData() const
{
    return _autoPcffInfo;
}

void AutoPcOffData::setPcOffData(ReserveData data)
{
    _alertflag = false; _pcoffflag = false;
    _autoPcffInfo.use = data.use;
    _autoPcffInfo.hour = data.hour;
    _autoPcffInfo.min = data.min;
    _autoPcffInfo.type = data.type;
    _autoPcffInfo.weektype = data.weektype;
    _autoPcffInfo.monthtype = data.monthtype;

    setJsonData();

    if (_autoPcffInfo.use) {
        timerStart();
        //시작 프로그램 등록 (우선 제거)
        //ConfigData::inst()->setStartupOptOnly(true);
    }
    else
        timerStop();

}

//사실상 off만 온다 -> ㄴㄴ on off 다옴
void AutoPcOffData::setPcOffOnOff(bool off)
{
    _alertflag = false; _pcoffflag = false;

    _autoPcffInfo.use = off;
    setJsonData();

    if (_autoPcffInfo.use)
        timerStart();
    else
        timerStop();
}