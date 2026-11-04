#include "optimizecontroller.h"

#include <QTimer>


#include "../../view/optimize/optimizeview.h"

#include <view/common/traypopup.h>
#include <view/Component/traypopupcomponent.h>

#include "../../model/optimize/optimizdata.h"
#include "../../model/optimize/fileoptimizer.h"
#include "../../model/optimize/ieoptimizer.h"
#include "../../model/optimize/regoptimizer.h"
#include "../../model/configdata/configdata.h"

OptimizeController::OptimizeController(QObject *parent)
	: QObject(parent), _filecheck(false), _iecheck(false), _regcheck(false), _scanning(false), _optimizing(false), _automode(false), _allcount(0), _checkcount(0),
	_filetype(FileOptimizeType::none), _regtype(RegOptimizeType::none)
{
	_checkcount = 0;
	QObject::connect(this,  SIGNAL(optimizeCount()), this, SLOT(optimizeCountEnd()));

	QObject::connect(this, SIGNAL(startFilecheck()), FileOptimizer::inst(), SLOT(checkFiles()));
	QObject::connect(FileOptimizer::inst(), SIGNAL(sendfilecheckMsg(QString)), this, SLOT(getMessage(QString)));
	QObject::connect(FileOptimizer::inst(), &FileOptimizer::filechecked, [=](FileOptimizeType type) {

		if (_filetype != type)		{
			_filetype = type;
			emit sendStatusMsg(FileOptimizer::inst()->getFileTypeToString(_filetype) + "チェック中です");
		}

		_checkcount++;
		sendprogress();
		});

	//file check end
	QObject::connect(FileOptimizer::inst(), &FileOptimizer::fileCheckComplete, [=](int count) {
		_checkcount = count;
		sendprogress();
		//emit sendper(progress);
		emit sendStatusMsg("ファイルの検査が完了されました");

		//ie 최적화 검사
		emit startIEcheck();
		});


	//IE
	QObject::connect(this, SIGNAL(startIEcheck()), IEOptimizer::inst(), SLOT(checkIEFiles()));
	QObject::connect(IEOptimizer::inst(), SIGNAL(sendIEcheckMsg(QString)), this, SLOT(getMessage(QString)));
	QObject::connect(IEOptimizer::inst(), &IEOptimizer::ieFilechecked, [=](FileOptimizeType type) {

		if (_filetype != type) {
			_filetype = type;
			emit sendStatusMsg(FileOptimizer::inst()->getFileTypeToString(_filetype) + "チェック中です");
		}

		_checkcount++;
		sendprogress();
		});

	//ie check end
	QObject::connect(IEOptimizer::inst(), &IEOptimizer::ieFileCheckComplete, [=](int count) {
		//_checkcount = count;
		sendprogress();
		emit sendStatusMsg("ファイルの検査が完了されました");

		//reg 최적화 검사

		/*if(ConfigData::inst()->getRegOptimizeOpt())
			emit startRegcheck();
		else {*/
			emit sendStatusMsg("最適化項目のチェックが完了しました"); //최적화 할 항목들 검사 완료 되었습니다.

			emit allcheckfinished();

			_scanning = false;

			//auto 최적화 일 경우
			if (_automode)
				emit clickOtpBtn();
		//}

		});



	//reg=======
	QObject::connect(this, SIGNAL(startRegcheck()), RegOptimizer::inst(), SLOT(checkRegs()));
	QObject::connect(RegOptimizer::inst(), &RegOptimizer::regchecked, [=](RegOptimizeType type) {

		if (_regtype != type) {
			_regtype = type;
			emit sendStatusMsg(RegOptimizer::inst()->getRegTypeToString(_regtype) + "チェック中です");
		}

		_checkcount++;
		sendprogress();
		
		});
	
	//reg check end
	QObject::connect(RegOptimizer::inst(), &RegOptimizer::regCheckComplete, [=](int count) {
		_checkcount = _allcount;
		sendprogress(true);

		//emit sendper(100); //end
		emit sendStatusMsg("最適化項目のチェックが完了しました"); //최적화 할 항목들 검사 완료 되었습니다.

		emit allcheckfinished();

		_scanning = false;

		//auto 최적화 일 경우
		if (_automode) 
			emit clickOtpBtn();

		});


	//start  file optimize
	QObject::connect(this, SIGNAL(startFileOptimize()), FileOptimizer::inst(), SLOT(optimizeFiles()));
	QObject::connect(FileOptimizer::inst(), &FileOptimizer::sendfileoptimized, [=](fileOptimizeData data, bool delsuccess) {

		QString msg;
		if (delsuccess) 
			msg = " 最適化完了";//최적화 완료
		else
			msg = " 削除失敗"; //삭제 실패

		emit sendStatusMsg(FileOptimizer::inst()->getFileTypeToString(data.type) + msg);

		_checkcount++;
		sendprogress();
		});

	QObject::connect(FileOptimizer::inst(), &FileOptimizer::fileOptimizeComplete, [=](size_t allcount, size_t success_count, QString filesize) {


		QString msg = QString("システムファイル最適化完了 (%1, %2)").arg(success_count).arg(filesize);
		emit sendStatusMsg(msg);

		//if (_filetype != type) {
			//_filetype = type;
			//emit sendStatusMsg(FileOptimizer::inst()->getFileTypeToString(_filetype) + "チェック中です");
		//}

		_checkcount = allcount;
		sendprogress();

		//ie최적화 진행
		emit startIEFileOptimize();

		//기록
		emit setScanTime();
		});


	//IE Optimize
	QObject::connect(this, SIGNAL(startIEFileOptimize()), IEOptimizer::inst(), SLOT(optimizeIEFiles()));
	QObject::connect(IEOptimizer::inst(), &IEOptimizer::sendIEoptimized, [=](fileOptimizeData data, bool delsuccess) {

		QString msg;
		if (delsuccess)
			msg = " 最適化完了";//최적화 완료
		else
			msg = " 削除失敗";//삭제 실패

		emit sendStatusMsg(IEOptimizer::inst()->getIEFileTypeToString(data.type) + msg);
		_checkcount++;
		sendprogress();
		});

	QObject::connect(IEOptimizer::inst(), &IEOptimizer::ieOptimizeComplete, [=](size_t allcount, size_t success_count, QString filesize) {

		QString msg = QString("ブラウザキャッシュ最適化完了 (%1, %2)").arg(success_count).arg(filesize);
		emit sendStatusMsg(msg);
		sendprogress();

		//reg최적화 진행
		//if (ConfigData::inst()->getRegOptimizeOpt())
			//emit startRegOptimize();
		//else {
			_checkcount = _allcount;
			sendprogress(true);

			_optimizing = false;

			emit allOptimizefinished();
			if (_automode) {
				_automode = false;

				//초기 화면으로 전달
				emit setInitPage();

				openTrayPopup("スマート最適化完了");
			}
		//}

		});


	//reg optimize(사용안함)
	QObject::connect(this, SIGNAL(startRegOptimize()), RegOptimizer::inst(), SLOT(optimizeRegs()));
	QObject::connect(RegOptimizer::inst(), &RegOptimizer::sendRegoptimized, [=](regOptimizeData data, bool delsuccess) {

		QString msg;
		if (delsuccess)
			msg = " 最適化完了";//최적화 완료
		else
			msg = " 削除失敗";//삭제 실패

		emit sendStatusMsg(RegOptimizer::inst()->getRegTypeToString(data.optimizetype) + msg);
		_checkcount++;
		sendprogress();
		});

	//전체 최적화 완료
	QObject::connect(RegOptimizer::inst(), &RegOptimizer::regOptimizeComplete, [=](size_t allcount, size_t success_count) {

		QString msg = QString("レジストリ最適化完了 (%1)").arg(success_count);
		emit sendStatusMsg(msg);
		_checkcount = _allcount;
		sendprogress(true);

		_optimizing = false;

		emit allOptimizefinished();
		if (_automode) {
			_automode = false;


			//초기 화면으로 전달
			emit setInitPage();

			
			openTrayPopup("スマート最適化完了");

		}
			
		});



	//set optimize=========
	QObject::connect(this, SIGNAL(fileSearchCount()), FileOptimizer::inst(), SLOT(getCheckfileCount()));
	QObject::connect(FileOptimizer::inst(), &FileOptimizer::getFileSearchCount, [=](size_t count) {
		if (!_filecheck) {
			_allcount += count;
			_filecheck = true;
			emit optimizeCount();
		}
		});


	QObject::connect(this, SIGNAL(ieSarchCount()), IEOptimizer::inst(), SLOT(getCheckIEfileCount()));
	QObject::connect(IEOptimizer::inst(), &IEOptimizer::getIEFileSearchCount, [=](size_t count) {
		if (!_iecheck) {
			_allcount += count;
			_iecheck = true;
			emit optimizeCount();
		}
		});

	//set optimize=========


	//pcoffOptimize
	QObject::connect(FileOptimizer::inst(), &FileOptimizer::pcOffOptiStart, [=]() {
		emit hideOptimize(false);
		});

	//set config
	QObject::connect(this, SIGNAL(setScanTime()), ConfigData::inst(), SLOT(updateOptimizeDate()));
}

OptimizeController::~OptimizeController()
{}

void OptimizeController::getSystemCheckValue(QString& count, QString& size)
{
	count = size = "0";
	QStringList ar = FileOptimizer::inst()->getFileOptimizeCheckValue();
	if (ar.count() != 2)
		return;

	count = ar[0];
	size = ar[1];
}

void OptimizeController::getSystemFailValue(size_t& count, size_t& size)
{
	count = size = 0;

	auto faildata = FileOptimizer::inst()->getFileOptimizeFailValue();
	size = std::get<0>(faildata);
	count = std::get<1>(faildata);
		
	//QStringList ar = FileOptimizer::inst()->getFileOptimizeFailValue();
	//if (ar.count() != 2)
	//	return;

	//count = ar[0];
	//size = ar[1];
}

void OptimizeController::getIECheckValue(QString& count, QString& size)
{
	count = size = "0";
	QStringList ar = IEOptimizer::inst()->getIEFileOptimizeCheckValue();
	if (ar.count() != 2)
		return;

	count = ar[0];
	size = ar[1];
}

void OptimizeController::getIEFailValue(size_t& count, size_t& size)
{
	count = size = 0;

	auto faildata = IEOptimizer::inst()->getIEFileOptimizeFailValue();
	size = std::get<0>(faildata);
	count = std::get<1>(faildata);

}

void OptimizeController::getRegFailValue(size_t& count)
{
	count = RegOptimizer::inst()->getRegOptimizeFailValue();
}

QString OptimizeController::getRegCheckValue()
{
	return RegOptimizer::inst()->getRegOptimizeCheckValue();
}

QString OptimizeController::getRegOptimizeValue()
{
	return RegOptimizer::inst()->getRegOptimizeValue();
}

void OptimizeController::setScanDate()
{
	emit setScanTime();
}

void OptimizeController::sendprogress(bool end)
{
	if(end)
		emit sendper(100);
	else {

		int progress = (float)_checkcount / _allcount * 100;
		emit sendper(progress);
	}
}

void OptimizeController::openTrayPopup(QString msg)
{
	RegOptimizer::inst()->teminateDelThread();


	//auto traypopup = new TrayPopup(msg);
	//traypopup->show();


	emit callTraypopup(msg);
}

void OptimizeController::setOptimize()
{

	_checkcount = _allcount = 0;
	_filecheck = _iecheck = _regcheck = false;

	_filetype = FileOptimizeType::none;
	_regtype = RegOptimizeType::none;

	_scanning = true;
	_optimizing = false;

	qDebug() << "start fileSearchCount";
	emit fileSearchCount();

	qDebug() << "start ieSarchCount";
	emit ieSarchCount();

	/*QTimer::singleShot(10, [=]() {
		_allcount += RegOptimizer::inst()->getAllCheckRegCount();
		_regcheck = true;
		emit optimizeCount();
		});*/
}


void OptimizeController::optimizeCountEnd()
{
	if (_filecheck == true && _iecheck == true/* && _regcheck == true*/) {
		//전송
		//emit sendStatusMsg("최적화 할 항목들 검사 시작 합니다.");
		emit sendStatusMsg("ファイル最適化チェック中");
		emit startFilecheck();
	}
}


void OptimizeController::getMessage(QString msg)
{
	emit sendStatusMsg(msg);
}

void OptimizeController::startOptimize()
{
	qDebug() << "startOptimize()";

	_checkcount = 0;
	_scanning = false;
	_optimizing = true;

	_allcount = FileOptimizer::inst()->getOptimizeFileCount();
	_allcount += IEOptimizer::inst()->getOptimizeIeFileCount();

	qDebug() << "call startFileOptimize()";

	emit startFileOptimize();
}

void OptimizeController::hideOptimize(bool lightver)
{
	if (_scanning == true || _optimizing == true) //수동 최적화 중
		return;

	if (_automode) //이미 자동 최적화 중이다.
		return;

	_automode = true;
	emit clickOtpBtn();

	//setOptimize();
}

void OptimizeController::pcoffOptimize()
{
	FileOptimizer::inst()->pcoffRecycleBin();
}