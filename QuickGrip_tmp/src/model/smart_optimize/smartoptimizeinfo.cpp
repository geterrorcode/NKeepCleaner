#include "smartoptimizeinfo.h"

#include <QFile>

#include <model/configdata/configdata.h>

#include "../../ui.h"

SmartOptimizeInfo::SmartOptimizeInfo(QObject *parent)
	: QObject(parent), _cfgpath("")
{
	_smartinfo.clear();
    
    //init
    for (int i = 1; i < 5; i++) {
        ReserveData data;
        _smartinfo.insert(i, data);
    }

    QString filepath = QString("%1/cfg/").arg(qApp->applicationDirPath()); //test.json
    if (!QDir().exists(filepath)) {
        QDir().mkdir(filepath);
    }
    _cfgpath = QString("%1optireserve.json").arg(filepath);

    if (!updateData()) { //항목 없으면 json 초기화
        initJsonData();
    }

    //if (getUseData() == true) { //예약 항목 있으면 시작프로그램 등록 확인 -> ConfigData 자체적으로 체크
    //    ConfigData::inst()->setStartupOptOnly(true);
    //}
}

SmartOptimizeInfo::~SmartOptimizeInfo()
{}

bool SmartOptimizeInfo::updateData() 
{
    QFile loadFile(_cfgpath);
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

void SmartOptimizeInfo::initJsonData()
{
    QJsonArray jsonArray;
    QJsonObject savedata;
    QMapIterator<int, ReserveData> iter(_smartinfo);
    while (iter.hasNext()) {
        iter.next();

        QJsonObject jsonObject;
        jsonObject.insert("type", (int)iter.value().type);
        jsonObject.insert("weektype", (int)iter.value().weektype);
        jsonObject.insert("monthtype", (int)iter.value().monthtype);
        jsonObject.insert("hour", (int)iter.value().hour);
        jsonObject.insert("min", (int)iter.value().min);
        jsonObject.insert("use", (bool)iter.value().use);

        savedata.insert(QString("%1").arg(iter.key()), jsonObject);
    }

    QJsonDocument jsonDoc;
    jsonDoc.setObject(savedata);

    QFile file(_cfgpath);
    if (!file.open(QIODevice::WriteOnly))
        return;
    file.write(jsonDoc.toJson());
    file.close();
}

void SmartOptimizeInfo::parseJsonData(const QJsonObject& json)
{
    for (size_t i = 1; i < 5; i++)
    {
        QString key = QString("%1").arg(i);
        if (json.contains(key) && json[key].isObject()) {

            auto data = _smartinfo[i];

            QJsonObject item = json[key].toObject();
            if (!item["type"].isDouble())
                continue;
            
            data.type = (ReservType)item["type"].toInt();
            if (data.type == ReservType::WEEK) {
                if (item["weektype"].isDouble()) 
                    data.weektype = (ReservWeekType)item["weektype"].toInt();
            }
            else if (data.type == ReservType::MONTH) {
                if (item["monthtype"].isDouble()) 
                    data.monthtype = (ReservMonthType)item["monthtype"].toInt();
            }

            if (item["hour"].isDouble()) 
                data.hour= item["hour"].toInt();

            if (item["min"].isDouble()) 
                data.min = item["min"].toInt();

            if (item["use"].isBool()) 
                data.use = item["use"].toBool();

            _smartinfo[i] = data;
        }
    }
}

bool SmartOptimizeInfo::getUseData()
{
    bool use = false;
    QMapIterator<int, ReserveData> iter(_smartinfo);
    while (iter.hasNext()) {
        iter.next();

        if (iter.value().use == true) {
            use = true;
            break;
        }
    }
    return use;
}

/// <summary>
/// 
/// 
/// </summary>
/// <returns></returns>
bool SmartOptimizeInfo::isBootingOptimize() const
{
    if (_smartinfo.count(1) == 0) {
        qWarning("Reserve Data is wrong");
        return false;
    }

    const auto &data = _smartinfo[1];
    if (data.type == ReservType::PCON) {
        //return true;
        return data.use;
    }
        
    return false;
}

bool SmartOptimizeInfo::isPcOffOptimize() const
{
    if (_smartinfo.count(1) == 0) {
        qWarning("Reserve Data is wrong");
        return false;
    }

    //const auto& data = _smartinfo[1];
    //if (data.type == ReservType::PCOFF) {
    //    return data.use;
    //    //return true;
    //}
        
    return false;
}

void SmartOptimizeInfo::setReserve(int key, ReserveData data, bool del)
{
    try
    {
        if (_smartinfo.count(key) == 0) {
            qWarning("Reserve Data is wrong");
            return;
        }

        _smartinfo[key] = data;

        QFile file(_cfgpath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QJsonParseError JsonParseError;
        QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
        file.close();


        QJsonObject RootObject = JsonDocument.object();
        QJsonValueRef ref = RootObject.find(QString("%1").arg(key)).value();
        QJsonObject addvalue = ref.toObject();

        addvalue.insert("type", (int)data.type);
        addvalue.insert("weektype", (int)data.weektype);
        addvalue.insert("monthtype", (int)data.monthtype);
        addvalue.insert("hour", (int)data.hour);
        addvalue.insert("min", (int)data.min);
        addvalue.insert("use", (bool)data.use);

        ref = addvalue;
        JsonDocument.setObject(RootObject);
        if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
            return;
        file.write(JsonDocument.toJson());
        file.close();

        if(!del) //삭제는 따로 보낸다(delReserve)
            emit reserveUpdateComplete();

        if (data.use == true) {
             //우선 제거
            ConfigData::inst()->setStartupOptOnly(true);
        }
    }
    catch (const std::exception&)
    {

    }
    
}

void SmartOptimizeInfo::delReserve(QList<int> list)
{
    for (auto key : list)
    {
        if (_smartinfo.count(key) == 0)
            continue;

        ReserveData data = _smartinfo[key];
        data.use = false;
        data.hour = 0;
        data.min = 0;
        data.weektype = ReservWeekType::NONE;
        data.monthtype = ReservMonthType::NONE;

        _smartinfo[key] = data;

        setReserve(key, data, true);
    }

    emit reserveDelComplete();
}