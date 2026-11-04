#pragma once

#include <QObject>

class SvcInstaller  : public QObject
{
	Q_OBJECT

public:
	SvcInstaller(QObject *parent = Q_NULLPTR);
	~SvcInstaller();

	bool isServiceInstalled(const QString& svcname);
	bool isServiceRunning(const QString &svcname);
	bool installService(const QString& svcname, const QString& scvpath);
	bool startService(const QString& svcname);
	bool deleteService(const QString& svcname);
};
