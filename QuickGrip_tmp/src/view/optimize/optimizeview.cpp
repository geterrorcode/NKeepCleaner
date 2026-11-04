#include "optimizeview.h"

#include <QFrame>
#include <QAbstractButton>
#include <QMovie>

#include "view/Component/containerlayout.h"
#include "view/Container/backbuttonwidget.h"
#include "view/common/alertpopup.h"
#include "optimizefilelist.h"
//#include "view/common/traypopup.h"

#include "../../controller/optimize/optimizecontroller.h"

#include "../../common/tool/utils.h"
#include "../../common/defines/ssdefins.h"

#include "../../ui.h"


OptimizeView::OptimizeView(int marginx, int marginy, QWidget *parent)
	: QWidget(parent), m_marginsizeX(marginx), m_marginsizeY(marginy),
    _optimize_btn(Q_NULLPTR), _progressBar(Q_NULLPTR), _message(Q_NULLPTR), _result_message(Q_NULLPTR), _autoOptimize_label(Q_NULLPTR), _optimizestatus_gif(Q_NULLPTR),
    _result_system(Q_NULLPTR), _result_ie(Q_NULLPTR), _result_reg(Q_NULLPTR),
    _completresult_system(Q_NULLPTR), _completresult_ie(Q_NULLPTR), _completresult_reg(Q_NULLPTR), _check_status(false)
    , _balloonImg(Q_NULLPTR), _balloonImg2(Q_NULLPTR)
{

    _check_status = false;

    if(m_marginsizeX == 0)
        m_marginsizeX = 218;
    if (m_marginsizeY == 0)
        m_marginsizeY = 50;
    //int marginsize_x = 218;
    //int marginsize_y = 50;

    auto baseLayout = new ContainerLayout(this);
    {
        auto backgroundFrame = new QFrame;
        backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/mainbg.png"));
        baseLayout->addWidget(backgroundFrame);
    }

    _autoOptimize_label = new CustomLabel(18, "#FFD968", true, 0, 0, this);
    {
        _autoOptimize_label->setText("スマート最適化進行中です");
        _autoOptimize_label->setGeometry(0, 20, 1008 - 217, 30);
        _autoOptimize_label->setAlignment(Qt::AlignHCenter);
        _autoOptimize_label->hide();
    }


    QString gifpath = QString("%1/image/motion_none.gif").arg(qApp->applicationDirPath());
    QMovie* movie = new QMovie(gifpath);
    if (!movie->isValid()) {
        qDebug() << "gif error";
    }

    
    _optimizestatus_gif = new QLabel(this);
    {
        _optimizestatus_gif->setFixedSize(215, 228);
        _optimizestatus_gif->setMovie(movie);
        _optimizestatus_gif->setAttribute(Qt::WA_NoSystemBackground);
        _optimizestatus_gif->setGeometry(505 - m_marginsizeX, 104 - m_marginsizeY, 215, 228);
        _optimizestatus_gif->show();
    }
    movie->setScaledSize(QSize(215, 228));
    movie->start();


    auto arrow = new QLabel(this);
    arrow->setStyleSheet(ssdefines::imageWidget.arg(":/preview/resource/preview/underarrow.png"));
    arrow->setGeometry(604 - m_marginsizeX, 328 - m_marginsizeY, 16, 17);
    arrow->show();


    auto scanComment = new CustomLabel(15, "#FDBB30", true, 0, 0, this);
    //최적화가 필요한 파일을 스캔합니다.
    scanComment->setText("※最適化が必要なファイルをスキャンします");
    scanComment->setAlignment(Qt::AlignCenter);
    scanComment->setGeometry(50, 415 - m_marginsizeY, 700, 24);
    scanComment->show();


    QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preopti/resource/preview/opti/optibtn.png")).arg("QPushButton{font-weight:bold;}");

    _optimize_btn = new Button(this);
    _optimize_btn->setStyleSheet(ss);
    _optimize_btn->setText("スキャン開始");
    _optimize_btn->setGeometry(536 - m_marginsizeX, 354 - m_marginsizeY, 153, 40);
    _optimize_btn->show();
    connect(_optimize_btn, &Button::clicked, [=]() {

        QString gifpath = QString("%1/image/").arg(qApp->applicationDirPath());

        if (!_check_status) { //파일 체크
            
            movie->stop();
            movie->setFileName(gifpath + "motion_ing.gif");
            movie->start();

            scanComment->hide();
            _optimize_btn->hide();
            _progressBar->setValue(0);
            _progressBar->show();
            _completresult_system->hide();
            _completresult_ie->hide();
            //_completresult_reg->hide();
            _result_message->hide();
            _message->show();
            emit getOptimizeinfo();
        }
        else { //최적화


            //_balloonImg->hide();
            //_balloonImg2->hide();

            emit optimize();

            _optimize_btn->hide();
            _result_system->hide();
            _result_ie->hide();
            //_result_reg->hide();

            _progressBar->setValue(0);
            _progressBar->show();
            _message->show();

            movie->stop();
            movie->setFileName(gifpath + "motion_ing.gif");
            movie->start();

        }
        });

    _progressBar = new QProgressBar(this);
    _progressBar->setStyleSheet("QProgressBar {background:#202224; border:none; border-radius:8px; color: white;}"
                    "QProgressBar::chunk {background-color: #FDBB30; border-radius:8px;}");
    //_progressBar->setGeometry(464 - marginsize_x, 364 - marginsize_y, 300, 20);
    _progressBar->setGeometry(931- m_marginsizeX - 619, 375- m_marginsizeY, 619, 19);
    _progressBar->setTextVisible(true);
    _progressBar->setAlignment(Qt::AlignCenter);
    _progressBar->hide();

    _message = new CustomLabel(15, "#ffffff", true, 0, 0, this);
    //정기적인 상태 체크와 최적화를 진행하여 불필요한 문제를 예방하고 최상의 상태를 유지하세요.
    _message->setText("定期的な状態チェックと最適化を行い、不要な問題を防ぎ最高の状態を維持しましょう");
    _message->setAlignment(Qt::AlignCenter);
    _message->setGeometry(50, 500 - m_marginsizeY, 700, 24);
    _message->show();

    _result_message = new CustomLabel(15, "#ffffff", true, 0, 80, this);
    _result_message->setWordWrap(true);
    _result_message->setAlignment(Qt::AlignCenter);
    _result_message->setGeometry(50, 380- m_marginsizeY, 700, 24);
    _result_message->hide();

    { //최적화 파일 검색 결과

        if (!_result_system) {
            _result_system = new OptimizeResultWidget(":/preopti/resource/preview/opti/system.png",
                "システム", this);
            //_result_system->setGeometry(258 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
            _result_system->setGeometry(308 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
            _result_system->hide();
        }

        if (!_result_ie) {
            _result_ie = new OptimizeResultWidget(":/preopti/resource/preview/opti/ie.png",
                "ブラウザ", this);
            //_result_ie->setGeometry(504 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
            _result_ie->setGeometry(704 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
            _result_ie->hide();
        }

        //if (!_result_reg) {

        //    QString disable("レジストリ");
        //    if (!ConfigData::inst()->getRegOptimizeOpt()) {
        //        disable = "(비활성)";
        //    }

        //    _result_reg = new OptimizeResultWidget(":/preopti/resource/preview/opti/reg.png",
        //        QString("レジストリ"), this);
        //    _result_reg->setGeometry(747 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
        //    _result_reg->hide();
        //}
    }

    { //최적화 후 결과 
        if (!_completresult_system) {
            _completresult_system = new OptimizeCompleteResultWidget(":/preopti/resource/preview/opti/system.png",
                "システム", this);
            //레지스트리 최적화 있을때 위치값
            //_completresult_system->setGeometry(258 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
            _completresult_system->setGeometry(308 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
            _completresult_system->hide();
        }

        if (!_completresult_ie) {
            _completresult_ie = new OptimizeCompleteResultWidget(":/preopti/resource/preview/opti/ie.png",
                "ブラウザ", this);
            //_completresult_ie->setGeometry(504 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
            _completresult_ie->setGeometry(704 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
            _completresult_ie->hide();
        }

        //if (!_completresult_reg) {
        //    _completresult_reg = new OptimizeCompleteResultWidget(":/preopti/resource/preview/opti/reg.png",
        //        "レジストリ", this);
        //    _completresult_reg->setGeometry(747 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
        //    _completresult_reg->hide();
        //}
    }


    //_balloonImg = new QLabel(this);
    //_balloonImg->setStyleSheet(ssdefines::imageWidget.arg(":/optimize/resource/optimize/balloon.png"));
    //_balloonImg->setGeometry(472 - m_marginsizeX, 398 - m_marginsizeY, 120, 73);
    //_balloonImg->hide();

    //_balloonImg2 = new QLabel(this);
    //_balloonImg2->setStyleSheet(ssdefines::imageWidget.arg(":/optimize/resource/optimize/balloon.png"));
    //_balloonImg2->setGeometry(867 - m_marginsizeX, 398 - m_marginsizeY, 120, 73);
    //_balloonImg2->hide();

    setFixedSize(parent->size());

    QObject::connect(this, SIGNAL(getOptimizeinfo()), OptimizeController::inst(), SLOT(setOptimize()));
    QObject::connect(this, SIGNAL(optimize()), OptimizeController::inst(), SLOT(startOptimize()));
    QObject::connect(OptimizeController::inst(), SIGNAL(sendStatusMsg(QString)), this, SLOT(updateMessage(QString)));
    QObject::connect(OptimizeController::inst(), SIGNAL(sendper(int)), this, SLOT(updateProgress(int)));

    QObject::connect(OptimizeController::inst(), SIGNAL(clickOtpBtn()), this, SLOT(sendClickedOptimize()));


    QObject::connect(OptimizeController::inst(), &OptimizeController::allcheckfinished, this, [=]() {
        _check_status = true;

        _progressBar->hide();
        _message->hide();
        _optimize_btn->show();
        _optimize_btn->setText("最適化開始");

        size_t checkfilecount = 0, checkfilesize = 0;

        QString filecount, filesize;
        OptimizeController::inst()->getSystemCheckValue(filecount, filesize);
        _result_system->setOptiText(QString("ファイル : %1 files").arg(filecount),  QString("容量 : %1").arg(filesize));
        _result_system->show();
       
        checkfilecount = filecount.toInt();

        if (filesize.indexOf("GB") > 0 || filesize.indexOf("TB") > 0) {
            checkfilesize = 1000;
        }
        else if (filesize.indexOf("KB") > 0){
            checkfilesize = 0;
        }
        else { //mb
            QString mb = filesize.left(filesize.indexOf("MB"));
            checkfilesize = (int)mb.toFloat();
        }

        OptimizeController::inst()->getIECheckValue(filecount, filesize);
        _result_ie->setOptiText(QString("ファイル : %1 files").arg(filecount), QString("容量 : %1").arg(filesize));
        _result_ie->show();
        

        checkfilecount+= filecount.toInt();
        if (filesize.indexOf("GB") > 0 || filesize.indexOf("TB") > 0) {
            checkfilesize = 1000;
        }
        else if (filesize.indexOf("KB") > 0) {
            checkfilesize = 0;
        }
        else { //mb
            QString size = filesize.left(filesize.indexOf("MB"));
            checkfilesize += (int)size.toFloat();
        }

        //QString msg;
        //if (ConfigData::inst()->getRegOptimizeOpt())
        //    msg = QString("ファイル : %1 files").arg(OptimizeController::inst()->getRegCheckValue());
        //else
        //    msg = "비활성화 된 상태";

        //_result_reg->setOptiText(msg);
        //_result_reg->show();

        checkfilecount += OptimizeController::inst()->getRegCheckValue().toInt();


        //_balloonImg->show();
        //_balloonImg2->show();

        checkfilecount += OptimizeController::inst()->getRegCheckValue().toInt();

        QString gifpath; 
        
        OptimizeLevel level = getLevel(checkfilecount, checkfilesize);

        if (level == OptimizeLevel::Good) {
            gifpath = QString("%1/image/motion_new_good.gif").arg(qApp->applicationDirPath());
            if(!QFile::exists(gifpath))
                gifpath = QString("%1/image/motion_good.gif").arg(qApp->applicationDirPath());
        }
        else if (level == OptimizeLevel::Bad) {
            gifpath = QString("%1/image/motion_new_bad.gif").arg(qApp->applicationDirPath());
            if (!QFile::exists(gifpath))
                gifpath = QString("%1/image/motion_bad.gif").arg(qApp->applicationDirPath());
        }
        else {
            gifpath = QString("%1/image/motion_new.gif").arg(qApp->applicationDirPath());
            if (!QFile::exists(gifpath))
                gifpath = QString("%1/image/motion.gif").arg(qApp->applicationDirPath());
        }

        movie->stop();
        movie->setFileName(gifpath);
        movie->start();

        });

    QObject::connect(OptimizeController::inst(), &OptimizeController::allOptimizefinished, this, [=]() {

        //원래 용량, 삭제된 용량 비교
        _progressBar->hide();
        _message->hide();
        _optimize_btn->show();
        //_optimize_btn->setText("状態チェック");
        _optimize_btn->setText("スキャン開始"); //스캔시작
        
        size_t checkfilecount = 0 , checkfilesizetmp = 0, checkfilesize = 0;
        //하단에 성공/실패 항목 보여줌
        QString filecount, filesize;
        OptimizeController::inst()->getSystemCheckValue(filecount, filesize);
        size_t filefailcount, filefailsize = 0;
        OptimizeController::inst()->getSystemFailValue(filefailcount, filefailsize);
        _completresult_system->setOptiCompleteText(QString("ファイル : %1 files").arg(filecount), QString("容量 : %1").arg(filesize), QString("ファイル : %1 files").arg(filefailcount), QString("容量 : %1").arg(utils::getFileSizeType(filefailsize)));
        _completresult_system->show();
        

        checkfilecount += filefailcount;
        checkfilesizetmp += filefailsize;


        OptimizeController::inst()->getIECheckValue(filecount, filesize);
        size_t iefilefailcount, iefilefailsize = 0;
        OptimizeController::inst()->getIEFailValue(iefilefailcount, iefilefailsize);
        _completresult_ie->setOptiCompleteText(QString("ファイル : %1 files").arg(filecount), QString("容量 : %1").arg(filesize), QString("ファイル : %1 files").arg(iefilefailcount), QString("容量 : %1").arg(utils::getFileSizeType(iefilefailsize)));
        _completresult_ie->show();
       

        checkfilecount += iefilefailcount;
        checkfilesizetmp += iefilefailsize;


        //size_t regfilefailcount = 0;
        //if (!ConfigData::inst()->getRegOptimizeOpt()) {
        //    _completresult_reg->setOptiCompleteText(Q_NULLPTR, Q_NULLPTR, "비활성화 된 상태", Q_NULLPTR);
        //}
        //else {
        //    OptimizeController::inst()->getRegFailValue(regfilefailcount);
        //    _completresult_reg->setOptiCompleteText(QString("ファイル : %1 files").arg(OptimizeController::inst()->getRegOptimizeValue()), Q_NULLPTR, QString("ファイル : %1 files").arg(regfilefailcount), Q_NULLPTR);
        //}
        //_completresult_reg->show();
        //checkfilecount += regfilefailcount;


        if (checkfilecount > 0) {
            _result_message->setText("他のプログラムを実行中のため最適化されてない項目があります。\r\nプログラム終了後、再度最適化してください。");
            _result_message->show();
        }

        { //gif area
            QString filesize = utils::getFileSizeType(checkfilesizetmp);
            if (filesize.indexOf("GB") > 0 || filesize.indexOf("TB") > 0) 
                checkfilesize = 1000;
            else if (filesize.indexOf("KB") > 0) 
                checkfilesize = 0;
            else { //mb
                QString test = filesize.left(filesize.indexOf("MB"));
                checkfilesize += (int)test.toFloat();
            }

            OptimizeLevel level = getLevel(checkfilecount, checkfilesize);
            QString gifpath;
            if (level == OptimizeLevel::Good) {
                gifpath = QString("%1/image/motion_new_good.gif").arg(qApp->applicationDirPath());
                if (!QFile::exists(gifpath))
                    gifpath = QString("%1/image/motion_good.gif").arg(qApp->applicationDirPath());
            }
            else if (level == OptimizeLevel::Bad) {
                gifpath = QString("%1/image/motion_new_bad.gif").arg(qApp->applicationDirPath());
                if (!QFile::exists(gifpath))
                    gifpath = QString("%1/image/motion_bad.gif").arg(qApp->applicationDirPath());
            }
            else {
                gifpath = QString("%1/image/motion_new.gif").arg(qApp->applicationDirPath());
                if (!QFile::exists(gifpath))
                    gifpath = QString("%1/image/motion.gif").arg(qApp->applicationDirPath());
            }

            movie->stop();
            movie->setFileName(gifpath);
            movie->start();

            _check_status = false;
        }
        
        });


    QObject::connect(OptimizeController::inst(), &OptimizeController::setInitPage, this, [=]() {
        _autoOptimize_label->hide();
        _completresult_system->hide();
        _completresult_ie->hide();
        //_completresult_reg->hide();
        _result_message->hide();

        _message->setText("定期的な状態チェックと最適化を行い不要な問題を予防し、最良の状態を維持します");
        _message->show();
        });


    connect(_result_system, &OptimizeResultWidget::sendClicked, [=]() {
        auto filelist = new OptimizeFileList(OptimizeType::FileType, false, window());
        filelist->show();
        });

    connect(_result_ie, &OptimizeResultWidget::sendClicked, [=]() {
        auto filelist = new OptimizeFileList(OptimizeType::IEFileType, false, window());
        filelist->show();
        });

    //connect(_result_reg, &OptimizeResultWidget::sendClicked, [=]() {
    //    auto filelist = new OptimizeFileList(OptimizeType::RegType, false, window());
    //    filelist->show();
    //    });

    connect(_completresult_system, &OptimizeCompleteResultWidget::sendCompleteResultClicked, [=]() {
        auto filelist = new OptimizeFileList(OptimizeType::FileType, true, window());
        filelist->show();
        });

    connect(_completresult_ie, &OptimizeCompleteResultWidget::sendCompleteResultClicked, [=]() {
        auto filelist = new OptimizeFileList(OptimizeType::IEFileType, true, window());
        filelist->show();
        });

    //connect(_completresult_reg, &OptimizeCompleteResultWidget::sendCompleteResultClicked, [=]() {
    //    auto filelist = new OptimizeFileList(OptimizeType::RegType, true, window());
    //    filelist->show();
    //    });

    //{ //pcoff tray popup
    //    connect(OptimizeController::inst(), &OptimizeController::callTraypopup, [=](QString msg) {
    //        //QString msg = QString("PC자동 종료 설정으로 \r\nPC가 <font color=#FDBB30>5분후</font> 자동 종료 됩니다.");
    //        auto traypopup = new TrayPopup("test");
    //        traypopup->show();
    //        });
    //}

}

OptimizeView::~OptimizeView()
{}

void OptimizeView::initResource()
{
    //{ //최적화 파일 검색 결과
    //    if (!_result_system) {
    //        _result_system = new OptimizeResultWidget(":/preopti/resource/preview/opti/system.png",
    //            "アドレス", this);
    //        _result_system->setGeometry(258 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
    //        _result_system->hide();
    //    }

    //    if (!_result_ie) {
    //        _result_ie = new OptimizeResultWidget(":/preopti/resource/preview/opti/ie.png",
    //            "システム", this);
    //        _result_ie->setGeometry(504 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
    //        _result_ie->hide();
    //    }

    //    if (!_result_reg) {
    //        _result_reg = new OptimizeResultWidget(":/preopti/resource/preview/opti/system.png",
    //            "ブラウザ", this);
    //        _result_reg->setGeometry(747 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
    //        _result_reg->hide();
    //    }
    //}


    //{ //최적화 후 결과 
    //    if (!_completresult_system) {
    //        _completresult_system = new OptimizeCompleteResultWidget(":/preopti/resource/preview/opti/system.png",
    //            "アドレス", this);
    //        _completresult_system->setGeometry(258 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
    //        _completresult_system->hide();
    //    }

    //    if (!_completresult_ie) {
    //        _completresult_ie = new OptimizeCompleteResultWidget(":/preopti/resource/preview/opti/ie.png",
    //            "システム", this);
    //        _completresult_ie->setGeometry(504 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
    //        _completresult_ie->hide();
    //    }

    //    if (!_completresult_reg) {
    //        _completresult_reg = new OptimizeCompleteResultWidget(":/preopti/resource/preview/opti/system.png",
    //            "ブラウザ", this);
    //        _completresult_reg->setGeometry(747 - m_marginsizeX, 444 - m_marginsizeY, 221, 202);
    //        _completresult_reg->hide();
    //    }
    //}
}

OptimizeLevel OptimizeView::getLevel(int filecount, int filesize)
{
    if (filecount <= 100 && filesize <= 100)
        return OptimizeLevel::Good;
    else if(filecount >= 500 || filesize >= 1000)
        return OptimizeLevel::Bad;
    return OptimizeLevel::Nomal;
}

void OptimizeView::updateProgress(int per)
{
    if(_progressBar->value() < per)
        _progressBar->setValue(per);
}

void OptimizeView::updateMessage(QString Message)
{
    //qDebug() << "updateMessage : " << Message;
    _message->setText(Message);
}

void OptimizeView::sendClickedOptimize()
{
    if (_optimize_btn) {
        _autoOptimize_label->show();
        _optimize_btn->clicked(true);
    }
}