#include "webpagecaller.h"

WebPageCaller::WebPageCaller(QObject *parent)
	: QObject(parent)
{

	_manager = new QNetworkAccessManager(this);
	connect(_manager, &QNetworkAccessManager::finished, this, &WebPageCaller::onFinished);

}

WebPageCaller::~WebPageCaller()
{
}


void WebPageCaller::onFinished(QNetworkReply* reply) 
{

    QUrl url = reply->request().url(); // 요청 URL을 얻습니다.
    if (reply->error() == QNetworkReply::NoError) {
        QString data = reply->readAll();
        emit dataFetched(data, url); // 데이터와 요청 URL을 함께 전달합니다.
    }
    else {
        emit dataFetched("Error occurred: " + reply->errorString(), url);
    }
    reply->deleteLater();
}

void WebPageCaller::fetchData(const QUrl& url)
{
    QNetworkRequest request(url);
	_manager->get(request); // URL에 대한 GET 요청을 시작합니다.
}