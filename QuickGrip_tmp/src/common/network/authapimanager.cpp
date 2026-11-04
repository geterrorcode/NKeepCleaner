#include "authapimanager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>


AuthApiManager::AuthApiManager(QString url, QString json, QObject *parent)
	: QObject(parent), _url(""), _json("")
{

	_url = url;
    _json = json;

	
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postData; // Prepare your POST data here
    postData = json.toUtf8();


    QNetworkReply* reply = manager.post(request, postData);

    connect(reply, &QNetworkReply::errorOccurred, this, &AuthApiManager::errorControl);


    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Error:" << reply->errorString();
            return;
        }

        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

        if (jsonDoc.isNull()) {
            qWarning() << "Invalid JSON";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        // Process jsonObj...


        reply->deleteLater();
        });
}

AuthApiManager::~AuthApiManager()
{}

void AuthApiManager::startauth()
{
    //QNetworkAccessManager manager;
    //QNetworkRequest request(_url);
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //QByteArray postData; // Prepare your POST data here
    //postData = _json.toUtf8();


    //QNetworkReply* reply = manager.post(request, postData);

    //connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
    //    this, &AuthApiManager::errorControl);


    //QObject::connect(reply, &QNetworkReply::finished, [reply]() {
    //    if (reply->error() != QNetworkReply::NoError) {
    //        qWarning() << "Error:" << reply->errorString();
    //        return;
    //    }

    //    QByteArray response = reply->readAll();
    //    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

    //    if (jsonDoc.isNull()) {
    //        qWarning() << "Invalid JSON";
    //        return;
    //    }

    //    QJsonObject jsonObj = jsonDoc.object();

    //    // Process jsonObj...


    //    reply->deleteLater();
    //    });
}

void AuthApiManager::errorControl()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        // success
        qDebug() << "Success" << reply->readAll();  // print all data
    }
    else {
        // failure
        qDebug() << "Failure" << reply->errorString();
    }

    reply->deleteLater();
}