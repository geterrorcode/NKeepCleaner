#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class WebPageCaller  : public QObject
{
	Q_OBJECT

public:
	WebPageCaller(QObject *parent = Q_NULLPTR);
	~WebPageCaller();

private:
	QNetworkAccessManager* _manager;

public:
	void fetchData(const QUrl& url);

Q_SIGNALS:
	void dataFetched(const QString& data, const QUrl& url);

private  Q_SLOTS:
	void onFinished(QNetworkReply* reply);
};
