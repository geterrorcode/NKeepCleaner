#pragma once

#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QPointer>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "../../common/ux/singleton.h"

class SerialKeyInputWidget  : public QWidget, public common::ux::Singleton<SerialKeyInputWidget>
{
	Q_OBJECT

public:
	SerialKeyInputWidget(QWidget *parent = Q_NULLPTR);
	~SerialKeyInputWidget();

protected:

private:
	QNetworkAccessManager* _manager;
	QPointer<QLineEdit> _codeedit;
	QString _uuid;

	void checkSerialkey();

	void keyPressEvent(QKeyEvent* event) override;

	void AuthSuccess();
	void AuthFail(QString error_code);

public:
	void setGotoShow();

private Q_SLOTS:
	//void handleFinished();
	void handleError(QNetworkReply::NetworkError error);

	void AuthCheckFinished();
	void AuthResultCheck(bool success, QString resCode, QString errormsg);


};
