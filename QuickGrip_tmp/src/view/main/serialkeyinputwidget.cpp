#include "serialkeyinputwidget.h"

//#include <QFrame>

#include <QLabel>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QKeyEvent>
#include <QjsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDesktopServices>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

#include "../QuickGrip_tmp.h"

#include "view/Component/containerlayout.h"
#include "view/Component/customlabel.h"
#include "view/Component/button.h"
#include "view/Component/clickablelabel.h"
#include "view/common/alertpopup.h"

#include "view/Component/serialcodeedit.h"

//#include <common/network/authapimanager.h>
#include <common/tool/utils.h>
#include <Logger.h>

#include "../../common/defines/ssdefins.h"
#include "../../ui.h"

SerialKeyInputWidget::SerialKeyInputWidget(QWidget *parent)
	: QWidget(parent), _codeedit(Q_NULLPTR), _uuid("")
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(788, 472);
	//moveToCenter(this);
	auto baseLayout = new ContainerLayout(this);
	{
		// <background
		auto backgroundFrame = new QFrame;
		backgroundFrame->setStyleSheet("border-image: url(:/main/resource/main/mainbg.png); background-color: #2E3135; border: 1px solid #cecece; border-radius: 3px;");
		baseLayout->addWidget(backgroundFrame);
			
	}


	auto title = new QLabel(this);
	title->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/title.png"));
	title->setGeometry(10, 10, 217, 52);
	title->show();

	auto close = new Button(this);
	{
		close->setCursor(Qt::PointingHandCursor);
		close->setStyleSheet(ssdefines::imageButton.arg(":main/resource/main/close.png",
			":main/resource/main/close_over.png",
			":main/resource/main/close_press.png",
			":main/resource/main/close.png"));

		connect(close, &Button::clicked, [=]() {
			QCoreApplication::quit();
			});

		close->setGeometry(748, 10, 30,30);
		close->show();
	}

	auto msg = new CustomLabel(24, "#FFFFFF", true, 0, 0, this);
	msg->setText("シリアル番号を入力してください"); //시리얼 키를 입력 하세요.
	msg->setGeometry(0, 150, 788, 40);
	msg->setAlignment(Qt::AlignCenter);
	msg->show();

	int x = 400;
	//_codeedit = new SerialCodeEdit(this);
	_codeedit = new SerialCodeEdit(this);
	_codeedit->setStyleSheet(ssdefines::editcoutrol);
	_codeedit->setGeometry(788/2 - x /2 , 270, 400, 40);
	_codeedit->show();

	auto okbtn = new Button(this);
	{ //del
		okbtn->setText("確認"); //
		
		okbtn->setCursor(Qt::PointingHandCursor);
		QString ss = ssdefines::imageButton.arg(":preview/resource/preview/okbtn.png",
			":preview/resource/preview/okbtn.png",
			":preview/resource/preview/okbtn.png",
			":preview/resource/preview/okbtn.png") + QString("QAbstractButton {color:#000000; font: bold normal 14px;}");
		okbtn->setStyleSheet(ss);

		connect(okbtn, &Button::clicked, [=]() {

			checkSerialkey();

			});

		okbtn->setGeometry(788 / 2 - 76, 370, 153, 40);
		okbtn->show();
	}

	//QDate nowdate = QDate::currentDate();
	//QString date = nowdate.toString("yyyy-MM-dd");
	//_uuid =  QString("%1-%2").arg(utils::getUUID()).arg(date);

	//_uuid = "anyopen"; //우선 다 열리게
	_uuid = ConfigData::inst()->getUUID();


	/*QString imageWidget = "QWidget { background-color: rgba(255, 255, 255, 0); border-image: url(:/common/resource/common/checkbox_check.png);  background-repeat: no-repeat;}";
	setStyleSheet(imageWidget);*/


	//setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/mainbg.png"));
	//auto baseLayout = new ContainerLayout(this);
	//{
	//	auto backgroundFrame = new QFrame;
	//	backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/mainbg.png"));
	//	baseLayout->addWidget(backgroundFrame);
	//}

	/// <summary>
	/// 문의하기
	/// </summary>
	/// <param name="parent"></param>
	auto sendemail = new ClickableLabel(this);
	sendemail->setText("認証関連のお問い合わせ");
	sendemail->setGeometry(0, 430, 788, 40);
	sendemail->setAlignment(Qt::AlignHCenter);
	connect(sendemail, &ClickableLabel::clicked, [=]() {

		QDesktopServices::openUrl(QUrl("mailto:?to=helpdesk@tni-sc.com&subject=N-KeepCleaner お問い合わせメール&body=シリアル番号の認証ができない場合は、発行されたシリアル番号とお問い合わせを入力し送信してください。\r\nすぐに確認いたします。\r\n\r\n", QUrl::TolerantMode));

		});

	sendemail->show();

	_manager = new QNetworkAccessManager(this);
}

SerialKeyInputWidget::~SerialKeyInputWidget()
{
	if (_manager)
		_manager->deleteLater();
}

void SerialKeyInputWidget::checkSerialkey()
{

	//체크 후 파일 생성 및 갱신
	QString data = _codeedit->text().replace("-", "");

	if (data.isEmpty()) { //시리얼 번호를 입력하세요.
		auto alert = new AlertPopup(tr("シリアル番号を入力してください"), QObject::tr("確認"), "", false, window());
		alert->show();
		return;
	}

	if (data.length() != 12) {
		PLOG_INFO << "SerialKeyInput, Input SerialKey :  " << data;
		//시리얼 키가 맞지 않습니다.  시리얼 키를 확인 해주세요
		auto alert = new AlertPopup(tr("シリアル番号が一致しません。 \r\nシリアル番号を確認してください。"), QObject::tr("確認"), "", true, window());
		alert->show();
		return;
	}

	/*
	* 2024.01.19 Auth Server 변경
	*/
	//QUrl url("https://dev.tniauth.com/api/certification/serialKey/validation");
	QUrl url(QString("%1/api/certification/serialKey/validation").arg(AUTHSERVER_URL));
	QJsonDocument JsonDocument;
	QJsonObject obj;
	obj["p_key"] = data;
	obj["p_device"] = "PC";
	obj["p_uuid"] = _uuid;
	JsonDocument.setObject(obj);

	QString test(JsonDocument.toJson());

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	PLOG_INFO << "SerialKeyInput, Call URL :" << url.toString();
	QNetworkReply* reply = _manager->post(request, JsonDocument.toJson());

	connect(reply, &QNetworkReply::finished, this, &SerialKeyInputWidget::AuthCheckFinished);
	connect(reply, &QNetworkReply::errorOccurred, this, &SerialKeyInputWidget::handleError);



	/*
	* old Version Auth Server
	*/
	/*QJsonDocument JsonDocument;
	QJsonObject obj;
	obj["p_key"] = data;
	obj["p_device"] = "pc";
	obj["p_uuid"] = _uuid;
	JsonDocument.setObject(obj);

	QUrl url("https://tniauth.com/api/certification");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QNetworkReply* reply = _manager->post(request, JsonDocument.toJson());
	connect(reply, &QNetworkReply::finished, this, &SerialKeyInputWidget::handleFinished);
	connect(reply, &QNetworkReply::errorOccurred, this, &SerialKeyInputWidget::handleError);*/
	
}


void SerialKeyInputWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
		checkSerialkey();
	}
	else {
		// 그 외의 키 이벤트를 무시.
		event->ignore();
	}
}

//NOT USE
//void SerialKeyInputWidget::handleFinished()
//{
//	PLOG_INFO << "SerialKeyInput auth Finished";
//
//	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
//
//	if (reply->error() == QNetworkReply::NoError) {
//		
//		// success
//		QJsonParseError jsonError;
//		QByteArray response = reply->readAll();
//		QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
//		if (jsonError.error != QJsonParseError::NoError) {
//			//qDebug() << "Failed to parse JSON:" << jsonError.errorString();
//			PLOG_ERROR << "SerialKeyInputWidget :: Failed to parse JSON:" << jsonError.errorString();
//			AuthFail("(x10001)");
//			return;
//		}
//
//		if (!jsonDoc.isObject()) {
//			//qDebug() << "JSON is not an object";
//			PLOG_ERROR << "SerialKeyInputWidget :: JSON is not an object";
//			AuthFail("(x10002)");
//			return;
//		}
//
//		// {"return":"true","code":"000","fire_day":"20240517"}
//		QJsonObject jsonObj = jsonDoc.object();
//		
//		bool success = false;
//		auto res = jsonObj["return"];
//		if (res.isString()) {
//			if (res.toString() == "true")
//				success = true;
//			else success = false;
//
//			if (success) {
//				AuthSuccess();
//				auto expired = jsonObj["fire_day"];
//				PLOG_INFO << "SerialKeyInputWidget :: SUCCESS(fire_day):" << expired;
//			}
//			else { //fail
//
//				QString error;
//				if (jsonObj["code"].isString())
//					error = jsonObj["code"].toString();
//				PLOG_ERROR << "SerialKeyInputWidget :: ERROR(code):" << error;
//				AuthFail(error);
//			}
//		}
//		else 
//			AuthFail("(x10003)");
//	}
//	else {
//		// failure
//		//qDebug() << "Failure" << reply->errorString();
//		PLOG_ERROR << "SerialKeyInputWidget :: Failure(code):" << reply->errorString();
//		AuthFail("(x10004)");
//	}
//
//	reply->deleteLater();
//}

void SerialKeyInputWidget::handleError(QNetworkReply::NetworkError error)
{
	try
	{
		QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
		if (reply->error() == QNetworkReply::ProtocolInvalidOperationError) {
			// success
			QJsonParseError jsonError;
			QByteArray response = reply->readAll();
			QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
			if (jsonError.error != QJsonParseError::NoError) {
				PLOG_ERROR << "SerialKeyInputWidget jsonError Error";
				emit AuthResultCheck(false, "010", "Type : E1");
				return;
			}

			if (!jsonDoc.isObject()) {
				PLOG_ERROR << "SerialKeyInputWidget Object Error";
				emit AuthResultCheck(false, "010", "Type : E2");
				return;
			}

			QJsonObject jsonObj = jsonDoc.object();

			bool success = false;
			auto status = jsonObj["status"];
			if (status.isBool()) {
				if (status.toBool() == true)
					success = true;
				else success = false;

				QString error;
				if (jsonObj["code"].isString())
					error = jsonObj["code"].toString();

				if (error == "004" || error == "009") { //만료 나 UUID 중복 경우는 메인화면에서 다시 검증한다.
					PLOG_INFO << "SerialKeyInputWidget ByPass :  " << error;
					emit AuthResultCheck(true, error, "");
				}
				else {
					PLOG_ERROR << "SerialKeyInputWidget ErrorCode :  " << error;
					emit AuthResultCheck(false, error, "Type : E3");
				}
			}
			else {
				PLOG_ERROR << "SerialKeyInputWidget [status]Error ";
				emit AuthResultCheck(false, "010", "Type : E4");
			}
		}
		else {
			QString errorstring = reply->errorString();
			PLOG_ERROR << "SerialKeyInputWidget handleError : " << errorstring;

			// failure
			emit AuthResultCheck(false, "010", errorstring);
		}


		reply->deleteLater();
	}
	catch (const std::exception&)
	{
		PLOG_ERROR << "SerialKeyInputWidget exception Error";
	}
}

void SerialKeyInputWidget::AuthCheckFinished()
{
	try
	{
		QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
		if (reply->error() == QNetworkReply::NoError) {
			// success
			QJsonParseError jsonError;
			QByteArray response = reply->readAll();
			QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
			if (jsonError.error != QJsonParseError::NoError) {
				qDebug() << "Failed to parse JSON:" << jsonError.errorString();
				emit AuthResultCheck(true, "010", "E01");
				return;
			}

			if (!jsonDoc.isObject()) {
				qDebug() << "JSON is not an object";
				emit AuthResultCheck(true, "010", "E01");
				return;
			}

			/*
			*	{
					"status": boolean, // 성공 여부
					"code": String, // 반환 코드
					"expireDate": String Date, // 시리얼 키 만료 날짜
					"remainDays": integer // 만료까지 남은 일 수 .
				}
			*/

			QJsonObject jsonObj = jsonDoc.object();

			bool success = false;
			auto status = jsonObj["status"];
			if (status.isBool()) {
				if (status.toBool() == true)
					success = true;
				else success = false;

				if (success) {
					PLOG_INFO << "SerialKeyInputWidget AuthCheckFinished Success!";
					emit AuthResultCheck(true, "000", "");
				}
				else { //fail

					QString error;
					if (jsonObj["code"].isString())
						error = jsonObj["code"].toString();

					if (error == "004" || error == "009") { //만료 나 UUID 중복 경우는 메인화면에서 다시 검증한다.
						PLOG_INFO << "SerialKeyInputWidget AuthCheckFinished ByPass : " << error;
						emit AuthResultCheck(true, error, "");
					}
					else {
						PLOG_ERROR << "SerialKeyInputWidget AuthCheckFinished error : " << error;
						emit AuthResultCheck(false, error, "E03");
					}
				}
			}
			else {
				PLOG_ERROR << "SerialKeyInputWidget AuthCheckFinished [status]Error";
				emit AuthResultCheck(false, "010", "E04");
			}
		}
		else {
			// failure
			QString errorstring = reply->errorString();
			PLOG_ERROR << "SerialKeyInputWidget AuthCheckFinished failure :" << errorstring;
			emit AuthResultCheck(false, "010", errorstring);
		}

		reply->deleteLater();
	}
	catch (const std::exception&)
	{
		PLOG_ERROR << "SerialKeyInputWidget AuthCheckFinished exception";
	}
	
}

void SerialKeyInputWidget::AuthResultCheck(bool success, QString resCode, QString errormsg)
{
	if (success) {
		QString serial = _codeedit->text().replace("-", "");
		ConfigData::inst()->setAuthInfo(serial, _uuid);

		auto alert = new AlertPopup(tr("シリアル番号が確認されました"), QObject::tr("確認"), "", false, window());//시리얼키 확인 되었습니다
		connect(alert, &AlertPopup::okClicked, [=]() {
			QuickGrip_tmp::inst()->show();
			emit QuickGrip_tmp::inst()->setAuthCheck();
			this->hide();
			this->destroyed();

			});
		alert->show();
	}
	else {
		QString errmsg;
		if (resCode == "001") {
			errmsg = "確認できないシリアル番号です";
		}
		else if (resCode == "003") {
			errmsg = "シリアル番号が一致しません。 \r\nシリアル番号を確認してください。";
		}
		else {
			errmsg = QString("指定されていない認証エラー(errorcode:%1)です。\r\n再度実行してください。").arg(errormsg);
		}

		auto alert = new AlertPopup(errmsg, QObject::tr("確認"), "", true, window());
		alert->show();
	}
}

void SerialKeyInputWidget::AuthSuccess()
{
	QString serial = _codeedit->text().replace("-", "");
	ConfigData::inst()->setAuthInfo(serial, _uuid);
	/*QString appLocalPath = QString("%1/N-KeepCleaner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
	if (QDir(appLocalPath).exists() == false) {
		QDir().mkdir(appLocalPath);
	}

	QFile serialfile(QString("%1/serial").arg(appLocalPath));
	if (serialfile.open(QIODevice::ReadWrite)) {
		QTextStream stream(&serialfile);
		stream << data;
		serialfile.close();
	}

	QFile uniquefile(QString("%1/unique").arg(appLocalPath));
	if (uniquefile.open(QIODevice::ReadWrite)) {
		QTextStream stream(&uniquefile);
		stream << _uuid;
		uniquefile.close();
	}*/

	auto alert = new AlertPopup(tr("シリアル番号が確認されました"), QObject::tr("確認"), "", false, window());//시리얼키 확인 되었습니다
	connect(alert, &AlertPopup::okClicked, [=]() {
		QuickGrip_tmp::inst()->show();
		this->hide();
		this->destroyed();

		});
	alert->show();
}

void SerialKeyInputWidget::AuthFail(QString error_code)
{
	/*
	*결과 코드 설명
		000 : 반환 성공
		001 : request에 key 없음
		002 : request에 device 없음
		003 : 존재하지 않는 키
		004 : 이미 활성화 되어 있는 키 >> 변경됨
		>> 004 변경 : 키는 등록되어 있는데 uuid 가 일치 하지 않음

		005 : request에 uuid 없음
	*/
	QString errmsg;
	if (error_code == "001") {
		errmsg = "確認できないシリアル番号です";
	}
	else if (error_code == "002") {
		errmsg = "確認できないデバイスです";
	}
	else if (error_code == "003") {
		errmsg = "シリアル番号が一致しません。 \r\nシリアル番号を確認してください。";
	}
	else if (error_code == "004") {
		errmsg = "登録されているデバイスでは\r\nありません"; //등록된 device가 아닙니다.
	}
	else {
		errmsg = QString("指定されていない認証エラー(errorcode:%1)です。\r\n再度実行してください。").arg(error_code);
	}


	auto alert = new AlertPopup(errmsg, QObject::tr("確認"), "", false, window());//시리얼키 확인 되었습니다
	alert->show();

}

void SerialKeyInputWidget::setGotoShow()
{
	const Qt::WindowFlags flags(windowFlags());
	this->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
	activateWindow();
	show();


	this->setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
	show();
}