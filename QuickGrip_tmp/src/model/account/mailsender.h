#pragma once

#include <QObject>
#include <QString>

enum class MailSendResultCode
{
	Success,
	InvalidEmail,
	InvalidSerial,
	InvalidUuid,
	InvalidJson,
	InvalidLicense,
	RateLimited,
	AuthServerUnavailable,
	MailServiceError,
	InternalServerError,
	NetworkError,
	Timeout,
	InvalidResponse,
	UnknownError
};

struct MailSendResult
{
	MailSendResultCode code = MailSendResultCode::UnknownError;
	int httpStatus = 0;
	int networkError = 0;
	QString serverCode;
	QString requestId;
	QString messageId;

	bool isSuccess() const { return code == MailSendResultCode::Success; }
};

class MailSender : public QObject
{
	Q_OBJECT

public:
	explicit MailSender(QObject *parent = Q_NULLPTR);
	~MailSender() override;

	void sendSerialCodeMail(const QString& to, const QString& serialcode);

	bool sendEmail(const QString& from, const QString& to, const QString& subject, const QString& message);

	MailSendResult sendSerialCodeEmail(const QString& emailAddress, const QString& serial);
};
