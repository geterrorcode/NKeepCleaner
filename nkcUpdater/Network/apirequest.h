#pragma once

#include "request.h"

namespace Request {
namespace API {

typedef std::function<void (int statusCode, QVariantMap result)> Callback;

void call(QObject* context, QUrl path, QString method, Data body, Files files, Callback callback);
void get(QObject* context, QUrl path, Callback callback);
void post(QObject* context, QUrl path, Data body, Files files, Callback callback);
void post(QObject* context, QUrl path, Data body, Callback callback);
void post(QObject* context, QUrl path, Callback callback);
void post(QObject* context, QUrl path, QJsonObject json, Callback callback);

}
}
