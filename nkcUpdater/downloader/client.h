#pragma once

#include <QObject>
#include <QScopedPointer>

#include "../nkcupdater.h"

#include "Logger.h"
#include "downloader.h"


class Client : public QObject
{
	Q_OBJECT

public :
	enum StartMode {
		ReStartUpdate = 1
	};

public:
	struct Impl
	{
		nkcUpdater* mainContainer;

		Impl()
			: mainContainer(nullptr)
		{}
	};

static Client& Inst();

public:
	Client(QObject *parent = nullptr);
	~Client();

public:
	int getLauncherVersion();
	void setLauncherVersion(int version);
	int getLauncherExeVersion(const QString& exeFilePath);
public:
	bool startLauncher();
	
	void update();

	DownLoader* getDownLoader() { return _download.data(); };

public:
	bool isUpdate(const QString& arg);
	
	void Initialize();
	void createWidget(QWidget* parentWidget = nullptr);
	QWidget* GetMainWidget() const;

	void connectLocalServer();
	

private:
	Impl* m_impl;
	QScopedPointer<DownLoader> _download;

	bool isStartupOption();

private:
	std::unique_ptr<Log::Logger> _gLogger;
};
