#pragma once

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "../ux/singleton.h"

class AutoChecker  : public QObject, public common::ux::Singleton<AutoChecker>
{
	Q_OBJECT

public:
	AutoChecker(QObject *parent = Q_NULLPTR);
	~AutoChecker();

	void startAuthCheck();

	void sendUUIDUpdate(const QString id, bool andopen = false);

	void getSerialDetail();

	void getPaymentURL();
	void getExchangePaymentURL();
	void SetAutoPayTerminate();

private:
	QNetworkAccessManager* _manager;

	bool _anyopenType;

	int getOldestId(const QJsonArray& receipts);

Q_SIGNALS:
	//void sendExparedDays(int days, QString resCode);
	void sendExpreedInfo(int days, QString date, QString rescode);
	void resUUIDUpdate(bool success, QString rescode, QString errormsg, 
		int remainDay = 0, QString expiredDate = "", bool anyopentype = false);
	void resAuthDetailComplete();
	void resAutoPayTerComplete(const QString res_msg, const int res_code);

	void resCallPaymentURL(const QString url, const int rescode);
	void resCallExPaymentURL(const QString url, const int rescode);

private Q_SLOTS:
	//void authCheckComplete();
	void authCheckFinished();
	void authCheckError(QNetworkReply::NetworkError error);

	//uuidupdate
	void uuidUpdateFinished();
	void uuidUpdateError(QNetworkReply::NetworkError error);

	//detail
	void getSerialDetailFinished();
	void getSerialDetailError(QNetworkReply::NetworkError error);

	//get payment url
	void getPaymentURLFinished();
	void getPaymentURLError(QNetworkReply::NetworkError error);

	//get Exchange payment url
	void getExPaymentURLFinished();
	void getExPaymentURLError(QNetworkReply::NetworkError error);


	//res Auto Pay terminate
	void resAutoPayTerminateFinished();
	void resAutoPayTerminateError(QNetworkReply::NetworkError error);
};
