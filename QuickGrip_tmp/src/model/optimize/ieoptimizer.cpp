#include "ieoptimizer.h"

#include <QThread>
#include <QStandardPaths>

#include "../../common/tool/utils.h"

QMap<FileOptimizeType, QString> _ieFileTypetoName = {
	{FileOptimizeType::EdgeCache, "edgeブラウザキャッシュファイル"}, //엣지 브라우저 캐시 파일
	{FileOptimizeType::EdgeCookie, "edgeブラウザクッキーファイル"}, //엣지 브라우저 쿠키 파일
	{FileOptimizeType::EdgeCodeCache, "edgeブラウザコードキャッシュファイル"}, //엣지 브라우저 코드 캐시 파일
	{FileOptimizeType::IECache, "エクスプローラーブラウザキャッシュファイル"}, //익스플로러 브라우저 캐시 파일
	{FileOptimizeType::none, "未分類のファイル"} //미 분류된 파일
};

QMap<FileOptimizeType, QString> _ieFileTypetoOptimizeDescription = {
	{FileOptimizeType::EdgeCache, "キャッシュファイルを整理すると、古い情報の除去及びブラウザの性能向上により、システムのストレージ容量が増えます"}, //캐시 파일을 정리하면 오래된 정보가 제거되어 브라우저 성능이 향상되고, 시스템의 저장 공간이 확보됩니다.
	{FileOptimizeType::EdgeCookie, "クッキーファイルを削除すればオンラインでの個人情報セキュリティが強化され、ブラウザの反応速度が改善されます"}, //쿠키 파일을 삭제하면 온라인 개인 정보 보호가 강화되며, 브라우저의 반응 속도가 개선될 수 있습니다.
	{FileOptimizeType::EdgeCodeCache, "コードキャッシュファイルを整理すると最新ウェブサイトコードでの接近が保証され、不要なスペース占領を減らしシステムの性能を改善します。"}, //코드 캐시 파일을 정리하면 최신 웹사이트 코드로의 접근이 보장되며, 불필요한 공간 점유를 줄여 시스템 성능을 개선합니다.
	{FileOptimizeType::IECache, "キャッシュファイルを整理すると、古い情報の除去及びブラウザの性能向上により、システムのストレージ容量が増えます"}, //캐시 파일을 정리하면 오래된 정보가 제거되어 브라우저 성능이 향상되고, 시스템의 저장 공간이 확보됩니다.
	{FileOptimizeType::none, "未分類のファイル"} //미 분류된 파일
};

QMap<FileOptimizeType, QString> _ieFileTypetoDescription = {
	{FileOptimizeType::EdgeCache, "edgeブラウザキャッシュファイル"}, //웹 페이지 실행 시 자동으로 Edge에 의해 저장된 웹사이트 데이터입니다.
	{FileOptimizeType::EdgeCookie, "edgeブラウザクッキーファイル"}, //Edge 를 통해 웹사이트 방문 정보와 사용자 설정을 저장하는 작은 데이터 파일입니다.
	{FileOptimizeType::EdgeCodeCache, "edgeブラウザコードキャッシュファイル"}, //웹사이트의 자바스크립트 코드 실행을 가속화하기 위해 저장된 파일입니다.
	{FileOptimizeType::IECache, "エクスプローラーブラウザキャッシュファイル"}, //웹 페이지 실행 시 자동으로 Internet Explorer에 의해 저장된 웹사이트 데이터입니다.
	{FileOptimizeType::none, "未分類のファイル"} //미 분류된 파일
};



QMap<FileOptimizeType, bool> _ieFileTypetoCheck = {
	{FileOptimizeType::EdgeCache, false},
	{FileOptimizeType::EdgeCookie, true},
	{FileOptimizeType::EdgeCodeCache, false},
	{FileOptimizeType::IECache, true}
};



IEOptimizer::IEOptimizer(QObject *parent)
	: QObject(parent), _searchthread(Q_NULLPTR), _deletethread(Q_NULLPTR), _alliecount(0), _allCompleteieCount(""), _allCompleteieSize("")
	, _delcount(0), _alldelsize(0), _delfailcount(0), _alldelfailsize(0)
{
	_search_paths.clear();
	_checkfiles.clear();
	_failfiles.clear();
	_filetypeinfo.clear();
	_optimizeIEType.clear();
}

IEOptimizer::~IEOptimizer()
{
	if (!_searchthread.isNull()) {
		_searchthread->deleteLater();
		_searchthread = Q_NULLPTR;
	}

	if (!_deletethread.isNull()) {
		_deletethread->deleteLater();
		_deletethread = Q_NULLPTR;
	}

}


void iefilecheckthread() {
	forever{

		IEOptimizer::inst()->getIEFileOptimizeSearchCount();
	return;

		if (QThread::currentThread()->isInterruptionRequested()) {
			return;
		}
	}
}

void IEOptimizer::getAllCheckIECount()
{
	auto thread = QThread::create(iefilecheckthread);
	thread->start();
	connect(thread, &QThread::finished, this, [=]() {
		emit getIEFileSearchCount(_alliecount);
	
		thread->deleteLater();
		});
}

void IEOptimizer::getCheckIEfileCount()
{
	getAllCheckIECount();
}

size_t IEOptimizer::getIEFileOptimizeSearchCount()
{
	_alliecount = 0;
	_search_paths.clear();

	size_t result = 0;

	//appdata
	QString appLocalPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
	//QString appPath = appLocalPath.left(appLocalPath.lastIndexOf("/") + 1);
	appLocalPath += "/";


	//엣지 캐시
	{
		//\AppData\Local\Microsoft\Edge\User Data\Default\Cache\Cache_Data
		QString edgecachePath = appLocalPath + "Microsoft/Edge/User Data/Default/Cache/Cache_Data/";
		{
			size_t size = utils::getAllFileCountSubfolder(edgecachePath);
			if (size > 0) {

				_alliecount += size;
				QStringList paths;
				paths.append(edgecachePath);
				_search_paths.insert(FileOptimizeType::EdgeCache, paths);

				if (getIEFileTypeToCheck(FileOptimizeType::EdgeCache) == Qt::Checked) {
					updateIEOptimizeState(FileOptimizeType::EdgeCache, true);
				}
			}
		}
	}

	//쿠키 파일
	{
		//\AppData\Local\Microsoft\Edge\User Data\Default\Cache\Cache_Data
		QString edgecachePath = appLocalPath + "Microsoft/Edge/User Data/Default/Cookies/";
		{
			size_t size = utils::getAllFileCountSubfolder(edgecachePath);
			if (size > 0) {

				_alliecount += size;
				QStringList paths;
				paths.append(edgecachePath);
				_search_paths.insert(FileOptimizeType::EdgeCookie, paths);

				if (getIEFileTypeToCheck(FileOptimizeType::EdgeCookie) == Qt::Checked) {
					updateIEOptimizeState(FileOptimizeType::EdgeCookie, true);
				}
			}

		}
	}

	//엣지 코드 캐시 (우선 제거 해보자)
	/*{
		QString edgeCodecachePath = appLocalPath + "Microsoft/Edge/User Data/Default/Code Cache/";
		{
			size_t size = utils::getAllFileCountSubfolder(edgeCodecachePath);
			if (size > 0) {

				_alliecount += size;
				QStringList paths;
				paths.append(edgeCodecachePath);
				_search_paths.insert(FileOptimizeType::EdgeCodeCache, paths);


				if (getIEFileTypeToCheck(FileOptimizeType::EdgeCodeCache) == Qt::Checked) {
					updateIEOptimizeState(FileOptimizeType::EdgeCodeCache, true);
				}

			}
		}
	}*/


	//ie cache
	{ //\AppData\Local\Microsoft\Internet Explorer\CacheStorage

		QString iecachePath = appLocalPath + "Microsoft/Internet Explorer/CacheStorage/";
		{
			size_t size = utils::getAllFileCountSubfolder(iecachePath);
			if (size > 0) {

				_alliecount += size;
				QStringList paths;
				paths.append(iecachePath);
				_search_paths.insert(FileOptimizeType::IECache, paths);

				if (getIEFileTypeToCheck(FileOptimizeType::IECache) == Qt::Checked) {
					updateIEOptimizeState(FileOptimizeType::IECache, true);
				}
			}
		}

	}

	result = _alliecount;
	return result;
}


QString IEOptimizer::getIEFileTypeToString(FileOptimizeType type)
{
	if (_ieFileTypetoName.contains(type)) {
		return _ieFileTypetoName[type];
	}

	return "未分類のファイル";
}

QString IEOptimizer::getIEFileTypeToDesc(FileOptimizeType type)
{
	if (_ieFileTypetoDescription.contains(type)) {
		return _ieFileTypetoDescription[type];
	}

	return "未分類のファイル";
}

QString IEOptimizer::getIEFileTypeToOptimizeDesc(FileOptimizeType type)
{
	if (_ieFileTypetoOptimizeDescription.contains(type)) {
		return _ieFileTypetoOptimizeDescription[type];
	}

	return "未分類のファイル";
}

Qt::CheckState IEOptimizer::getIEFileTypeToCheck(FileOptimizeType type)
{
	if (_ieFileTypetoCheck.contains(type)) {
		if (_ieFileTypetoCheck[type])
			return Qt::Checked;
		else  return Qt::Unchecked;
	}

	return Qt::Checked;
}

QStringList IEOptimizer::getIEFileOptimizeCheckValue()
{
	QStringList ar;
	ar.push_back(_allCompleteieCount);
	ar.push_back(_allCompleteieSize);
	return ar;
}

std::tuple<size_t, size_t> IEOptimizer::getIEFileOptimizeFailValue()
{
	return std::make_tuple(_alldelfailsize, _delfailcount);
	//QStringList ar;
	//ar.push_back(QString("%1").arg(_delfailcount));
	//ar.push_back(getIEFileOptimizeSize(_alldelfailsize));
	//return ar;
}

QString IEOptimizer::getIEFileOptimizeSize(size_t sizse)
{
	if (sizse == 0)
		return "0 MB";

	return utils::getFileSizeType(sizse);
}

void IEOptimizer::checkIEFiles()
{
	if (_search_paths.size() == 0) {
		emit sendIEcheckMsg("ブラウザキャッシュの検査が完了されました");//브라우저 캐시 검사 완료되었습니다.
		_allCompleteieCount = QString("0");
		_allCompleteieSize = utils::getFileSizeType(0);

		emit ieFileCheckComplete(0);

		return;
	}


	_filetypeinfo.clear();
	_checkfiles.clear();
	_failfiles.clear();
	//_optimizeIEType.clear();

	if (_searchthread.isNull() == false) {
		_searchthread->terminate();
		delete _searchthread;
		_searchthread = Q_NULLPTR;
	}

	_searchthread = new SearchThread(_search_paths, true, this);

	//파일 한개 완료
	QObject::connect(_searchthread, &SearchThread::sendIefileinfo, [=](FileOptimizeType type, QString path, size_t size) {

		fileOptimizeData data;
		data.filepath = path;
		data.type = type;
		data.filesize = size;
		_checkfiles.push_back(data);

		emit ieFilechecked(type);
		});


	//file type별 저장
	QObject::connect(_searchthread, &SearchThread::searchFinishedbyIEType, [=](FileOptimizeType type, size_t allcount, size_t allsize) {

		fileOptimizeTypeeData data;

		
		data.typestring = getIEFileTypeToString(type);
		data.filesize = allsize;
		data.fileCount = allcount;
		_filetypeinfo.insert(type, data);

		emit sendIEcheckMsg(getIEFileTypeToString(type) + " 完了");
		});

	//전체 완료
	QObject::connect(_searchthread, &SearchThread::searchFinishedIE, [=](size_t allcount, size_t allsize) {

		_allCompleteieCount = QString("%1").arg(allcount);
		_allCompleteieSize = utils::getFileSizeType(allsize);

		emit ieFileCheckComplete(allcount);

		});


	_searchthread->start();
}


void IEOptimizer::optimizeIEFiles()
{
	if (_deletethread.isNull() == false) {
		delete _deletethread;
		_deletethread = Q_NULLPTR;
	}

	_deletethread = new DeleteThread(this);
	_deletethread->setDelFileInfo(_checkfiles, _optimizeIEType, true);

	_alldelsize = _delcount = 0;
	_alldelfailsize = _delfailcount = 0;
	//파일 한개 완료
	QObject::connect(_deletethread, &DeleteThread::snedDeleteIEFile, [=](fileOptimizeData data, bool delsuccess) {

		if (delsuccess) {
			_alldelsize += data.filesize;
			_delcount++;
		}
		else {
			_alldelfailsize += data.filesize;
			_delfailcount++;

			_failfiles.push_back(data);

			//qDebug() << "Delete Error! : " << data.filepath;
		}
			

		emit sendIEoptimized(data, delsuccess);
		});


	QObject::connect(_deletethread, &DeleteThread::snedDeleteIEFileComplete, [=]() {

		_allCompleteieCount = QString("%1").arg(_delcount);
		_allCompleteieSize = getIEFileOptimizeSize(_alldelsize);

		emit ieOptimizeComplete(_checkfiles.count(), _delcount, _allCompleteieSize);
		});


	_deletethread->start();
}

void IEOptimizer::updateIEOptimizeState(FileOptimizeType type, bool use)
{
	// QVector의 iterator를 사용하여 value의 존재 여부를 확인
	auto it = std::find(_optimizeIEType.begin(), _optimizeIEType.end(), type);

	if (use) {
		// use가 true이며 값이 존재하지 않으면 추가
		if (it == _optimizeIEType.end()) {
			_optimizeIEType.append(type);
		}
	}
	else {
		// use가 false이며 값이 존재하면 삭제
		if (it != _optimizeIEType.end()) {
			_optimizeIEType.erase(it);
		}
	}
}

bool IEOptimizer::isIEOptimizeType(FileOptimizeType type)
{
	auto it = std::find(_optimizeIEType.begin(), _optimizeIEType.end(), type);

	if (it != _optimizeIEType.end())
		return true;
	return false;
}

size_t IEOptimizer::getOptimizeIeFileCount()
{
	size_t fileCount = 0;

	//_filetypeinfo
	for (auto it = _filetypeinfo.cbegin(); it != _filetypeinfo.cend(); ++it) {
		FileOptimizeType type = it.key();
		if (isIEOptimizeType(it.key()) == true) {
			fileCount += it.value().fileCount;
		}
	}

	return fileCount;
}