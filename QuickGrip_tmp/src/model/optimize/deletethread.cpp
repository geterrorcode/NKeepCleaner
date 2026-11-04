#include "deletethread.h"

#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QDirIterator>

#include "../../common/tool/utils.h"

DeleteThread::DeleteThread(QObject *parent)
	: QThread(parent) , _opttype(OptimizeType::none)
{
	_fileinfos.clear();
	_reginfos.clear();
	_optimizeUseState.clear();
}

DeleteThread::~DeleteThread()
{
	if (isRunning()) {
		quit();
		wait(); // 스레드가 종료될 때까지 대기
	}
}


void DeleteThread::run()
{
	switch (_opttype)
	{
	case OptimizeType::FileType:
		deleteFiles();
		break;
	case OptimizeType::IEFileType:
		deleteIEFiles();
		break;
	case OptimizeType::RegType:
		deleteRegs();
		break;
	case OptimizeType::none:
		break;
	default:
		break;
	}
}

void DeleteThread::setDelFileInfo(const QVector<fileOptimizeData> fileinfo, const QVector<FileOptimizeType> optimizeTypeState, bool iefile)
{
	if (iefile)
		_opttype = OptimizeType::IEFileType;
	else  _opttype = OptimizeType::FileType;
	
	_fileinfos = fileinfo;
	_fileinfos.detach();

	_optimizeUseState = optimizeTypeState;
	_optimizeUseState.detach();
}

void DeleteThread::setDelRegInfo(const QMap<int, regOptimizeData> reginfo)
{
	_opttype = OptimizeType::RegType;
	_reginfos = reginfo;
	_reginfos.detach();
}

void DeleteThread::deleteFiles()
{
	bool optimizeFlag = false;
	bool subFoldercheck = false;
	FileOptimizeType temptype = FileOptimizeType::none;
	for (auto data : _fileinfos)
	{
		if (temptype != data.type) { //신규 type으로 진입
			temptype = data.type;

			subFoldercheck = true;
			optimizeFlag = isOptimizeType(temptype);
			if(optimizeFlag)
				deleteSubFolders(temptype);
		}

		if (!optimizeFlag)
			continue;

		QFile file(data.filepath);
		if (!file.exists()) {
			//완료로 전송
			emit snedDeleteFile(data, false);
			continue;
		}

		//file delete
		emit snedDeleteFile(data, file.remove());

	}
	//전체 완료 전송
	emit snedDeleteFileComplete();
}

void DeleteThread::deleteSubFolders(FileOptimizeType type)
{
	QStringList paths = getTargetFolder(type);
	for (auto path : paths)
	{
		QDirIterator it(path, QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
		while (it.hasNext()) {

			//it.next();
			if (it.next().isEmpty())
				continue;

			QFileInfo fileInfo = it.fileInfo();
			QString path = fileInfo.absoluteFilePath();
			//qDebug() << "path : " << path << ", Size : " << fileInfo.size();
			if (fileInfo.size() == 0)
				QDir().rmdir(fileInfo.absoluteFilePath());
		}
	}
}

QStringList DeleteThread::getTargetFolder(FileOptimizeType type)
{
	QStringList paths;
	paths.clear();

	QString appLocalPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
	QString appPath = utils::getUserAppPath();

	switch (type)
	{
	case FileOptimizeType::FileRecent:
		paths.append(QString("%1Roaming/Microsoft/Windows/Recent/").arg(appPath));
		break;
	case FileOptimizeType::FileWinLog:
		paths.append(QString("%1System32/winevt/Logs/").arg(utils::getWindowsPath()));
		break;
	case FileOptimizeType::FileWinTemp:
		paths.append(QString("%1/Microsoft/Windows/Temporary Internet Files/").arg(appLocalPath));
		break;
	case FileOptimizeType::FileAppCache:
		paths.append(QString("%1/Temp/").arg(appLocalPath));
		paths.append(QString("%1/cache/").arg(appLocalPath));
		break;
	case FileOptimizeType::FileUpdateTemp:
		paths.append(QString("%1softwaredistribution/download/").arg(utils::getWindowsPath()));
		break;
	case FileOptimizeType::FileWinCache:
		paths.append(QString("%1Temp/").arg(utils::getWindowsPath()));
		break;
	case FileOptimizeType::FileWinPrefetch:
		paths.append(QString("%1Prefetch/").arg(utils::getWindowsPath()));
		break;
	case FileOptimizeType::FileWinTeamsCache:
		paths.append(QString("%1Roaming/Microsoft/teams/application cache/cache/").arg(appPath));
		paths.append(QString("%1Roaming/Microsoft/teams/blob_storage/").arg(appPath));
		paths.append(QString("%1Roaming/Microsoft/teams/Cache/").arg(appPath));
		paths.append(QString("%1Roaming/Microsoft/teams/GPUcache/").arg(appPath));
		paths.append(QString("%1Roaming/Microsoft/teams/IndexedDB/").arg(appPath));
		paths.append(QString("%1Roaming/Microsoft/teams/Local Storage/").arg(appPath));
		paths.append(QString("%1Roaming/Microsoft/teams/tmp/").arg(appPath));
		break;
	case FileOptimizeType::FileWinDump:
		paths.append(QString("%1Minidump/").arg(utils::getWindowsPath()));
		break;
	case FileOptimizeType::EdgeCache:
		break;
	case FileOptimizeType::EdgeCodeCache:
		break;
	case FileOptimizeType::IECache:
		break;
	case FileOptimizeType::none:
		break;
	default:
		break;
	}

	return paths;
}

void DeleteThread::deleteIEFiles()
{
	bool optimizeFlag = false;
	FileOptimizeType temptype = FileOptimizeType::none;

	for (auto data : _fileinfos)
	{
		if (temptype != data.type) { //신규 type으로 진입
			temptype = data.type;

			optimizeFlag = isOptimizeType(temptype);
		}

		QFile file(data.filepath);
		if (!file.exists()) {
			//완료로 전송
			emit snedDeleteIEFile(data, false);
			continue;
		}

		if (!optimizeFlag)
			continue;

		//완료 전송
		emit snedDeleteIEFile(data, file.remove());
	}
	//전체 완료 전송
	emit snedDeleteIEFileComplete();
}


void DeleteThread::deleteRegs()
{
	for (auto data : _reginfos){

		QSettings reg(data.regpath, QSettings::NativeFormat);

		//qDebug() << data.regpath << ", " << data.keyname;

		if(data.keyname.isEmpty())
			reg.remove(".");
		else
			reg.remove(data.keyname);


		bool success = false;
		if (reg.childGroups().contains(data.keyname, Qt::CaseInsensitive))
			success = false;
		else
			success = true;

		emit snedDeleteReg(data, success);
	}

	//this->terminate();

	//전체 완료 전송
	emit snedDeleteRegComplete();
}

bool DeleteThread::isOptimizeType(FileOptimizeType type) const
{
	auto it = std::find(_optimizeUseState.begin(), _optimizeUseState.end(), type);

	if (it != _optimizeUseState.end())
		return true;
	return false;
}