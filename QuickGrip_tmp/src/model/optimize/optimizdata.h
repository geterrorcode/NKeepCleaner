#pragma once

#include <Windows.h>
#include <QObject>
#include <QMap>
#include <QVector>

enum class OptimizeLevel
{
	Good = 0,
	Nomal,
	Bad
};


enum class OptimizeType
{

	FileType = 0,
	IEFileType,
	RegType,
	none
};

enum class FileOptimizeType
{
	//system cache
	FileRecent = 0,
	FileWinLog,
	FileWinTemp,
	FileAppCache,
	FileUpdateTemp,
	FileWinCache,
	FileWinPrefetch,
	FileWinTeamsCache,
	FileWinDump,
	FileWinRecycleBin,
	//browser cache
	EdgeCache,
	EdgeCookie,
	EdgeCodeCache,
	IECache,
	none
};


enum class IEOptimizeType
{
	EdgeCache = 0,
	EdgeCodeCache,
	IECache,
	none
};

enum class RegOptimizeType
{
	RegWrongAppPath = 0,
	RegWrongActiveX,
	RegWrongModuleUsage,
	RegWrongMRU,
	RegWrongStartup,
	RegWrongUninstall,
	RegWrongFirewallRules,
	none
};


struct fileOptimizeTypeeData {
	bool use;
	QString typestring;
	//QString filesize;
	size_t filesize;
	size_t fileCount;

	fileOptimizeTypeeData()
	{
		use = false;
		typestring = "Windowsキャッシュファイル"; //윈도우 캐시 파일
		//filesize = "";
		filesize = 0;
		fileCount = 0;
	}
};
typedef QMap<FileOptimizeType, fileOptimizeTypeeData> defFiletypeInfo;


struct fileOptimizeData {
	QString filepath;
	//QString filesize;
	size_t filesize;
	FileOptimizeType type;

	fileOptimizeData()
	{
		filepath = ""; //default
		filesize = 0;
		type = FileOptimizeType::FileWinCache;
	}
};

struct regOptimizeData {

	QString regpath;
	QString keyname;
	QString regvalue;
	RegOptimizeType optimizetype;

	regOptimizeData()
	{
		regpath = "";
		keyname = "";
		regvalue = "";
	}

};