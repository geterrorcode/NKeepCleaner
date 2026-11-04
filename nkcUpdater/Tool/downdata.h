#pragma once

#include <QObject>
#include <QVariant>

class DownData : public QObject
{
	Q_OBJECT

public:
	DownData(QThread* _thread);

public:
	void setDownInfo(const QList<QVariantMap>& versionList, const QString& workDir);
	bool isDownFiles();
	int	getDownFileSize();

public slots:
	void slotFindDownList();


signals:
	void signalComplateOneFile(QString endFilePath, QString orgFilePath);
	void signalProgress(qint64 bytesReceived, qint64 bytesTotal);
	void signalError(int errorCode, QString errorMsg, QString errorFile);
	void signalComplate(int downCount);


private:
	void getDownLoadList(const QList<QVariantMap>& versions);
	

public:
	struct downList {
		qint64 size;
		QString orgFilePath;
		QString downFilePath;
		QString url;
	};

	QList<downList> _downList;
	QString _workDir;
	

};
