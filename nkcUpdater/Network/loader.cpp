#include "loader.h"
#include "networkmanager.h"

#include <QMessageBox>

Loader::Loader(QNetworkReply* reply, QObject* parent)
    : m_reply(reply), QObject(parent)
{
 /*   QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    sslConfiguration.setProtocol(QSsl::TlsV1_2);
    m_reply->setSslConfiguration(sslConfiguration);*/

    connect(reply, &QNetworkReply::finished, this, [=] () {
        if (reply->error() == QNetworkReply::NoError) {

            QVariant fromCache = reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
            bool ischche = fromCache.toBool();
            

            qDebug() << "page from cache?" << ischche;
            
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

    
   /* connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, [this, reply] (QNetworkReply::NetworkError code) {
        emit error(code);
    });*/
}

Loader::Loader(QUrl url, QObject* parent)
    : Loader(NetworkManager::inst().get(QNetworkRequest(url)), parent)
{

}


Loader::Loader(QNetworkRequest request, QObject* parent)
    : Loader(NetworkManager::inst().get(request), parent)
{

}

Loader::~Loader()
{

}

