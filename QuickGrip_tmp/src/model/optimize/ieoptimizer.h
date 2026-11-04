#pragma once

#include <QObject>
#include <Windows.h>
#include <QMap>
#include <QVector>
#include <QPointer>
#include <tuple>

#include "optimizdata.h"
#include "searchthread.h"
#include "deletethread.h"

#include "../../common/ux/singleton.h"

class IEOptimizer  : public QObject,
	public common::ux::Singleton<IEOptimizer>
{
	Q_OBJECT

public:
	IEOptimizer(QObject *parent = Q_NULLPTR);
	~IEOptimizer();

	void getAllCheckIECount();
	size_t getIEFileOptimizeSearchCount();
	
	QString getIEFileTypeToString(FileOptimizeType type);
	QString getIEFileTypeToDesc(FileOptimizeType type);
	QString getIEFileTypeToOptimizeDesc(FileOptimizeType type);
	Qt::CheckState getIEFileTypeToCheck(FileOptimizeType type);

	QStringList getIEFileOptimizeCheckValue();
	std::tuple<size_t, size_t> getIEFileOptimizeFailValue();

	void updateIEOptimizeState(FileOptimizeType type, bool use);
	bool isIEOptimizeType(FileOptimizeType type);

	QVector<fileOptimizeData>::iterator getIEScanFirstIter() {
		return _checkfiles.begin();
	}

	QVector<fileOptimizeData>::iterator getIEScanLastIter() {
		return _checkfiles.end();
	}

	QVector<fileOptimizeData>::iterator getIEFailFirstIter() {
		return _failfiles.begin();
	}

	QVector<fileOptimizeData>::iterator getIEFailLastIter() {
		return _failfiles.end();
	}

	size_t getOptimizeIeFileCount();

private:
	size_t _alliecount;
	QString _allCompleteieCount;
	QString _allCompleteieSize;


	QPointer<SearchThread> _searchthread;
	QPointer<DeleteThread> _deletethread;

	QMap<FileOptimizeType, QStringList> _search_paths;
	QVector<fileOptimizeData> _checkfiles;
	QVector<fileOptimizeData> _failfiles;

	QVector<FileOptimizeType> _optimizeIEType;

	defFiletypeInfo _filetypeinfo;

	size_t _delcount;
	size_t _alldelsize;

	//실패 기록
	size_t _delfailcount;
	size_t _alldelfailsize;


	//size_t getEdgeCache();
	//size_t getIECache();
	QString getIEFileOptimizeSize(size_t sizse);
	
signals:
	void sendIEcheckMsg(QString);
	void getIEFileSearchCount(size_t count);
	void ieFilechecked(FileOptimizeType);
	void ieFileCheckComplete(int count);

	void sendIEoptimized(fileOptimizeData data, bool delsuccess);
	void ieOptimizeComplete(size_t allcount, size_t success_count, QString filesize);

public slots:
	void getCheckIEfileCount();
	void checkIEFiles();

	void optimizeIEFiles();
};
