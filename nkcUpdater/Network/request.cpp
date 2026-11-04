#include "request.h"

#include <QHttpMultiPart>
#include <QBuffer>
#include <QJsonDocument>

//#include <Qux/include/utility.h>
#include "../QUX/include/utility/qvalue.h"

#include "networkmanager.h"
#include "loader.h"

namespace Request {

void call(QObject* context, QUrl url, QString method, Data headers, Data body, Files files, Callback callback)
{
    QNetworkRequest request(url);
    for (auto key : headers.keys()) {
        request.setRawHeader(qPrintable(key), qPrintable(headers[key]));
    }

    QNetworkReply* reply = nullptr;
    if (!body.isEmpty() || !files.isEmpty()) {
        auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        for (auto key : body.keys()) {
            QHttpPart part;
            part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"").arg(key).toUtf8());
            part.setBody(body[key].toUtf8());
            multiPart->append(part);
        }

        for (auto key : files.keys()) {
            auto file = files[key];
            QHttpPart part;
            part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(file.type));
            part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"; filename=\"%2\"").arg(key, file.name)));

            auto bytes = Qux::createQValue(file.bytes, multiPart);
            auto buffer = new QBuffer(bytes, multiPart);
            buffer->open(QIODevice::ReadOnly);
            part.setBodyDevice(buffer);

            multiPart->append(part);
        }

        reply = NetworkManager::inst().sendCustomRequest(request, qPrintable(method), multiPart);
        multiPart->setParent(reply);
    }
    else {
        reply = NetworkManager::inst().sendCustomRequest(request, qPrintable(method));
    }

    auto loader = new Loader(reply, context);
    QObject::connect(loader, &Loader::completed, [=] (QByteArray bytes) {
        callback(QNetworkReply::NoError, reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), QJsonDocument::fromJson(bytes).toVariant());
        loader->deleteLater();
    });
    QObject::connect(loader, &Loader::error, [=] (QNetworkReply::NetworkError code) {
        callback(code, 0, QVariant());
        loader->deleteLater();
    });
}

// Json 
void call(QObject* context, QUrl url, QString method, Data headers, QJsonObject json, Callback callback)
{
    QNetworkRequest request(url);
    for (auto key : headers.keys()) {
        request.setRawHeader(qPrintable(key), qPrintable(headers[key]));
    }

    QJsonDocument doc;
    doc.setObject(json);
    QByteArray bytes = doc.toJson(QJsonDocument::Compact);
    QString strFromObj = QJsonDocument(json).toJson(QJsonDocument::Compact).toStdString().c_str();

    QNetworkReply* reply = nullptr;

    reply = NetworkManager::inst().sendCustomRequest(request, qPrintable(method), bytes);


    auto loader = new Loader(reply, context);
    QObject::connect(loader, &Loader::completed, [=](QByteArray bytes) {
        callback(QNetworkReply::NoError, reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), QJsonDocument::fromJson(bytes).toVariant());
        loader->deleteLater();
        });
    QObject::connect(loader, &Loader::error, [=](QNetworkReply::NetworkError code) {
        callback(code, 0, QVariant());
        loader->deleteLater();
        });
}



}