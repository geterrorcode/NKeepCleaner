#pragma once


#include <QObject>
#include <Windows.h>
#include <QMap>
#include <QVector>
#include <QPointer>
#include <unordered_map>
#include <tuple>

#include "optimizdata.h"

#include "searchthread.h"
#include "deletethread.h"

#include "../../common/tool/utils.h"

#include "../../common/ux/singleton.h"

class FileOptimizer  : public QObject,
	public common::ux::Singleton<FileOptimizer>
{
	Q_OBJECT

public:
	FileOptimizer(QObject *parent = Q_NULLPTR);
	~FileOptimizer();

public:
	void getAllCheckFileCount();
	size_t getFileOptimizeSearchCount();

	
	void recycleBinClear();
	//void checkOptimizeFile();

	QString getFileTypeToString(FileOptimizeType type);
	FileOptimizeType getStringToFileType(QString text);
	QString getFileTypeToDesc(FileOptimizeType type);
	QString getFileTypeToOptimizeDesc(FileOptimizeType type);
	Qt::CheckState getFileTypeToCheck(FileOptimizeType type);


	QStringList getFileOptimizeCheckValue();
	//QStringList getFileOptimizeFailValue();
	std::tuple<size_t, size_t> getFileOptimizeFailValue();

	QString getFileOptimizeSize(float size);

	void pcoffRecycleBin();

	void updateOptimizeState(FileOptimizeType type, bool use);
	bool isOptimizeType(FileOptimizeType type);

	QMap<FileOptimizeType, QStringList>::iterator getFileSearchFirstIter() {
		return _search_paths.begin();
	}

	QMap<FileOptimizeType, QStringList>::iterator getFileSearchLastIter() {
		return _search_paths.end();
	}

	QVector<fileOptimizeData>::iterator getFileScanFirstIter() {
		return _checkfiles.begin();
	}

	QVector<fileOptimizeData>::iterator getFileScanLastIter() {
		return _checkfiles.end();
	}

	QVector<fileOptimizeData>::iterator getFileFailFirstIter() {
		return _failfiles.begin();
	}

	QVector<fileOptimizeData>::iterator getFileFailLastIter() {
		return _failfiles.end();
	}

	QString getRecyleBinSize() const { return utils::getFileSizeType(_recyclebin_info.first);}
	size_t getRecyleBinCount() const { return _recyclebin_info.second; }

	size_t getOptimizeFileCount();
private:
	size_t _allfilecount;
	QString _allCompleteFileCount;
	QString _allCompleteFileSize;

	QPointer<SearchThread> _searchthread;
	QPointer<DeleteThread> _deletethread;

	QMap<FileOptimizeType, QStringList> _search_paths;
	QVector<fileOptimizeData> _checkfiles;
	QVector<fileOptimizeData> _failfiles;

	QVector<FileOptimizeType> _optimizeType;

	std::unordered_map<FileOptimizeType, size_t> _optimize_count;
	//QPair<QString/*size*/, size_t/*file count*/> _recyclebin_info;
	QPair<size_t/*size*/, size_t/*file count*/> _recyclebin_info;

	defFiletypeInfo _filetypeinfo;
	
	size_t _delcount;
	size_t _alldelsize;

	//실패 기록
	size_t _delfailcount;
	size_t _alldelfailsize;
	
	void optimizeRecycleBin();

	void updateOptimizeFileCount(const FileOptimizeType& type, size_t count);
	
signals:
	
	void sendfilecheckMsg(QString);
	void getFileSearchCount(size_t count);
	void filechecked(FileOptimizeType);
	void fileCheckComplete(int count);

	void sendfileoptimized(fileOptimizeData data, bool delsuccess);
	void fileOptimizeComplete(size_t allcount, size_t success_count, QString filesize);

	void pcOffOptiStart();

public slots:
	void getCheckfileCount();
	void checkFiles();

	void checkFile(const FileOptimizeType type, const QString path, const size_t size);
	void checkFinish();

	void optimizeFiles();
};
