#pragma once

#include <QNetworkReply>
#include <QByteArray>
#include "../defineproperty/defineproperty.h"

class Loader : public QObject
{
    Q_OBJECT

    PROPERTY_NOCHANGE(QNetworkReply*, reply)

public:
    Loader(QNetworkReply* reply, QObject* parent = nullptr);
    Loader(QUrl url, QObject* parent = nullptr);
    Loader(QNetworkRequest request, QObject* parent = nullptr);

    ~Loader();

signals:
    void completed(QByteArray bytes);
    void progress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code, QString errorString);

private slots:
    void error(QNetworkReply::NetworkError code);
};
