#include "loader.h"
#include "networkmanager.h"
#include "replytimeout.h"


Loader::Loader(QNetworkReply* reply, QObject* parent)
    : m_reply(reply), QObject(parent)
{
    
    // 타임 아웃 설정
    //ReplyTimeout::set(m_reply, 500);

    connect(reply, &QNetworkReply::finished, this, [=] () {
        if (reply->error() == QNetworkReply::NoError) {
            QVariant fromCache = reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
           
            emit completed(reply->readAll());
        }
    });

    
    connect(reply, &QNetworkReply::finished, [=] () {
        reply->deleteLater();
    });

    connect(reply, &QNetworkReply::downloadProgress, this, [=] (qint64 bytesReceived, qint64 bytesTotal) {
        if (reply->error() == QNetworkReply::NoError) {
            emit progress(bytesReceived, bytesTotal);
            
        }
    });

    
    //connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    //connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, [this, reply] (QNetworkReply::NetworkError code) {
    connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, [this, reply](QNetworkReply::NetworkError code) {
        auto errorString = reply->errorString();
        emit error(code, errorString);
    });
}

Loader::Loader(QUrl url, QObject* parent)
    : Loader(NetworkManager::inst().get(QNetworkRequest(url)), parent){}


Loader::Loader(QNetworkRequest request, QObject* parent)
    : Loader(NetworkManager::inst().get(request), parent){}


void error(QNetworkReply::NetworkError code) {
    //auto errorString = reply->errorString();
    //emit error(code, errorString);

    qDebug() << "QNetworkReply::NetworkError " << code << "received";
}

Loader::~Loader(){}

