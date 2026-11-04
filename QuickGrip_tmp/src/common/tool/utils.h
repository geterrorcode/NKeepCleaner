#pragma once

#include <QObject>
#include <QFileIconProvider>

namespace utils {

	QIcon getIconFromFile(const QString filePath);
	QString getFileSizeType(float size);
	qint64 getFolderAllSize(QString folderPath);
	QString getFolderSizeString(QString folderPath);

	std::string getPlatform();
	std::string getOSVersion();


	bool networkControl(bool connect);
	size_t getAllFileCount(const QString folderpath);
	size_t getAllFileCountSubfolder(const QString folderpath);
	size_t getAllFileCountSubfolder_exp(const QString folderpath, const QStringList exeptions);
	QString getRecycleBinPath();
	std::wstring getKnownFolderPath();

	QString getFileMakeDate(const QString filePath);
	QString getAvaliableHardSpace();

	QString getUserAppPath();
	QString getWindowsPath();

	QString toRichText(const QString& txt, const int lineHeight);
	void setStartup(const QString name, const QString path, bool use);
	void setTaskScheduler(const QString name, const QString path, bool use);

	void pcShutDown();

	QString aesEncryptor(QString str);
	QString aesDecryptor(QString str);

	QString getSystemUUID();
	
}

