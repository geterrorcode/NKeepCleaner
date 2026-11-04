#pragma once

#include <QObject>
#include <QApplication>

#include <QDebug>


class AuthApiManager  : public QObject
{
	Q_OBJECT

public:
	AuthApiManager(QString url, QString json, QObject *parent = Q_NULLPTR);
	~AuthApiManager();

private:
	QString _url;
	QString _json;

	void errorControl();

public:
	void startauth();

Q_SIGNALS:
	void sendAuthResult(bool success);
};
