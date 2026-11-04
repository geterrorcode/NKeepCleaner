#include "loadsync.h"

#include <QjsonObject>
#include <QJsonDocument>
#include <QJsonValue>

#include "loader.h"

#include "../ux/waitforsignal.h"

QByteArray loadSync(QUrl url)
{
    Loader loader(url, nullptr);
    QByteArray result;
    QObject::connect(&loader, &Loader::completed, [&result] (QByteArray bytes) {
        result = bytes;
    });

    QObject::connect(&loader, &Loader::error, [&result](QNetworkReply::NetworkError errorCode, QString errorString) {

        QJsonObject rootObj;// = jsonDocument.object();

        QJsonObject  resultObj;
        resultObj.insert("errorcode", QJsonValue::fromVariant(errorCode));
        resultObj.insert("errormsg", QJsonValue::fromVariant(""));


        rootObj.insert("result", resultObj);
        result = QJsonDocument(rootObj).toJson();

    });
    

    common::ux::waitForSignal(&loader, &Loader::completed, &Loader::error);

    return result;
}

