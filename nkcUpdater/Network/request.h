#pragma once

#include <QNetworkReply>
#include <QJsonObject>
#include <functional>

namespace Request {

typedef QMap<QString, QString> Data;

struct File
{
    QString name;
    QString type;
    QByteArray bytes;
};
typedef QMap<QString, File> Files;

typedef std::function<void (QNetworkReply::NetworkError error, int statusCode, QVariant result)> Callback;

void call(QObject* context, QUrl url, QString method, Data headers, Data body, Files files, Callback callback);
void call(QObject* context, QUrl url, QString method, Data headers, QJsonObject json, Callback callback);

}
