#include "mailsender.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QUrl>

#include <Logger.h>
#include <common/tool/utils.h>
#include <model/configdata/configdata.h>

namespace
{
const QUrl kSerialBackupApiUrl(
    QStringLiteral("https://an6817m9b3.execute-api.ap-northeast-1.amazonaws.com/serial-backup"));
constexpr int kRequestTimeoutMs = 15000;

QString responseForLog(QByteArray response)
{
    QString text = QString::fromUtf8(response);
    text.replace("\r\n", " | ");
    text.replace('\n', " | ");
    return text.left(1000);
}

MailSendResultCode resultCodeFromServerCode(const QString& code)
{
    if (code == QStringLiteral("INVALID_EMAIL"))
        return MailSendResultCode::InvalidEmail;
    if (code == QStringLiteral("INVALID_SERIAL"))
        return MailSendResultCode::InvalidSerial;
    if (code == QStringLiteral("INVALID_UUID"))
        return MailSendResultCode::InvalidUuid;
    if (code == QStringLiteral("INVALID_JSON"))
        return MailSendResultCode::InvalidJson;
    if (code == QStringLiteral("INVALID_LICENSE"))
        return MailSendResultCode::InvalidLicense;
    if (code == QStringLiteral("RATE_LIMITED"))
        return MailSendResultCode::RateLimited;
    if (code == QStringLiteral("AUTH_SERVER_UNAVAILABLE"))
        return MailSendResultCode::AuthServerUnavailable;
    if (code == QStringLiteral("MAIL_SERVICE_ERROR"))
        return MailSendResultCode::MailServiceError;
    if (code == QStringLiteral("INTERNAL_ERROR"))
        return MailSendResultCode::InternalServerError;

    return MailSendResultCode::UnknownError;
}
}

MailSender::MailSender(QObject* parent)
    : QObject(parent)
{
}

MailSender::~MailSender() = default;

void MailSender::sendSerialCodeMail(const QString& to, const QString& serialcode)
{
    if (!sendSerialCodeEmail(to, serialcode).isSuccess())
        PLOG_WARNING << "[MAIL_API_SEND_FAILED] legacyEntryPoint=true";
}

MailSendResult MailSender::sendSerialCodeEmail(
    const QString& emailAddress, const QString& serial)
{
    const QString email = emailAddress.trimmed();
    const QString serialKey = serial.trimmed();

    QString uuid = ConfigData::inst()->getUUID().trimmed();
    if (uuid.isEmpty()) {
        uuid = utils::getSystemUUID().trimmed();
        if (!uuid.isEmpty())
            ConfigData::inst()->setUUID(uuid);
    }

    if (email.isEmpty()) {
        PLOG_WARNING << "[MAIL_API_INVALID_REQUEST] reason=emailEmpty";
        return { MailSendResultCode::InvalidEmail };
    }

    if (serialKey.isEmpty()) {
        PLOG_WARNING << "[MAIL_API_INVALID_REQUEST] reason=serialEmpty";
        return { MailSendResultCode::InvalidSerial };
    }

    if (uuid.isEmpty()) {
        PLOG_WARNING << "[MAIL_API_INVALID_REQUEST] reason=uuidEmpty";
        return { MailSendResultCode::InvalidUuid };
    }

    QJsonObject payload;
    payload.insert(QStringLiteral("email"), email);
    payload.insert(QStringLiteral("serial"), serialKey);
    payload.insert(QStringLiteral("uuid"), uuid);

    QNetworkRequest request(kSerialBackupApiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    request.setRawHeader("Accept", "application/json");

    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.post(
        request, QJsonDocument(payload).toJson(QJsonDocument::Compact));

    QEventLoop eventLoop;
    QTimer timeoutTimer;
    bool timedOut = false;
    timeoutTimer.setSingleShot(true);

    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    QObject::connect(&timeoutTimer, &QTimer::timeout, &eventLoop, [&]() {
        timedOut = true;
        reply->abort();
        eventLoop.quit();
        });

    PLOG_INFO << "[MAIL_API_REQUEST_START] endpoint=serial-backup";
    timeoutTimer.start(kRequestTimeoutMs);
    eventLoop.exec();
    timeoutTimer.stop();

    const int httpStatus = reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QNetworkReply::NetworkError networkError = reply->error();
    const QString networkErrorText = reply->errorString();
    const QByteArray response = reply->readAll();
    reply->deleteLater();

    if (timedOut) {
        PLOG_WARNING << "[MAIL_API_TIMEOUT] timeoutMs=" << kRequestTimeoutMs;
        MailSendResult result;
        result.code = MailSendResultCode::Timeout;
        result.networkError = static_cast<int>(networkError);
        return result;
    }

    if (networkError != QNetworkReply::NoError && httpStatus == 0) {
        PLOG_WARNING << "[MAIL_API_NETWORK_ERROR] code="
            << static_cast<int>(networkError)
            << ", error=" << networkErrorText;
        MailSendResult result;
        result.code = MailSendResultCode::NetworkError;
        result.networkError = static_cast<int>(networkError);
        return result;
    }

    QJsonParseError parseError;
    const QJsonDocument responseDocument = QJsonDocument::fromJson(response, &parseError);
    if (parseError.error != QJsonParseError::NoError || !responseDocument.isObject()) {
        PLOG_WARNING << "[MAIL_API_INVALID_RESPONSE] status=" << httpStatus
            << ", parseError=" << parseError.errorString()
            << ", responseLength=" << response.size();

        MailSendResult result;
        result.code = (httpStatus == 429)
            ? MailSendResultCode::RateLimited
            : MailSendResultCode::InvalidResponse;
        result.httpStatus = httpStatus;
        result.networkError = static_cast<int>(networkError);
        return result;
    }

    const QJsonObject responseObject = responseDocument.object();
    const bool ok = responseObject.value(QStringLiteral("ok")).toBool(false);
    const QString serverCode = responseObject.value(QStringLiteral("code")).toString();
    const QString requestId = responseObject.value(QStringLiteral("requestId")).toString();
    const QString messageId = responseObject.value(QStringLiteral("messageId")).toString();

    MailSendResult result;
    result.httpStatus = httpStatus;
    result.networkError = static_cast<int>(networkError);
    result.serverCode = serverCode;
    result.requestId = requestId;
    result.messageId = messageId;

    if (httpStatus == 200 && ok) {
        result.code = MailSendResultCode::Success;
        PLOG_INFO << "[MAIL_API_SUCCESS] status=" << httpStatus
            << ", requestId=" << requestId
            << ", messageIdPresent=" << !messageId.isEmpty();
        return result;
    }

    result.code = resultCodeFromServerCode(serverCode);
    if (result.code == MailSendResultCode::UnknownError) {
        if (httpStatus == 429)
            result.code = MailSendResultCode::RateLimited;
        else if (httpStatus == 502)
            result.code = MailSendResultCode::MailServiceError;
        else if (httpStatus == 503)
            result.code = MailSendResultCode::AuthServerUnavailable;
        else if (httpStatus >= 500)
            result.code = MailSendResultCode::InternalServerError;
        else if (httpStatus == 200)
            result.code = MailSendResultCode::InvalidResponse;
    }

    PLOG_WARNING << "[MAIL_API_REJECTED] status=" << httpStatus
        << ", code=" << serverCode
        << ", requestId=" << requestId
        << ", networkCode=" << static_cast<int>(networkError)
        << ", response=" << responseForLog(response);
    return result;
}

bool MailSender::sendEmail(const QString& from, const QString& to,
    const QString& subject, const QString& message)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(subject);
    Q_UNUSED(message);
    PLOG_WARNING << "[MAIL_API_UNSUPPORTED_GENERIC_SEND]";
    return false;
}
