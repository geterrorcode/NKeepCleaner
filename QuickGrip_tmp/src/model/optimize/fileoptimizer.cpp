#include "fileoptimizer.h"

#include <Windows.h>
#include <QStandardPaths>
#include <QDir>
#include <QProcessEnvironment>
#include <QDirIterator>
#include <QThread>
#include <string>
#include <iostream>
#include <filesystem>
#include <QDebug>

namespace fs = std::filesystem;


#include "../../controller/optimize/optimizecontroller.h"
#include "searchthread.h"




QMap<FileOptimizeType, QString> _fileTypetoName = {
	{FileOptimizeType::FileRecent, "Windows実行記録ファイル"}, //윈도우 실행 기록 파일
	{FileOptimizeType::FileWinLog, "Windowsログファイル"}, //윈도우 로그 파일
	{FileOptimizeType::FileWinTemp, "Windows一時ファイル"}, //윈도우 임시 파일
	{FileOptimizeType::FileAppCache, "プログラムキャッシュ(Cache)ファイル"}, //프로그램 캐시 파일
	{FileOptimizeType::FileUpdateTemp, "Windowsアップデート一時ファイル"}, //윈도우 업데이트 임시 파일
	{FileOptimizeType::FileWinCache, "Windowsキャッシュ(Cache)ファイル"}, //윈도우 캐시 파일
	{FileOptimizeType::FileWinPrefetch, "Windows事前ダウンロードキャッシュファイル"},//윈도우 사전 다운로드 캐시 파일
	{FileOptimizeType::FileWinTeamsCache, "Windows Teams キャッシュ(Cache)ファイル"}, //윈도우Teams 캐시 파일
	{FileOptimizeType::FileWinDump, "Windowsダンプファイル"},//윈도우 덤프 파일
	{FileOptimizeType::FileWinRecycleBin, "ゴミ箱"},//휴지통
	{FileOptimizeType::none, "未分類のファイル"} //미 분류된 파일
};

QMap<FileOptimizeType, QString> _fileTypetoOptimizeDescription = {
	{FileOptimizeType::FileRecent, "実行記録を削除すると個人情報セキュリティが強化されWindowsがより速く起動します"}, //실행 기록을 지우면 개인 정보 보호가 강화되고 Windows가 더 빠르게 기동합니다.
	{FileOptimizeType::FileWinLog, "不要なログファイルを消去しディスク容量を増やし、Windowsの性能を改善します"}, //불필요한 로그 파일을 제거하여 디스크 공간을 확보하고 Windows성능을 개선합니다.
	{FileOptimizeType::FileWinTemp, "不要なファイルを削除しWindowsを整理、ディスク容量を増やします"}, //임시 파일을 삭제함으로써 Windows를 정리하고 디스크 공간을 확보합니다.
	{FileOptimizeType::FileAppCache, "古いキャッシュを整理しプログラムの性能を最適化しWindowsを迅速に運用します"}, //오래된 캐시를 정리하여 프로그램 성능을 최적화하고 Windows을 신속하게 운영합니다..
	{FileOptimizeType::FileUpdateTemp, "アップデート時の不要なファイルを削除しディスク容量を増やして、アップデートプロセスを整理します"}, //업데이트 임시 파일을 제거하여 디스크 공간을 확보하고 업데이트 프로세스를 정리합니다.
	{FileOptimizeType::FileWinCache, "Windowsのキャッシュを整理しWindowsの反応速度を向上させます"}, //Windows캐시를 정리하여 Windows의 반응 속도를 향상시킵니다.
	{FileOptimizeType::FileWinPrefetch, "不要な事前ダウンロードファイルを消去することによりディスク容量の使用を抑え、Windowsを最適化します"},//불필요한 사전 다운로드 파일을 제거함으로써 디스크 공간을 절약하고 Windows를 최적화합니다.
	{FileOptimizeType::FileWinTeamsCache, "Teams キャッシュを整理しアプリの性能を向上させ、ストレージ容量を増やします"}, //Teams 캐시를 정리하여 앱 성능을 향상시키고 저장 공간을 확보합니다.
	{FileOptimizeType::FileWinDump, "古いダンプファイルを削除しディスク容量を増やして、Windowsの整理をサポートします"},//오래된 덤프 파일을 제거하여 디스크 공간을 확보하고 Windows의 정리를 도와줍니다.
	{FileOptimizeType::FileWinRecycleBin, "ゴミ箱を空けて不要なファイルを除去しディスク容量を増やします"},//휴지통을 비워서 불필요한 파일을 제거하고, 디스크 공간을 확보합니다.
	{FileOptimizeType::none, "未分類のファイル"} //미 분류된 파일
};


QMap<FileOptimizeType, QString> _fileTypetoDescription = {
	{FileOptimizeType::FileRecent, "ユーザーが実行したプログラムとファイルの記録です"}, //사용자가 실행한 프로그램과 파일의 기록입니다
	{FileOptimizeType::FileWinLog, "Windows運用中に発生したイベントとエラーの記録です"}, //시스템 운영 중 발생하는 이벤트와 오류의 기록입니다.
	{FileOptimizeType::FileWinTemp, "プログラム実行及びインストール中に一時的に生成されるファイルです"}, //프로그램 실행 및 설치 중 임시로 생성되는 파일입니다.
	{FileOptimizeType::FileAppCache, "プログラムの実行時に使用されたデータの一時的な保管場所です"}, //프로그램의 실행시 사용된 데이터의 임시 저장소입니다.
	{FileOptimizeType::FileUpdateTemp, "Windowsのアップデート中に生成される不要なファイルです"}, //윈도우 업데이트 임시 파일
	{FileOptimizeType::FileWinCache, "Windows動作時に生成される不要なデータファイルです"}, //Windows동작시 생성되는 임시 데이터 파일 입니다.
	{FileOptimizeType::FileWinPrefetch, "今後のアップデートのために事前にダウンロードされたファイルです"},//미래의 업데이트를 위해 미리 다운로드된 파일입니다.
	{FileOptimizeType::FileWinTeamsCache, "Microsoft Tearmsの使用中に生成される不要なファイルです"}, //Microsoft Teams 사용 중 생성되는 임시 파일입니다.
	{FileOptimizeType::FileWinDump, "システムエラーや干渉があった場合に生成される、エラーに関する情報ファイルです"},//시스템 오류나 출돌 시 생성되는 오류 관련 정보 파일 입니다.
	{FileOptimizeType::FileWinRecycleBin, "ユーザーが削除したファイルが一時的に保存されている場所です"},//사용자가 삭제한 파일이 임시로 저장되는 위치입니다.
	{FileOptimizeType::none, "未分類のファイル"} //미 분류된 파일
};


QMap<FileOptimizeType, bool> _fileTypetoCheck = {
	{FileOptimizeType::FileRecent, false}, 
	{FileOptimizeType::FileWinLog, true}, 
	{FileOptimizeType::FileWinTemp, true}, 
	{FileOptimizeType::FileAppCache, true},
	{FileOptimizeType::FileUpdateTemp, true}, 
	{FileOptimizeType::FileWinCache, true}, 
	{FileOptimizeType::FileWinPrefetch, true},
	{FileOptimizeType::FileWinTeamsCache, true}, 
	{FileOptimizeType::FileWinDump, false},
	{FileOptimizeType::FileWinRecycleBin, false},
	{FileOptimizeType::none, "未分類のファイル"} 
};



FileOptimizer::FileOptimizer(QObject *parent)
	: QObject(parent), _searchthread(Q_NULLPTR), _deletethread(Q_NULLPTR), _allfilecount(0), _allCompleteFileCount(""), _allCompleteFileSize("")
	, _delcount(0), _alldelsize(0), _delfailcount(0), _alldelfailsize(0)
{
	_search_paths.clear();
	_checkfiles.clear();
	_failfiles.clear();
	_filetypeinfo.clear();
	_optimize_count.clear();
	
	_optimizeType.clear();
}

FileOptimizer::~FileOptimizer()
{
	if (_searchthread.isNull() == false) {
	//if (!_searchthread) {
		delete _searchthread;
		_searchthread = Q_NULLPTR;
	}

	if (_deletethread.isNull() == false) {
		delete _deletethread;
		_deletethread = Q_NULLPTR;
	}
}

QString FileOptimizer::getFileTypeToString(FileOptimizeType type)
{
	if (_fileTypetoName.contains(type)) {
		return _fileTypetoName[type];
	}

	return "未分類のファイル";
}

FileOptimizeType FileOptimizer::getStringToFileType(QString text)
{
	for (const auto& key : _fileTypetoName.keys()) {
		if (_fileTypetoName[key] == text) {
			return (FileOptimizeType)key;
		}
	}

	return FileOptimizeType::none;
}

QString FileOptimizer::getFileTypeToDesc(FileOptimizeType type)
{
	if (_fileTypetoDescription.contains(type)) {
		return _fileTypetoDescription[type];
	}

	return "";
}

QString FileOptimizer::getFileTypeToOptimizeDesc(FileOptimizeType type)
{
	if (_fileTypetoOptimizeDescription.contains(type)) {
		return _fileTypetoOptimizeDescription[type];
	}
	return "未分類のファイル";
}

Qt::CheckState FileOptimizer::getFileTypeToCheck(FileOptimizeType type)
{
	if (_fileTypetoCheck.contains(type)) {
		if(_fileTypetoCheck[type])
			return Qt::Checked;
		else  return Qt::Unchecked;
	}

	return Qt::Checked;
}

QStringList FileOptimizer::getFileOptimizeCheckValue()
{
	QStringList ar;
	ar.push_back(_allCompleteFileCount);
	ar.push_back(_allCompleteFileSize);
	return ar;
}

//QStringList FileOptimizer::getFileOptimizeFailValue()
std::tuple<size_t, size_t> FileOptimizer::getFileOptimizeFailValue()
{
	return std::make_tuple(_alldelfailsize, _delfailcount);
	/*QStringList ar;
	ar.push_back(QString("%1").arg(_delfailcount));
	ar.push_back(getFileOptimizeSize(_alldelfailsize));
	return ar;*/
}

QString FileOptimizer::getFileOptimizeSize(float size)
{
	if (size == 0)
		return "0 MB";

	return utils::getFileSizeType(size);
}


//=====thread==========================
void filecheckthread() {
	forever{

		FileOptimizer::inst()->getFileOptimizeSearchCount();
		break;

		if (QThread::currentThread()->isInterruptionRequested()) {
			return;
		}
	}
}


void recyclebincleaerthread() {
	forever{

		FileOptimizer::inst()->recycleBinClear();
		break;

		if (QThread::currentThread()->isInterruptionRequested()) {
			return;
		}
	}
}
//========================================



void FileOptimizer::getAllCheckFileCount()
{
	auto thread = QThread::create(filecheckthread);
	thread->start();
	connect(thread, &QThread::finished, this, [=]() {
		emit getFileSearchCount(_allfilecount);

		});
}

size_t FileOptimizer::getFileOptimizeSearchCount()
{
	_allfilecount = 0;
	_search_paths.clear();
	_optimizeType.clear();

	size_t allFileCount = 0;

	QString windowsPath = utils::getWindowsPath();

	//C:/Users/acount/AppData/Local
	QString appLocalPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
	QString appPath = appLocalPath.left(appLocalPath.lastIndexOf("/") + 1);
	appLocalPath += "/";

	//윈도우 실행 기록 파일(\Roaming\Microsoft\Windows\Recent)
	QString recentPath = appPath + "Roaming/Microsoft/Windows/Recent/";
	{
		size_t size = utils::getAllFileCountSubfolder(recentPath);
		if (size > 0) {
			//_allfilecount += size;
			allFileCount += size;
			updateOptimizeFileCount(FileOptimizeType::FileRecent, size);

			QStringList paths;
			paths.append(recentPath);
			_search_paths.insert(FileOptimizeType::FileRecent, paths);

			if (getFileTypeToCheck(FileOptimizeType::FileRecent) == Qt::Checked) {
				updateOptimizeState(FileOptimizeType::FileRecent, true);
			}
		}
	}

	//윈도우 로그 파일(%SystemRoot%\System32\winevt\Logs) //삭제 안됨. 우선 빼자
	/*QString winlog = windowsPath + "System32/winevt/Logs/";
	{
		size_t size = utils::getAllFileCountSubfolder(winlog);
		if (size > 0) {
			_allfilecount += size;
			QStringList paths;
			paths.append(winlog);
			_search_paths.insert(FileOptimizeType::FileWinLog, paths);
		}
	}*/


	{//윈도우 임시 파일
		QStringList paths;
		size_t fileCount = 0;
		//(C:\Users\\AppData\Local\Microsoft\Windows\Temporary Internet Files)
		QString wintempPath = appLocalPath + "Microsoft/Windows/Temporary Internet Files/";
		{
			size_t size = utils::getAllFileCountSubfolder(wintempPath);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(wintempPath);


			}
		}

		//C:\Windows\Temp
		wintempPath = windowsPath + "Temp";
		{
			size_t size = utils::getAllFileCountSubfolder(wintempPath);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(wintempPath);
			}
		}

		if (paths.size() > 0) {
			_search_paths.insert(FileOptimizeType::FileWinTemp, paths);
			updateOptimizeFileCount(FileOptimizeType::FileWinTemp, fileCount);
			if (getFileTypeToCheck(FileOptimizeType::FileWinTemp) == Qt::Checked) {
				updateOptimizeState(FileOptimizeType::FileWinTemp, true);
			}
		}
	}


	//프로그램 실행 캐시 파일
	{
		QStringList paths;
		size_t fileCount = 0;
		//AppData\Local\Temp
		QString programCache = appLocalPath + "Temp/";
		{
			//size_t size = utils::getAllFileCountSubfolder(programCache);
			QStringList ar;
			ar.append(".etl");
			ar.append(".etlx");
			size_t size = utils::getAllFileCountSubfolder_exp(programCache, ar);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(programCache);
			}
		}

		//AppData/Local/cache
		programCache = appLocalPath + "cache/";
		{
			size_t size = utils::getAllFileCountSubfolder(programCache);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(programCache);
			}
		}

		if (paths.size() > 0) {
			_search_paths.insert(FileOptimizeType::FileAppCache, paths);
			updateOptimizeFileCount(FileOptimizeType::FileAppCache, fileCount);
			if (getFileTypeToCheck(FileOptimizeType::FileAppCache) == Qt::Checked) {
				updateOptimizeState(FileOptimizeType::FileAppCache, true);
			}
		}
	}


	//윈도우 업데이트 임시 저장 파일
	//c:\windows\softwaredistribution\download
	/*QString windowuptemp = windowsPath + "softwaredistribution/download/";
	{
		size_t size = utils::getAllFileCountSubfolder(windowuptemp);
		if (size > 0) {
			_allfilecount += size;
			QStringList paths;
			paths.append(windowuptemp);
			_search_paths.insert(FileOptimizeType::FileUpdateTemp, paths);
		}
	}*/


	//윈도우 동작 캐시 파일
	//C:\Windows\Temp
	//QString wintemp = windowsPath + "Temp/";
	//{
	//	size_t size = utils::getAllFileCountSubfolder(wintemp);
	//	if (size > 0) {
	//		_allfilecount += size;
	//		QStringList paths;
	//		paths.append(wintemp);
	//		_search_paths.insert(FileOptimizeType::FileWinCache, paths);
	//	}
	//}

	//윈도우 사전 다운로드 캐시 파일
	//C:\Windows\Prefetch
	QString winPrefetch = windowsPath + "Prefetch/";
	{
		size_t size = utils::getAllFileCountSubfolder(winPrefetch);
		if (size > 0) {
			//_allfilecount += size;
			allFileCount += size;
			updateOptimizeFileCount(FileOptimizeType::FileWinPrefetch, size);

			QStringList paths;
			paths.append(winPrefetch);
			_search_paths.insert(FileOptimizeType::FileWinPrefetch, paths);

			if (getFileTypeToCheck(FileOptimizeType::FileWinPrefetch) == Qt::Checked) {
				updateOptimizeState(FileOptimizeType::FileWinPrefetch, true);
			}
		}
	}

	{//윈도우 teams 캐시(//AppData\Roaming\Microsoft\Teams)

		QStringList paths;
		size_t fileCount = 0;
		QString winTeams = appPath + "Roaming/Microsoft/teams/application cache/cache/";
		{
			size_t size = utils::getAllFileCountSubfolder(winTeams);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(winTeams);
			}
		}

		winTeams = appPath + "Roaming/Microsoft/teams/blob_storage/";
		{
			size_t size = utils::getAllFileCountSubfolder(winTeams);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(winTeams);
			}
		}

		winTeams = appPath + "Roaming/Microsoft/teams/Cache/";
		{
			size_t size = utils::getAllFileCountSubfolder(winTeams);
			if (size > 0) {
				//_allfilecount += size;
				fileCount += size;
				paths.append(winTeams);
			}
		}

		winTeams = appPath + "Roaming/Microsoft/teams/GPUcache/";
		{
			size_t size = utils::getAllFileCountSubfolder(winTeams);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(winTeams);
			}
		}

		winTeams = appPath + "Roaming/Microsoft/teams/IndexedDB/";
		{
			size_t size = utils::getAllFileCountSubfolder(winTeams);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(winTeams);
			}
		}

		winTeams = appPath + "Roaming/Microsoft/teams/Local Storage/";
		{
			size_t size = utils::getAllFileCountSubfolder(winTeams);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(winTeams);
			}
		}

		winTeams = appPath + "Roaming/Microsoft/teams/tmp/";
		{
			size_t size = utils::getAllFileCountSubfolder(winTeams);
			if (size > 0) {
				//_allfilecount += size;
				allFileCount += size;
				fileCount += size;
				paths.append(winTeams);
			}
		}

		if (paths.size() > 0) {
			_search_paths.insert(FileOptimizeType::FileWinTeamsCache, paths);
			updateOptimizeFileCount(FileOptimizeType::FileWinTeamsCache, fileCount);
			if (getFileTypeToCheck(FileOptimizeType::FileWinTeamsCache) == Qt::Checked) {
				updateOptimizeState(FileOptimizeType::FileWinTeamsCache, true);
			}

		}
	}


	//윈도우 덤프파일
	QString wintump = windowsPath + "Minidump/";
	{
		size_t size = utils::getAllFileCount(wintump);
		if (size > 0) {
			//_allfilecount += size;
			allFileCount += size;
			QStringList paths;
			paths.append(wintump);
			_search_paths.insert(FileOptimizeType::FileWinDump, paths);
			updateOptimizeFileCount(FileOptimizeType::FileWinDump, size);
			if (getFileTypeToCheck(FileOptimizeType::FileWinDump) == Qt::Checked) {
				updateOptimizeState(FileOptimizeType::FileWinDump, true);
			}

		}
	}


	SHQUERYRBINFO BinInfo;
	::ZeroMemory(&BinInfo, sizeof(BinInfo));
	BinInfo.cbSize = sizeof(BinInfo);
	if (SHQueryRecycleBin(NULL, &BinInfo) == S_OK) {

		size_t size = BinInfo.i64NumItems;
		if (size > 0) {


			//_allfilecount += size;
			allFileCount += size;
			updateOptimizeFileCount(FileOptimizeType::FileWinRecycleBin, size);

			if (getFileTypeToCheck(FileOptimizeType::FileWinRecycleBin) == Qt::Checked) {
				updateOptimizeState(FileOptimizeType::FileWinRecycleBin, true);
			}

		}
			

		_recyclebin_info = qMakePair(BinInfo.i64Size, size);
	}

	//result = _allfilecount;
	_allfilecount = allFileCount;
	return allFileCount;
}

void FileOptimizer::optimizeRecycleBin()
{
	emit sendfilecheckMsg("ゴミ箱のファイルを整理しています");

	auto thread = QThread::create(recyclebincleaerthread);
	thread->start();
	connect(thread, &QThread::finished, this, [=]() {
		emit sendfilecheckMsg("ゴミ箱のファイルの整理が完了しました");

		_allCompleteFileCount = QString("%1").arg(_delcount);
		_allCompleteFileSize = getFileOptimizeSize(_alldelsize);

		qDebug() << "&DeleteThread::optimizeRecycleBin : " << _allCompleteFileSize << ", FileSize : " << _alldelsize;

		emit fileOptimizeComplete(_checkfiles.count(), _delcount, _allCompleteFileSize);
		});
}

void FileOptimizer::updateOptimizeFileCount(const FileOptimizeType& type, size_t count)
{
	auto it = _optimize_count.find(type);
	if (it != _optimize_count.end()) {
		// 이미 존재하면 size_t 값을 업데이트
		it->second = count;
	}
	else {
		// 존재하지 않으면 새로 추가
		_optimize_count[type] = count;
	}
}

size_t FileOptimizer::getOptimizeFileCount()
{
	size_t fileCount = 0;

	//_filetypeinfo
	for (auto it = _filetypeinfo.cbegin(); it != _filetypeinfo.cend(); ++it) {
		FileOptimizeType type = it.key();
		if (isOptimizeType(it.key()) == true) {
			fileCount += it.value().fileCount;
		}
	}

	return fileCount;
}

void FileOptimizer::pcoffRecycleBin()
{
	auto thread = QThread::create(recyclebincleaerthread);
	thread->start();
	connect(thread, &QThread::finished, this, [=]() {
		emit pcOffOptiStart();
		});
}


void FileOptimizer::recycleBinClear()
{//_checkfiles.count(), _delcount, _allCompleteFileSize
	SHQUERYRBINFO BinInfo;
	::ZeroMemory(&BinInfo, sizeof(BinInfo));
	BinInfo.cbSize = sizeof(BinInfo);
	if (SHQueryRecycleBin(NULL, &BinInfo) == S_OK) {

		FileOptimizeType type = FileOptimizeType::FileWinRecycleBin;
		fileOptimizeTypeeData data;
		data.typestring = getFileTypeToString(type);
		//data.filesize = BinInfo.i64NumItems;
		data.filesize = BinInfo.i64Size;
		data.fileCount = BinInfo.i64NumItems;
		_filetypeinfo.insert(type, data);

		_alldelsize += BinInfo.i64Size;
		_allCompleteFileSize = getFileOptimizeSize(_alldelsize);
		qDebug() << "recycleBinClear() : " << _allCompleteFileSize << ", FileCount : " << _alldelsize;

		_delcount += BinInfo.i64NumItems;
		qDebug() << "_delcount += BinInfo.i64NumItems,  " << _delcount;

		STARTUPINFOW si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		std::string arg = "cmd.exe /c \"echo Y|PowerShell.exe -NoProfile -Command Clear-RecycleBin\"";
		wchar_t wtext[MAX_PATH];
		mbstowcs(wtext, arg.c_str(), arg.length() + 1);//includes null
		LPWSTR ptr = wtext;


		if (CreateProcessW(NULL, ptr, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}
}

void FileOptimizer::getCheckfileCount()
{
	getAllCheckFileCount();
}


void FileOptimizer::updateOptimizeState(FileOptimizeType type, bool use)
{
	// QVector의 iterator를 사용하여 value의 존재 여부를 확인
	auto it = std::find(_optimizeType.begin(), _optimizeType.end(), type);

	if (use) {
		// use가 true이며 값이 존재하지 않으면 추가
		if (it == _optimizeType.end()) {
			_optimizeType.append(type);
		}
	}
	else {
		// use가 false이며 값이 존재하면 삭제
		if (it != _optimizeType.end()) {
			_optimizeType.erase(it);
		}
	}
}

bool FileOptimizer::isOptimizeType(FileOptimizeType type)
{
	auto it = std::find(_optimizeType.begin(), _optimizeType.end(), type);

	if (it != _optimizeType.end())
		return true;
	return false;
}

void FileOptimizer::checkFiles()
{
	if (_search_paths.size() == 0) {
		emit sendfilecheckMsg("ファイルのチェックが完了しました");//파일 검사 완료 되었습니다.
		return;
	}


	_filetypeinfo.clear();
	_checkfiles.clear();
	_failfiles.clear();
	if (_searchthread.isNull() == false) {
		_searchthread->terminate();
		delete _searchthread;
		_searchthread = Q_NULLPTR;
	}

	_searchthread = new SearchThread(_search_paths, false, this);
	
	//파일 한개 Scan 완료
	QObject::connect(_searchthread, &SearchThread::sendfileinfo, [=](FileOptimizeType type, QString path, size_t size) {

		fileOptimizeData data;
		data.filepath = path;
		data.type = type;
		//data.filesize = utils::getFileSizeType(size);
		data.filesize = size;
		_checkfiles.push_back(data);

		emit filechecked(type);	
		});


	//file type별 저장
	QObject::connect(_searchthread, &SearchThread::searchFinishedbyType, [=](FileOptimizeType type, size_t allcount, size_t allsize) {

		fileOptimizeTypeeData data;
		data.typestring = getFileTypeToString(type);
		//data.filesize = utils::getFileSizeType(allsize);
		data.filesize = allsize;
		data.fileCount = allcount;
		_filetypeinfo.insert(type, data);

		emit sendfilecheckMsg(getFileTypeToString(type) + " 完了");

		});

	//전체 완료
	QObject::connect(_searchthread, &SearchThread::searchFinished, [=](size_t allcount, size_t allsize) {

		SHQUERYRBINFO BinInfo;
		::ZeroMemory(&BinInfo, sizeof(BinInfo));
		BinInfo.cbSize = sizeof(BinInfo);
		if (SHQueryRecycleBin(NULL, &BinInfo) == S_OK) {

			FileOptimizeType type = FileOptimizeType::FileWinRecycleBin;
			fileOptimizeTypeeData data;
			data.typestring = getFileTypeToString(type);
			//data.filesize = BinInfo.i64NumItems;
			data.filesize = BinInfo.i64Size;
			data.fileCount = BinInfo.i64NumItems;
			_filetypeinfo.insert(type, data);

			allsize += BinInfo.i64Size;
			allcount += BinInfo.i64NumItems;
		}

		_allCompleteFileCount = QString("%1").arg(allcount);
		_allCompleteFileSize = utils::getFileSizeType(allsize);
		qDebug() << "&SearchThread::searchFinished : " << _allCompleteFileSize << ", FileCount : " << allsize;


		emit fileCheckComplete(allcount);

		});


	_searchthread->start();
}

void FileOptimizer::checkFile(FileOptimizeType type, QString path, size_t size)
{
	fileOptimizeData data;
	data.filepath = path;
	data.type = type;
	//data.filesize = utils::getFileSizeType(size);
	data.filesize = size;
	_checkfiles.push_back(data);

	emit filechecked(type);
}

void FileOptimizer::checkFinish()
{
	emit filechecked(FileOptimizeType::FileAppCache);
}

void FileOptimizer::optimizeFiles() //file 최적화 진행
{
	_delcount = 0;
	_alldelsize = 0;
	
	qDebug() << "_delcount = 0 ,  " << _delcount;

	_delfailcount = 0;
	_alldelfailsize = 0;
	

	if (_deletethread.isNull() == false) {
		delete _deletethread;
		_deletethread = Q_NULLPTR;
	}

	_deletethread = new DeleteThread(this);
	_deletethread->setDelFileInfo(_checkfiles, _optimizeType);


	//파일 한개 완료
	QObject::connect(_deletethread, &DeleteThread::snedDeleteFile, [=](fileOptimizeData data, bool delsuccess) {

		if (delsuccess) {
			_alldelsize += data.filesize;
			_delcount++;
			

			qDebug() << "_delcount++,  " << _delcount <<" ,File :" << data.filepath;

		}
		else {

			_alldelfailsize += data.filesize;
			_delfailcount++;
			qDebug() << "_delfailcount++,  " << _delfailcount;
			_failfiles.push_back(data);

			qDebug() << "Delete Error! : " << data.filepath;
		}
			

		emit sendfileoptimized(data, delsuccess);
		//_allfilecount
		//emit filechecked(type);
		});
	 

	QObject::connect(_deletethread, &DeleteThread::snedDeleteFileComplete, [=]() {

		_allCompleteFileCount = "";
		_allCompleteFileSize = "";


		//휴지통 최적화
		if(isOptimizeType(FileOptimizeType::FileWinRecycleBin))
			optimizeRecycleBin();
		else {
			_allCompleteFileCount = QString("%1").arg(_delcount);
			_allCompleteFileSize = getFileOptimizeSize(_alldelsize);
			emit fileOptimizeComplete(_checkfiles.count(), _delcount, _allCompleteFileSize);
			qDebug() << "&DeleteThread::snedDeleteFileComplete : " << _allCompleteFileSize << ", FileSize : " << _alldelsize;
		}

		});


	//snedDeleteFileComplete
	_deletethread->start();

}