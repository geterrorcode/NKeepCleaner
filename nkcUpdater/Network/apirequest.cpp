#include "apirequest.h"



namespace Request {
namespace API {

void call(QObject* context, QUrl path, QString method, Data body, Files files, Callback callback)
{
    QString host;
    auto url = QUrl(QString("https://%1").arg(host)).resolved(path);

    Data header;
    Request::call(context, url, method, header, body, files, [=] (QNetworkReply::NetworkError error, int statusCode, QVariant result) {
        callback(statusCode, result.toMap());
    });
}

void call(QObject* context, QUrl path, QString method, QJsonObject json, Callback callback)
{
    QString host;
    auto url = QUrl(QString("https://%1").arg(host)).resolved(path);

    Data header;
    Request::call(context, url, method, header, json,  [=](QNetworkReply::NetworkError error, int statusCode, QVariant result) {
        callback(statusCode, result.toMap());
     });
}

void get(QObject* context, QUrl path, Callback callback)
{
    call(context, path, "GET", Data(), Files(), callback);
}

void post(QObject* context, QUrl path, Data body, Files files, Callback callback)
{
    call(context, path, "POST", body, files, callback);
}

void post(QObject* context, QUrl path, Data body, Callback callback)
{
    call(context, path, "POST", body, Files(), callback);
}

void post(QObject* context, QUrl path, Callback callback)
{
    call(context, path, "POST", Data(), Files(), callback);
}

void post(QObject* context, QUrl path, QJsonObject json, Callback callback)
{
    call(context, path, "POST", json, callback);
}

}
}