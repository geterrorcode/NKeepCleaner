#pragma once

#include <QObject>

#include <Qux/include/singleton.h>

class NLog : public QObject
		, public Qux::Singleton<NLog>
{
	
public:
	void initialize();
	void send(int subCategory, const QJsonObject& dynamicJsonData);

	const QString& getMacAddress();
	const QString& getOS();
	const QString& getRAM();
	const QString& getCPU();
	const QString& getResolution();
	const QString& getDefaultBrowser();
	const QString& getIpAddress();

public :
	enum SubCategoryType
	{
		LauncherUpdate = 51,
		LaunhcerUpdateResult = 52,
		LauncherStart = 53,
		LauncherLink = 54,
		GameInstall = 55,
		GameInstallResult = 56,
		GameStart = 59,
		GameUninstall = 58,
		GameStartResult = 57,

		
	};

	enum linkType
	{
		LinkHome = 0,
		LinkCafe = 1,
		LinkYoutube = 2,
		LinkInstagram = 3,
		LinkFacebook = 4,
		LinkTwitter = 5,
		LinkAdvertising = 6,

	};

	enum PlayStartErrorType
	{
		PlayNone = 0,
		PlayAlreadyUserCancel = 1, 
		PlayTerminatePrevGame = 2,
		PlayPrereqSetup = 3,
		PlayRunProcessError = 4,
		
	};
private:
	friend NLog& Qux::Singleton<NLog>::inst();
	NLog();
	


private:
	QJsonObject CommonData(const int subCategory, const QJsonObject& dynamicJsonData);
	QString findMacAddress();
	
	std::tuple<QString, QString> getNetwork();

	void getSystemInfo();
	

	QString getDefaultBrowserInfo(const int ostype);

public: 
	QString _macAddress;
	QString _os;
	QString _cpu;
	QString _ram;
	QString _resolution;
	QString _defaultBrowser;
	QString _ipAddress;
};


inline const NLog& nlog() { return NLog::cinst(); }
