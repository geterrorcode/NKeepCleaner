#pragma once

#include <QObject>

class DownLoader : public QObject
{
	Q_OBJECT

public:
	DownLoader(QObject *parent = nullptr);
	~DownLoader();

public:
	void update();

public:
	bool getDownVersionInfo();
	int getVersion();
	bool deleteOldFile();


signals:
	void siganlProgress(qint64 recv, qint64 total);
	void signalComplate();
	void signalError(int errorType, int errorCode, QString errorMsg, QString FileName);


private:
	QByteArray _versionData;

};
