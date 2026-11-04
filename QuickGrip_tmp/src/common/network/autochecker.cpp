#include "autochecker.h"

#include "../../../QuickGrip_tmp.h"

#include <QCoreApplication>

#include <QSingleApplication/SingleApplication>

#include <QFile>
#include <QTextStream>
#include <QStandardPaths>


#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

#include <Logger.h>
#include <common/tool/utils.h>


AutoChecker::AutoChecker(QObject *parent)
	: QObject(parent), _anyopenType(false)
{
	_manager = new QNetworkAccessManager(this);
	//startAuthCheck();
}

AutoChecker::~AutoChecker()
{}

void AutoChecker::startAuthCheck()
{
	QString serial = ConfigData::inst()->getSerialKey();
	QString uuid = ConfigData::inst()->getUUID();
	if (uuid.isEmpty()) {
		uuid = utils::getSystemUUID();
		ConfigData::inst()->setUUID(uuid);
	}


	if (serial.isEmpty() || uuid.isEmpty()) {
		emit sendExpreedInfo(-1, "", "003");
		return;
	}

	//dev
	//QUrl url("https://dev.tniauth.com/api/certification/serialKey/validation");
	QUrl url(QString("%1/api/certification/serialKey/validation").arg(AUTHSERVER_URL));
	QJsonDocument JsonDocument;
	QJsonObject obj;
	obj["p_key"] = serial;
	obj["p_device"] = "PC";
	obj["p_uuid"] = uuid;
	JsonDocument.setObject(obj);
	//strFromObj = "{\"p_device\":\"PC\",\"p_key\":\"YBNGBT194KJ9\",\"p_uuid\":\"anyopen\"}";

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	PLOG_INFO << "start AuthCheck : validation, p_key : "<< serial << " ,p_uuid : "<< uuid;
    QNetworkReply * reply = _manager->post(request, JsonDocument.toJson());
    connect(reply, &QNetworkReply::finished, this, &AutoChecker::authCheckFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &AutoChecker::authCheckError);


}

void AutoChecker::sendUUIDUpdate(const QString id, bool andopen)
{
	//api/certification/serialKey/approvement
	/*
	{
    "receiptsId": integer, // Receipts Id
    "p_device": "IOS" || "ANDROID" || "PC", // Device type
    "p_uuid": String // 변경할 UUID
	}
	*/

	_anyopenType = andopen;
	//QUrl url("https://dev.tniauth.com/api/certification/receipts");
	QUrl url(QString("%1/api/certification/receipts").arg(AUTHSERVER_URL));
	QJsonDocument JsonDocument;
	QJsonObject obj;
	obj["receiptsId"] = id.toInt();
	obj["p_device"] = "PC";
	obj["p_uuid"] = ConfigData::inst()->getUUID();
	JsonDocument.setObject(obj);

	QString strJson(JsonDocument.toJson(QJsonDocument::Compact));

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	PLOG_INFO << "start receipts(UUID Update) : receiptsId : " << id << " ,p_uuid : " << ConfigData::inst()->getUUID();

	QNetworkReply* reply = _manager->put(request, JsonDocument.toJson());
	connect(reply, &QNetworkReply::finished, this, &AutoChecker::uuidUpdateFinished);
	connect(reply, &QNetworkReply::errorOccurred, this, &AutoChecker::uuidUpdateError);


}


/// <summary>
/// 시리얼 코드 세부 정보 확인 api
/// 시리얼 코드 검증이 끝난뒤 호출.
/// url(GET) : ~/api/certification/serialKey/detail/<serialcode>
/// </summary>
void AutoChecker::getSerialDetail()
{
	QString serial = ConfigData::inst()->getSerialKey();

	if (serial.isEmpty()) {
		//emit sendExpreedInfo(-1, "", "003");
		return;
	}

	QUrl url(QString("%1/api/certification/serialKey/detail/%2").arg(AUTHSERVER_URL).arg(serial));
	//QString url = QString("https://dev.tniauth.com/api/certification/serialKey/detail/YBNGBT194KJ9");
	QNetworkRequest request(url);
	QNetworkReply* reply = _manager->get(request);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	PLOG_INFO << "start getSerialDetail : validation: " << serial ;

	connect(reply, &QNetworkReply::finished, this, &AutoChecker::getSerialDetailFinished);
	connect(reply, &QNetworkReply::errorOccurred, this, &AutoChecker::getSerialDetailError);
}

//
//void AutoChecker::authCheckComplete()
//{
//	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
//	if (reply->error() == QNetworkReply::NoError) {
//		// success
//		QJsonParseError jsonError;
//		QByteArray response = reply->readAll();
//		QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
//		if (jsonError.error != QJsonParseError::NoError) {
//			qDebug() << "Failed to parse JSON:" << jsonError.errorString();
//			emit sendExpreedInfo(-1, "");
//			return;
//		}
//
//		if (!jsonDoc.isObject()) {
//			qDebug() << "JSON is not an object";
//			emit sendExpreedInfo(-1, "");
//			return;
//		}
//
//		/*
//			{
//				"status": true,
//				"code": "000",
//				"expireDate": "2024-08-01",
//				"remainDays": 205
//			}
//		*/
//
//		QJsonObject jsonObj = jsonDoc.object();
//
//		bool success = false;
//		auto status = jsonObj["status"];
//		if (status.isBool()) {
//			if (status.toBool() == true)
//				success = true;
//			else success = false;
//
//			if (success) {
//				//AuthSuccess();
//				auto expireDate = jsonObj["expireDate"];
//				auto remainDays = jsonObj["remainDays"]; //int
//
//				emit sendExpreedInfo(remainDays.toInt(), "");
//			}
//			else { //fail
//
//				QString error;
//				if (jsonObj["code"].isString())
//					error = jsonObj["code"].toString();
//				emit sendExpreedInfo(-1, error);
//				//AuthFail(error);
//			}
//		}
//		else
//			emit sendExpreedInfo(-1, "");
//		//AuthFail("(x10003)");
//	}
//	else {
//		// failure
//		qDebug() << "<<Failure>>";
//		qDebug() << "Failure : " << reply->errorString();
//		//AuthFail("(x10004)");
//	}
//
//	reply->deleteLater();
//}

void AutoChecker::authCheckFinished()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::NoError) {
		// success
		QJsonParseError jsonError;
		QByteArray response = reply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
		if (jsonError.error != QJsonParseError::NoError) {
			//qDebug() << "Failed to parse JSON:" << jsonError.errorString();
			PLOG_ERROR << "Failed to parse JSON:" << jsonError.errorString();
			emit sendExpreedInfo(-1, "", "010");
			return;
		}

		if (!jsonDoc.isObject()) {
			PLOG_ERROR << "JSON is not an object";
			//qDebug() << "JSON is not an object";
			emit sendExpreedInfo(-1, "", "010");
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
				auto expireDate = jsonObj["expireDate"];
				auto remainDays = jsonObj["remainDays"]; //int
				emit sendExpreedInfo(remainDays.toInt(), expireDate.toString(), "000");

				//성공 시 세부정보 확인
				getSerialDetail();

			}
			else { //fail

				QString error;
				if (jsonObj["code"].isString())
					error = jsonObj["code"].toString();

				if (error == "009") {
					
					QJsonArray jsonArray = jsonObj["receipts"].toArray();

					if (jsonArray.count() <= 0) { //서버 오류인경우
						PLOG_ERROR << "Server Error : 001";

						emit sendExpreedInfo(-1, "", "001");
						return;
					}
					else {
						// 최소 ID 값을 찾기
						int minId = getOldestId(jsonArray);
						if (minId < 0) {

							PLOG_ERROR << "Auth Error. Get OldestId :  " << minId;

							for (const QJsonValue& value : jsonArray) {
								QJsonObject obj = value.toObject();
								int id = obj["id"].toInt();
								if (id < minId) {
									minId = id;
								}
							}
						}


						PLOG_ERROR << "Auth Error. ErrorCode :  " << error << LOG_TAB() << "minId : " <<QString::number(minId);

						emit sendExpreedInfo(-1, QString::number(minId), error);
					}
				}
				else {
					PLOG_ERROR << "Auth Error. ErrorCode :  " << error;

					emit sendExpreedInfo(-1, "", error);
				}
			}
		}
		else{

			PLOG_ERROR << "Auth Error. [status] Not Find! ErrorCode : 010 ";
			emit sendExpreedInfo(-1, "", "010");
		}
	}
	else {
		// failure
		PLOG_ERROR << "Auth Error. [reply] Error! ErrorCode : 010 ";
		emit sendExpreedInfo(-1, "", "010");
	}

	reply->deleteLater();
}

void AutoChecker::authCheckError(QNetworkReply::NetworkError error)
{
	PLOG_ERROR << "authCheckError ! ";

	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::ProtocolInvalidOperationError) {
		// success
		QJsonParseError jsonError;
		QByteArray response = reply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
		if (jsonError.error != QJsonParseError::NoError) {
			PLOG_ERROR << "authCheckError Json Error ";
			emit sendExpreedInfo(-1, "", "010");
			return;
		}

		if (!jsonDoc.isObject()) {
			PLOG_ERROR << "authCheckError isObject Error ";
			emit sendExpreedInfo(-1, "", "010");
			return;
		}

		QJsonObject jsonObj = jsonDoc.object();

		bool success = false;
		auto status = jsonObj["status"];
		if (status.isBool()) {
			if (status.toBool() == true)
				success = true;
			else success = false;

			if (success) {
				auto expireDate = jsonObj["expireDate"];
				auto remainDays = jsonObj["remainDays"]; //int
				emit sendExpreedInfo(remainDays.toInt(), expireDate.toString(), "000");
			}
			else { //fail

				QString error;
				if (jsonObj["code"].isString())
					error = jsonObj["code"].toString();

				if (error == "009") {
					PLOG_ERROR << "authCheckError Error : 009 ";
					emit sendExpreedInfo(-1, "", error);
				}
				else {
					PLOG_ERROR << "authCheckError ErrorCode :  " << error;
					emit sendExpreedInfo(-1, "", error);
				}
					
			}
		}
		else {
			PLOG_ERROR << "authCheckError [status] Error ";
			emit sendExpreedInfo(-1, "", "010");
		}
			
	}
	else {

		int error = reply->error();
		QString errorstring = reply->errorString();
		PLOG_ERROR << "authCheckError reply Error :  "<< error <<LOG_TAB() << "errorstring:" << errorstring;
		// failure
		emit sendExpreedInfo(-1, "", "010");
	}


	reply->deleteLater();
}

void AutoChecker::uuidUpdateFinished()
{
	/*
	{
    "status": true,
    "code": "000",
    "message": "success fully changed Receipts Modify",
    "data": {
        "id": integer, // Receipts Primary Key
        "serialKey": integer, // SerialKey Primary Key
        "deviceType": "IOS" || "ANDROID" || "PC", // 변경 된 Device Type
        "UUID": String // 변경 된 UUID
		}
	}
	*/

	PLOG_INFO << "uuid Update Finished";
	try
	{
		QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
		if (reply->error() == QNetworkReply::NoError) {

			// success
			QJsonParseError jsonError;
			QByteArray response = reply->readAll();
			QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
			if (jsonError.error != QJsonParseError::NoError) {
				//qDebug() << "Failed to parse JSON:" << jsonError.errorString();
				PLOG_ERROR << "Failed to parse JSON:" << jsonError.errorString();
				emit resUUIDUpdate(false, "010", "Type : E1");
				return;
			}

			if (!jsonDoc.isObject()) {
				//qDebug() << "JSON is not an object";
				PLOG_ERROR << "JSON is not an object";
				emit resUUIDUpdate(false, "010", "Type : E2");
				return;
			}

			QJsonObject jsonObj = jsonDoc.object();

			bool success = false;
			auto status = jsonObj["status"];
			if (status.isBool()) {
				if (status.toBool() == true)
					success = true;
				else success = false;

				if (success) {

					// "data" 객체 안의 값을 가져옴
					QJsonObject dataObj = jsonObj.value("data").toObject();

					auto remainDays = dataObj.value("remainDays"); //int
					auto expireDate = dataObj.value("expireDate");

					int inttext = remainDays.toInt();
					QString stringtest = expireDate.toString();

					if (_anyopenType)
						emit resUUIDUpdate(true, "000", "anyopen", remainDays.toInt(), expireDate.toString(), true);
					else
						emit resUUIDUpdate(true, "000", "", remainDays.toInt(), expireDate.toString(), false);
				}
				else { //fail
					QString error;
					if (jsonObj["code"].isString())
						error = jsonObj["code"].toString();

					emit resUUIDUpdate(false, error, "Type : E3");
				}
			}
			else
				emit resUUIDUpdate(false, "010", "Type : E4");
		}
		else {
			// failure

			QString errorstring = reply->errorString();
			emit resUUIDUpdate(false, "010", errorstring);
		}

		reply->deleteLater();
	}
	catch (const std::exception &e)
	{
		PLOG_INFO << "uuid Update exception Error! : "<< e.what();
	}

}

void AutoChecker::uuidUpdateError(QNetworkReply::NetworkError error)
{
	PLOG_ERROR << "uuid Update Error";

	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::ProtocolInvalidOperationError) {
		// success
		QJsonParseError jsonError;
		QByteArray response = reply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
		if (jsonError.error != QJsonParseError::NoError) {
			PLOG_ERROR << "uuid Update Error Code : 010, Type : E1";
			emit resUUIDUpdate(false, "010" ,  "Type : E1");
			return;
		}

		if (!jsonDoc.isObject()) {
			PLOG_ERROR << "uuid Update Error Code : 010, Type : E2";
			emit resUUIDUpdate(false, "010",  "Type : E2");
			return;
		}

		QJsonObject jsonObj = jsonDoc.object();

		bool success = false;
		auto status = jsonObj["status"];
		if (status.isBool()) {
			if (status.toBool() == true)
				success = true;
			else success = false;

			if (success) {
			}
			else { //fail
				QString error;
				if (jsonObj["code"].isString())
					error = jsonObj["code"].toString();

				PLOG_ERROR << "uuid Update Error Code : 010, Type : E3";
				emit resUUIDUpdate(false, error, "Type : E3");
			}
		}
		else {
			PLOG_ERROR << "uuid Update Error Code : 010, Type : E4";
			emit resUUIDUpdate(false, "010", "Type : E4");
		}
	}
	else {

		//int error = reply->error();
		QString errorstring = reply->errorString();
		// failure
		PLOG_ERROR << "uuid Update Error Code : 010, Type : " << errorstring;
		emit resUUIDUpdate(false, "010", errorstring);
	}


	reply->deleteLater();
}

int AutoChecker::getOldestId(const QJsonArray& receipts)
{
	// 초기 가장 오래된 날짜와 id를 정의. 최초의 "updated_at" 값을 최신으로 설정
	QDateTime oldestDate = QDateTime::fromString("9999-12-31T23:59:59.999Z", Qt::ISODate);
	int oldestId = -1;

	for (const QJsonValue& value : receipts) {
		QJsonObject obj = value.toObject();
		QDateTime updatedDate = QDateTime::fromString(obj["updated_at"].toString(), Qt::ISODate);
		int id = obj["id"].toInt();

		//가장 오래된 id 확인, 중복된 date인 경우 가장 작은 id로 리턴
		if (updatedDate < oldestDate || (updatedDate == oldestDate && id < oldestId)) {
			oldestDate = updatedDate;
			oldestId = id;
		}
	}

	return oldestId;
}

void AutoChecker::getSerialDetailFinished()
{
	/*
	{
	"id": integer,
    "serialKey": string,
    "companyId": integer,
    "APIFire": integer,
    "status": "APPROVED" || "OK" || "LOCKED",
    "expireDate": string, // 만료 날짜
    "auto_renew": boolean, // 자동 갱신 여부
    "initialized": boolean, // 최초 결제 혹은 약관동의 여부
    "is_subscribing": boolean, // 현재 구독 여부
    "nextBillingDate": string, // 다음 결제 일
    "contractId": string || null // ContractId(StoreFront), StoreFront SerialKey 가 아닌 경우  NULL
    "billings": [...object] // 결제 정보
	}
	*/

	PLOG_INFO << "Get Serial Detail Finished";
	try
	{
		QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
		if (reply->error() == QNetworkReply::NoError) {

			// success
			QJsonParseError jsonError;
			QByteArray response = reply->readAll();
			QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
			if (jsonError.error != QJsonParseError::NoError) {
				//qDebug() << "Failed to parse JSON:" << jsonError.errorString();
				PLOG_ERROR << "getSerialDetailFinished Failed to parse JSON:" << jsonError.errorString();
				//emit resUUIDUpdate(false, "010", "Type : E1");
				return;
			}

			if (!jsonDoc.isObject()) {
				PLOG_ERROR << "getSerialDetailFinished JSON is not an object";
				//emit resUUIDUpdate(false, "010", "Type : E2");
				return;
			}

			QJsonObject jsonObj = jsonDoc.object();

			auto expireDate = jsonObj["expireDate"];
			QString exDate("");
			if (expireDate.isString()) {
				exDate = jsonObj["expireDate"].toString();
			}

			auto auto_renew = jsonObj["auto_renew"];
			bool autorenewal = false;
			if (auto_renew.isBool()) {
				autorenewal = jsonObj["auto_renew"].toBool();
				ConfigData::inst()->setAutoRenewal(autorenewal);
			}

			auto is_subscribing = jsonObj["is_subscribing"];
			bool subscribing = false;
			if (is_subscribing.isBool()) {
				subscribing = jsonObj["is_subscribing"].toBool();
				ConfigData::inst()->setSubscribe(subscribing);
			}

			auto nextBillingDate = jsonObj["nextBillingDate"];
			QString billingDate("");
			if (nextBillingDate.isString()) {
				billingDate = jsonObj["nextBillingDate"].toString();
				ConfigData::inst()->setBillings(billingDate);
			}


			auto contractId = jsonObj["contractId"];
			QString cId("");
			if (contractId.isString()) { //월정액 판단
				cId = jsonObj["contractId"].toString();
				ConfigData::inst()->setContractId(cId);
			}

			emit resAuthDetailComplete();
			
		}
		else {
			// failure
			QString errorstring = reply->errorString();
			PLOG_ERROR << "Get Serial Detail Finished Error! : " << errorstring;
			//emit resUUIDUpdate(false, "010", errorstring);
		}

		reply->deleteLater();
	}
	catch (const std::exception& e)	{
		PLOG_INFO << "Get Serial Detail  exception Error! : " << e.what();
	}
}

void AutoChecker::getSerialDetailError(QNetworkReply::NetworkError error)
{
	PLOG_ERROR << "Get Serial Detail Error";

	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::ProtocolInvalidOperationError) {
		// success
		QJsonParseError jsonError;
		QByteArray response = reply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
		if (jsonError.error != QJsonParseError::NoError) {
			PLOG_ERROR << "Get Serial Detail Error Code : 010, Type : E1";
			//emit resUUIDUpdate(false, "010", "Type : E1");
			return;
		}

		if (!jsonDoc.isObject()) {
			PLOG_ERROR << "Get Serial Detail Error Code : 010, Type : E2";
			//emit resUUIDUpdate(false, "010", "Type : E2");
			return;
		}
	}
	else {

		//int error = reply->error();
		QString errorstring = reply->errorString();
		// failure
		PLOG_ERROR << "Get Serial Detail Error Code : 010, Type : " << errorstring;
		//emit resUUIDUpdate(false, "010", errorstring);
	}

	reply->deleteLater();
}


void AutoChecker::getPaymentURL()
{
	QString serial = ConfigData::inst()->getSerialKey();

	if (serial.isEmpty()) {
		PLOG_INFO << "getPaymentURL Error!: serial is Empty ";
		emit resCallPaymentURL("ErrorCode : EP-10", -10);
		return;
	}

	QUrl url(QString("%1/api/billing/automaticPayment/%2").arg(AUTHSERVER_URL).arg(serial));
	QNetworkRequest request(url);
	QNetworkReply* reply = _manager->get(request);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	PLOG_INFO << "start getPaymentURL : validation: " << serial;

	connect(reply, &QNetworkReply::finished, this, &AutoChecker::getPaymentURLFinished);
	connect(reply, &QNetworkReply::errorOccurred, this, &AutoChecker::getPaymentURLError);
}

void AutoChecker::getPaymentURLFinished()
{
	PLOG_INFO << "Get PaymentURL Finished";
	try
	{
		QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
		if (reply->error() == QNetworkReply::NoError) {

			// success
			QJsonParseError jsonError;
			QByteArray response = reply->readAll();
			QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
			if (jsonError.error != QJsonParseError::NoError) {
				//qDebug() << "Failed to parse JSON:" << jsonError.errorString();
				PLOG_ERROR << "getPaymentURLFinished Failed to parse JSON:" << jsonError.errorString();
				emit resCallPaymentURL("ErrorCode : EP01", 1);
				return;
			}

			if (!jsonDoc.isObject()) {
				PLOG_ERROR << "getPaymentURLFinished JSON is not an object";
				emit resCallPaymentURL("ErrorCode : EP02", 2);
				return;
			}

			QJsonObject jsonObj = jsonDoc.object();

			auto payurl = jsonObj["URL"];
			QString paymentURL("");
			if (payurl.isString()) 
				paymentURL = jsonObj["URL"].toString();

			PLOG_ERROR << "PaymentURL Is Success";

			emit resCallPaymentURL(paymentURL, 0);
		}
		else {
			// failure
			QString errorstring = reply->errorString();
			PLOG_ERROR << "Get getPaymentURLFinished Error! : " << errorstring;
			emit resCallPaymentURL("ErrorCode : EP03", 3);
		}

		reply->deleteLater();
	}
	catch (const std::exception& e) {
		PLOG_INFO << "Get Serial Detail  exception Error! : " << e.what();
		emit resCallPaymentURL("ErrorCode : EP-1", -1);
	}
}

void AutoChecker::getPaymentURLError(QNetworkReply::NetworkError error)
{
	PLOG_ERROR << "Get PaymentURL Error";

	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::ProtocolInvalidOperationError) {
		PLOG_ERROR << "Get getPaymentURLError Error Code : EP04";
		emit resCallPaymentURL("ErrorCode : EP04", 4);
		return;
	}
	else {
		QString errorstring = reply->errorString();
		// failure
		PLOG_ERROR << "Get getPaymentURLError Error Code : EP05, Type : " << errorstring;
		emit resCallPaymentURL("ErrorCode : EP05", 5);
	}

	reply->deleteLater();
}

void AutoChecker::getExchangePaymentURL()
{
	QString serial = ConfigData::inst()->getSerialKey();

	if (serial.isEmpty()) {
		PLOG_INFO << "getExchangePaymentURL Error!: serial is Empty";
		emit resCallExPaymentURL("ErrorCode : XEP-10", -10);
		return;
	}

	QUrl url(QString("%1/api/billing/automaticPayment/%2/%3/change/fromApp").arg(AUTHSERVER_URL).arg(serial).arg(ConfigData::inst()->getBillings()));
	QNetworkRequest request(url);
	QNetworkReply* reply = _manager->get(request);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	PLOG_INFO << "start getExchangePaymentURL : validation: " << serial;

	connect(reply, &QNetworkReply::finished, this, &AutoChecker::getExPaymentURLFinished);
	connect(reply, &QNetworkReply::errorOccurred, this, &AutoChecker::getExPaymentURLError);
}

void AutoChecker::getExPaymentURLFinished()
{
	PLOG_INFO << "Get Ex PaymentURL Finished";
	try
	{
		QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
		if (reply->error() == QNetworkReply::NoError) {

			// success
			QByteArray response = reply->readAll();
			QString responseString = QString::fromUtf8(response);
			if (responseString.isEmpty()) {
				PLOG_ERROR << "Ex PaymentURL Is Empty";
				emit resCallExPaymentURL("ErrorCode : XEP01", 1);
				return;
			}
			QString exchangePaymentURL = responseString;
			
			// 문자열 양 끝의 따옴표 제거
			if (responseString.startsWith("\"") && responseString.endsWith("\"")) {
				exchangePaymentURL = responseString.mid(1, responseString.length() - 2);
			}

			PLOG_ERROR << "Ex PaymentURL Is Success";

			emit resCallExPaymentURL(exchangePaymentURL, 0);
		}
		else {
			// failure
			QString errorstring = reply->errorString();
			PLOG_ERROR << "Get getExPaymentURLFinished Error! : " << errorstring;
			emit resCallExPaymentURL("ErrorCode : XEP02", 2);
		}

		reply->deleteLater();
	}
	catch (const std::exception& e) {
		PLOG_INFO << "Get getExPaymentURL exception Error! : " << e.what();
		emit resCallExPaymentURL("ErrorCode : XEP-1", -1);
	}
}

void AutoChecker::getExPaymentURLError(QNetworkReply::NetworkError error)
{
	PLOG_ERROR << "Get ExPaymentURL Error";

	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	QString errorstring = reply->errorString();
	PLOG_ERROR << "Get ExPaymentURL  Error Code : 010, Type : " << errorstring;

	emit resCallExPaymentURL("ErrorCode : XEP03", 3);
	reply->deleteLater();
}

void AutoChecker::SetAutoPayTerminate()
{
	QString serial = ConfigData::inst()->getSerialKey();
	if (serial.isEmpty()) {
		//emit sendExpreedInfo(-1, "", "003");
		return;
	}

	QString apiurl(QString("%1/api/billing/automaticPaymentTerminate/%2").arg(AUTHSERVER_URL).arg(ConfigData::inst()->getSerialKey()));
	PLOG_INFO << "Call AutoPayTerminate : " << apiurl;
	QUrl url(apiurl);
	QNetworkRequest request(url);
	QNetworkReply* reply = _manager->get(request);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	PLOG_INFO << "start getExchangePaymentURL : validation: " << serial;

	connect(reply, &QNetworkReply::finished, this, &AutoChecker::resAutoPayTerminateFinished);
	connect(reply, &QNetworkReply::errorOccurred, this, &AutoChecker::resAutoPayTerminateError);

}


void AutoChecker::resAutoPayTerminateFinished()
{
	PLOG_INFO << "res AutoPayTerminate Finished";
	try
	{
		QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
		if (reply->error() == QNetworkReply::NoError) {

			// success
			QJsonParseError jsonError;
			QByteArray response = reply->readAll();
			QString responseString = QString::fromUtf8(response);

			if (responseString.isEmpty()) {
				PLOG_ERROR << "resAutoPayTerminate Is Empty";
				emit resAutoPayTerComplete("ErrorCode : ATP01", 1);
				return;
			}

			if (responseString.contains("true", Qt::CaseInsensitive)) {
				PLOG_ERROR << "resAutoPayTerminate Is Success";
				emit resAutoPayTerComplete("success", 0);
			}
			else {
				PLOG_ERROR << "resAutoPayTerminate Is Fail" << responseString;
				emit resAutoPayTerComplete("ErrorCode : ATP02", 2);
			}
		}
		else {
			// failure
			QString errorstring = reply->errorString();
			PLOG_ERROR << "resAutoPayTerminateFinished Error! : " << errorstring;
			emit resAutoPayTerComplete("ErrorCode : ATP03", 3);
		}

		reply->deleteLater();
	}
	catch (const std::exception& e) {
		PLOG_INFO << "Get resAutoPayTerminateFinished exception Error! : " << e.what();
		emit resAutoPayTerComplete("ErrorCode : ATP-1", -1);
	}
}

void AutoChecker::resAutoPayTerminateError(QNetworkReply::NetworkError error)
{
	PLOG_ERROR << "res AutoPayTerminate Error";

	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->error() == QNetworkReply::ProtocolInvalidOperationError) {
		// success
		QJsonParseError jsonError;
		QByteArray response = reply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonError);
		if (jsonError.error != QJsonParseError::NoError) {
			PLOG_ERROR << "resAutoPayTerminateError Code : 04";
			emit resAutoPayTerComplete("ErrorCode : ATP04", 4);
			return;
		}

		if (!jsonDoc.isObject()) {
			PLOG_ERROR << "resAutoPayTerminateError Code : 05";
			emit resAutoPayTerComplete("ErrorCode : ATP05", 5);
			return;
		}
	}
	else {

		//int error = reply->error();
		QString errorstring = reply->errorString();
		// failure
		PLOG_ERROR << "resAutoPayTerminateError Code : 06 : " << errorstring;
		emit resAutoPayTerComplete("ErrorCode : ATP06", 6);
	}

	reply->deleteLater();
}
