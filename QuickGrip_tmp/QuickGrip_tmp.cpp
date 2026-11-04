#include "QuickGrip_tmp.h"

#include <QApplication>
#include <QJsonDocument>
#include <QFrame>
#include <QMenu>
#include <QDir>
#include <QSystemTrayIcon>
#include <QSysInfo>
#include <QProcess>

#include <Logger.h>

#include "common/ux/singleton.h"
#include "common/qvalue.h"
#include "common/network/localserver.h"

#include "view/Component/containerlayout.h"
#include "view/Component/vlayout.h"
#include "view/Component/hlayout.h"
#include "view/Component/dragger.h"
#include "view/common/traypopup.h"

#include "view/Component/traypopupcomponent.h"
#include "view/Component/shadow.h"

#include "view/optimize/optimizefilelist.h"

#include "view/main/maintitlecontainer.h"
#include "view/main/mainbottomcontainer.h"
#include "view/main/AuthSerialKeyResetPopup.h"
#include "view/main/authexpiredkeypopup.h"
#include "view/optimize/optimizeview.h"
#include "view/common/alertpopup.h"

#include "model/optimize/fileoptimizer.h"

#include "controller/optimize/optimizecontroller.h"
#include "controller/startup/startupcontroller.h"
#include "controller/smart_optimize/smartoptimizecontroller.h"
//#include "controller/smart_optimize/pcendsesstionevent.h"
#include "controller/auto_pcoff/autopcoffcontroller.h"
#include "controller/common/commoncontroller.h"
#include "controller/account/accountcontroller.h"

#include "src/common/defines/commondefine.h"
#include "src/common/defines/ssdefins.h"
#include "src/common/tool/utils.h"
#include "src/common/tool/svcinstaller.h"
//#include "src/common/tool/wndeventfilter.h"
#include "src/common/tool/stateeventhandler.h"
#include "src/common/network/autochecker.h"

#include "src/ui.h"



QuickGrip_tmp::QuickGrip_tmp(QWidget* parent)
    : QMainWindow(parent), _contentswidget(Q_NULLPTR), _optimizeview(Q_NULLPTR),
    _smartoptimizeView(Q_NULLPTR), _boostpingview(Q_NULLPTR), _autopcoffview(Q_NULLPTR), _startupview(Q_NULLPTR), _appwizview(Q_NULLPTR), _pcspecview(Q_NULLPTR), _optionview(Q_NULLPTR), _supportview(Q_NULLPTR), _accountview(Q_NULLPTR),
    _firstClick_startup(false), _firstClick_appwiz(false), _tray_option(false), _tray_run(false),
    _mainstatus_gif(Q_NULLPTR), _mainstatus_level(Q_NULLPTR), _mainstatus_lastexecute(Q_NULLPTR),
    _lastrun_label(Q_NULLPTR), _space_label(Q_NULLPTR), _reserve_label(Q_NULLPTR), _back_button(Q_NULLPTR),
    _main_text1(Q_NULLPTR), _main_text2(Q_NULLPTR), _main_text3(Q_NULLPTR),
    _bottom_speed(Q_NULLPTR), _bottom_space(Q_NULLPTR), _bottom_reserv(Q_NULLPTR), _bottom_ping(Q_NULLPTR),
    _menu_opti(Q_NULLPTR), _menu_smartopti(Q_NULLPTR), _menu_boostping(Q_NULLPTR), _menu_pcoff(Q_NULLPTR),
    _menu_startup(Q_NULLPTR), _menu_appwiz(Q_NULLPTR), _menu_pcspec(Q_NULLPTR), _menu_option(Q_NULLPTR), _menu_as(Q_NULLPTR), _menu_account(Q_NULLPTR),
    _authwaitingpopup(Q_NULLPTR), _mainstatus(Q_NULLPTR), _oldselect(viewIndex::none)
{
   
    PLOG_INFO << "N-KeepCleaner START";
    /*QByteArray machineUniqueId = QSysInfo::machineUniqueId();
    QString devicekey = (QString)machineUniqueId;
    int len = devicekey.size();*/

    // Convert QByteArray to QString
    //QString uuidString = QString::fromUtf8(machineUniqueId.toHex());

    
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    resize(MAIN_WIDTH, MAIN_HEIGTH);
    auto centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    auto baseLayout = new VLayout(centralWidget);
    {
        auto backgroundFrame = new QWidget;
        baseLayout->addWidget(backgroundFrame);
    }
    setCentralWidget(centralWidget);

    {
        //top
        auto topwidget = new QWidget;
        topwidget->setFixedHeight(50);
        topwidget->setStyleSheet(ssdefines::imageWidget.arg(":preview/resource/preview/top.png"));
        //topwidget->setStyleSheet("background-color: #ffffff; border: 1px solid #666666; ");

        auto topLayout = new ContainerLayout(topwidget);
        auto dragger = new Dragger(this);
        dragger->setFocusPolicy(Qt::StrongFocus); 
        topLayout->addWidget(dragger);

        auto topcontentsLayout = new HLayout();
        topcontentsLayout->setContentsMargins(0, 0, 0, 0);
        topLayout->addLayout(topcontentsLayout);

        auto homebtnLayout = new ContainerLayout();
        homebtnLayout->setContentsMargins(0, 0, 0, 0);

        //홈 버튼
        auto homebtn = new Button;
        {
            
            homebtn->setFixedSize(217, 52);
            homebtn->setCursor(Qt::PointingHandCursor);
            homebtn->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/title.png"));
            //auto opa = new QGraphicsOpacityEffect(homebtn);
            //opa->setOpacity(0);
            //homebtn->setGraphicsEffect(opa);
            connect(homebtn, &Button::clicked, [=]() {
                selectViewr(viewIndex::none);
                //QProcess::execute("shutdown /s /f /t 0");

                
                });
        }
        
        homebtnLayout->addWidget(homebtn);
        //=========================================================

        auto titleLayout = new ContainerLayout();
        titleLayout->setContentsMargins(10, 0, 10, 0);
        topcontentsLayout->addLayout(homebtnLayout);
        topcontentsLayout->addLayout(titleLayout);
        topcontentsLayout->addStretch(1);

        auto systembuttonLayout = new HLayout();
        systembuttonLayout->setContentsMargins(0, 0, 5, 0);

        _back_button = new BackButtonWidget(this);
        {
            _back_button->setGeometry(218, 10, 50, 50);
            _back_button->setCursor(Qt::PointingHandCursor);
            _back_button->hide();
            connect(_back_button, &BackButtonWidget::backButtonClicked, [=]() {
                selectViewr(viewIndex::none);
                });
        }
        
        auto manual = new Button(this);
        {
            manual->setGeometry(MAIN_WIDTH - 250, 7, 144, 36);
            manual->setCursor(Qt::PointingHandCursor);
            manual->setText(QObject::tr("製品説明書を見る"));
            manual->setStyleSheet("QAbstractButton {color:#FFFFFF; font: normal normal 14px;}");
            manual->setStyleSheet(QString("%1%2").arg("QAbstractButton {color:#FFFFFF; font: normal normal 14px;}").arg(ssdefines::imageButton.arg(":/common/resource/common/linebtn_normal_small.png",
                ":/common/resource/common/linebtn_over_small.png",
                ":/common/resource/common/linebtn_press_small.png",
                ":/common/resource/common/linebtn_normal_small.png")));

            manual->show();
            connect(manual, &Button::clicked, [=]() {

                QUrl url = QUrl::fromLocalFile("https://n.tnilab.com/manual/nkcManual_win.pdf");
                QDesktopServices::openUrl(url);

                });

        }

        auto close = new Button();
        {
            close->setFixedSize(30, 30);
            close->setCursor(Qt::PointingHandCursor);

            auto opa = new QGraphicsOpacityEffect(close);
            opa->setOpacity(0);
            close->setGraphicsEffect(opa);

            connect(close, &Button::clicked, [=]() {
                if (ConfigData::inst()->getTrayOpt())
                    hide();
                else {
                    PLOG_INFO << "N-KeepCleaner Click Close";
                    QCoreApplication::quit(); //종료
                }
                    

                });
        }

        auto minize = new Button();
        {
            minize->setFixedSize(30, 30);
            minize->setCursor(Qt::PointingHandCursor);

            auto opa = new QGraphicsOpacityEffect(minize);
            opa->setOpacity(0);
            minize->setGraphicsEffect(opa);
            connect(minize, &Button::clicked, this, &QWidget::showMinimized);
        }
        
        systembuttonLayout->addWidget(minize);
        systembuttonLayout->addWidget(close);
        

        topcontentsLayout->addLayout(systembuttonLayout);
                    
        baseLayout->addWidget(topwidget);
        baseLayout->addStretch(0);
    }

    {
        auto bodywidget = new QWidget;
        bodywidget->setStyleSheet("background-color: #23262A; border: 0px; ");
        bodywidget->setFixedHeight(MAIN_HEIGTH - 50 );
        
        auto bodyLayout = new HLayout(bodywidget);
        bodyLayout->setContentsMargins(0, 0, 0, 0);
        

        {
            //menu
            auto menuwidget = new QWidget;
            menuwidget->setStyleSheet(ssdefines::imageWidget.arg(":preview/resource/preview/menu.png"));
            menuwidget->setFixedWidth(217);

            //수정
            //auto menuLayout = new VLayout(menuwidget);
            auto menuLayout = new ContainerLayout(menuwidget);
            menuLayout->setContentsMargins(10, 0, 0, 0);
            bodyLayout->addWidget(menuwidget);

            int pos_y = 78;

            _menu_opti = new Button(this);
            {
                _menu_opti->setFixedSize(218, 50);
                _menu_opti->setGeometry(0, pos_y, 218, 50);
                _menu_opti->setCheckable(true);

                _menu_opti->setStyleSheet(ssdefines::imageToggleButton.arg(QString(":/menubtn/resource/main/menubtn/opti.png"))
                    .arg(":/menubtn/resource/main/menubtn/opti_hover.png")
                    .arg(":/menubtn/resource/main/menubtn/opti_click.png")
                    .arg(":/menubtn/resource/main/menubtn/opti.png")
                    .arg(":/menubtn/resource/main/menubtn/opti_check.png"));

                connect(_menu_opti, &Button::clicked, [=]() {

                    if (_menu_opti->isChecked()) {
                        PLOG_INFO << "N-KeepCleaner Click Optimize";
                        selectViewr(viewIndex::optimize);
                    }
                    });

                _menu_opti->show();
            }


            pos_y += 56;
            _menu_smartopti = new Button(this);
            {
                _menu_smartopti->setFixedSize(218, 50);
                _menu_smartopti->setGeometry(0, pos_y, 218, 50);
                _menu_smartopti->setCheckable(true);

                _menu_smartopti->setStyleSheet(ssdefines::imageToggleButton.arg(QString(":/menubtn/resource/main/menubtn/smartopti.png"))
                    .arg(":/menubtn/resource/main/menubtn/smartopti_hover.png")
                    .arg(":/menubtn/resource/main/menubtn/smartopti_click.png")
                    .arg(":/menubtn/resource/main/menubtn/smartopti.png")
                    .arg(":/menubtn/resource/main/menubtn/smartopti_check.png"));


                connect(_menu_smartopti, &Button::clicked, [=]() {
                    if (_menu_smartopti->isChecked()) {

                        PLOG_INFO << "N-KeepCleaner Click Smart Optimize";
                        selectViewr(viewIndex::smartoptimize);
                    }
                    
                    });

                _menu_smartopti->show();

                //menuLayout->addWidget(smartOptimze);
            }
            
            pos_y += 56;
            _menu_boostping = new Button(this);
            {
                //boostping->setFixedSize(130, 50);
                _menu_boostping->setFixedSize(218, 50);
                _menu_boostping->setGeometry(0, pos_y, 218, 50);
                _menu_boostping->setCheckable(true);
                //auto opa = new QGraphicsOpacityEffect(boostping);
                //opa->setOpacity(0);
                //boostping->setGraphicsEffect(opa);
                _menu_boostping->setStyleSheet(ssdefines::imageToggleButton.arg(QString(":/menubtn/resource/main/menubtn/boostping.png"))
                    .arg(":/menubtn/resource/main/menubtn/boostping_hover.png")
                    .arg(":/menubtn/resource/main/menubtn/boostping_click.png")
                    .arg(":/menubtn/resource/main/menubtn/boostping.png")
                    .arg(":/menubtn/resource/main/menubtn/boostping_check.png"));

                connect(_menu_boostping, &Button::clicked, [=]() {
                    if (_menu_boostping->isChecked()) {

                        PLOG_INFO << "N-KeepCleaner Click BoostPing";
                        selectViewr(viewIndex::boostping);
                    }
                        
                    });

                _menu_boostping->show();
            }

            pos_y += 56;
            _menu_pcoff = new Button(this);
            {
                //pcoff->setFixedSize(130, 50);

                _menu_pcoff->setFixedSize(218, 50);
                _menu_pcoff->setGeometry(0, pos_y, 218, 50);
                _menu_pcoff->setCheckable(true);
                //auto opa = new QGraphicsOpacityEffect(pcoff);
                //opa->setOpacity(0);
                //pcoff->setGraphicsEffect(opa);

                _menu_pcoff->setStyleSheet(ssdefines::imageToggleButton.arg(QString(":/menubtn/resource/main/menubtn/pcoff.png"))
                    .arg(":/menubtn/resource/main/menubtn/pcoff_hover.png")
                    .arg(":/menubtn/resource/main/menubtn/pcoff_click.png")
                    .arg(":/menubtn/resource/main/menubtn/pcoff.png")
                    .arg(":/menubtn/resource/main/menubtn/pcoff_check.png"));

                connect(_menu_pcoff, &Button::clicked, [=]() {
                    if (_menu_pcoff->isChecked()) {
                        PLOG_INFO << "N-KeepCleaner Click Auto PCOFF";
                        selectViewr(viewIndex::autopcoff);
                    }
                        
                    });
                //menuLayout->addWidget(pcoff);
                _menu_pcoff->show();
            }
            //menuLayout->addStretch(0);

            pos_y += 56;
            _menu_pcspec = new Button(this);
            {
                _menu_pcspec->setFixedSize(218, 50);
                _menu_pcspec->setGeometry(0, pos_y, 218, 50);
                _menu_pcspec->setCheckable(true);
                
                _menu_pcspec->setStyleSheet(ssdefines::imageToggleButton.arg(QString(":/menubtn/resource/main/menubtn/pcspec.png"))
                    .arg(":/menubtn/resource/main/menubtn/pcspec_hover.png")
                    .arg(":/menubtn/resource/main/menubtn/pcspec_click.png")
                    .arg(":/menubtn/resource/main/menubtn/pcspec.png")
                    .arg(":/menubtn/resource/main/menubtn/pcspec_check.png"));
                
                connect(_menu_pcspec, &Button::clicked, [=]() {
                    if (_menu_pcspec->isChecked()) {

                        PLOG_INFO << "N-KeepCleaner Click pc spec";
                        selectViewr(viewIndex::pcspec);
                    }
                    
                    });
                _menu_pcspec->show();
            }

            pos_y += 56;
            _menu_option = new Button(this);
            {
                _menu_option->setFixedSize(218, 50);
                _menu_option->setGeometry(0, pos_y, 218, 50);
                _menu_option->setCheckable(true);
                _menu_option->setStyleSheet(ssdefines::imageToggleButton.arg(QString(":/menubtn/resource/main/menubtn/config.png"))
                    .arg(":/menubtn/resource/main/menubtn/config_hover.png")
                    .arg(":/menubtn/resource/main/menubtn/config_click.png")
                    .arg(":/menubtn/resource/main/menubtn/config.png")
                    .arg(":/menubtn/resource/main/menubtn/config_check.png"));

                connect(_menu_option, &Button::clicked, [=]() {
                    if (_menu_option->isChecked()) {
                        PLOG_INFO << "N-KeepCleaner Click pc option";
                        selectViewr(viewIndex::optins);
                    }
                        
                    });
                _menu_option->show();
            }


            pos_y += 56;
            _menu_as = new Button(this);
            {
                _menu_as->setFixedSize(218, 50);
                _menu_as->setGeometry(0, pos_y, 218, 50);
                _menu_as->setCheckable(true);
                _menu_as->setStyleSheet(ssdefines::imageToggleButton.arg(QString(":/menubtn/resource/main/menubtn/as.png"))
                    .arg(":/menubtn/resource/main/menubtn/as_hover.png")
                    .arg(":/menubtn/resource/main/menubtn/as_click.png")
                    .arg(":/menubtn/resource/main/menubtn/as.png")
                    .arg(":/menubtn/resource/main/menubtn/as_check.png"));

                connect(_menu_as, &Button::clicked, [=]() {
                    if (_menu_as->isChecked()) {
                        PLOG_INFO << "N-KeepCleaner Click as";
                        selectViewr(viewIndex::as);
                    }
                        
                    });
                _menu_as->show();
            }

            pos_y += 56;
            _menu_account = new Button(this);
            {
                _menu_account->setFixedSize(218, 50);
                _menu_account->setGeometry(0, pos_y, 218, 50);
                _menu_account->setCheckable(true);
                _menu_account->setStyleSheet(ssdefines::imageToggleButton.arg(QString(":/menubtn/resource/main/menubtn/account.png"))
                    .arg(":/menubtn/resource/main/menubtn/account_hover.png")
                    .arg(":/menubtn/resource/main/menubtn/account_click.png")
                    .arg(":/menubtn/resource/main/menubtn/account.png")
                    .arg(":/menubtn/resource/main/menubtn/account_check.png"));

                connect(_menu_account, &Button::clicked, [=]() {
                    if (_menu_account->isChecked()) {
                        PLOG_INFO << "N-KeepCleaner Click Account";
                        selectViewr(viewIndex::account);
                    }

                    });
                _menu_as->show();
            }

            

            bodyLayout->addStretch(1);
        }

        {
        //contents
        auto contentswidget = new QWidget;
        bodyLayout->addWidget(contentswidget);
        contentswidget->setFixedSize(MAIN_WIDTH - 217, MAIN_HEIGTH - 50);
        initViewers(contentswidget);


        auto contentbase = new ContainerLayout(contentswidget);
        _contentswidget = new QWidget();
        _contentswidget->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/mainbg.png"));
        contentbase->addWidget(_contentswidget);
        

        auto contentsLayout = new VLayout(_contentswidget);
        contentsLayout->setContentsMargins(0, 30, 0, 0);
        
        _mainstatus = new QMovie();
        _mainstatus_gif = new QLabel(this);
        _mainstatus_gif->setFixedSize(215, 228);
        _mainstatus_gif->setMovie(_mainstatus);
        _mainstatus_gif->setAttribute(Qt::WA_NoSystemBackground);
        _mainstatus->setScaledSize(QSize(215, 228));
        //_mainstatus->start();
        _mainstatus_gif->setGeometry(505,104, 215, 228);
        _mainstatus_gif->show();

        //_main_text1 = new QLabel(this); //나쁨
        //_main_text1->setStyleSheet(ssdefines::imageWidget.arg(":/preview/resource/preview/maintext1.png"));
        _main_text1 = new CustomLabel(25, "#FD9230", true, 0, 0, this); //나쁨
        //_main_text1->setText("正常");
        _main_text1->setGeometry(580, 336, 60, 28);
        _main_text1->show();
        

        _main_text2 = new CustomLabel(16, "#FFFFFF", true, 0, 0, this); //lastupdate date
        _main_text2->setGeometry(646, 342, 200, 16);
        _main_text2->show();

        //_main_text3 = new QLabel(this); //lastupdate date
        _main_text3 = new CustomLabel(14, "#A9ABAD", false, 0,0,this); //lastupdate date
        _main_text3->setGeometry(218, 382, MAIN_WIDTH- 218, 14);
        _main_text3->setAlignment(Qt::AlignHCenter);
        _main_text3->show();
        _type = 0;


        {
            //PC자동 종료 쪽으로 기능 변경
            QString imagepath, status;
            if (AutoPcOffController::inst()->isUse()) {
                imagepath = ":/main/resource/main/bottom_speed_good.png";
                status = "オートON";
            }
            else {
                imagepath = ":/main/resource/main/bottom_speed.png";
                status = "オートOFF";
            }

            QString tooltip = QString("PCシャットダウン予約に移動");
            //シャットダウン予約
            _bottom_speed = new MainBottomContainer(imagepath, "シャットダウン予約", status, tooltip, this);
            {
                //isUse
                //_bottom_speed->setGeometry(258, 428, 139, 204);
                _bottom_speed->setGeometry(830, 428, 139, 204);
                _bottom_speed->show();

                connect(_bottom_speed, &Button::clicked, [=]() {

                    selectViewr(viewIndex::autopcoff);
                    });
            }

            const int  difday = ConfigData::inst()->getDiffDays();
            updateScanDiffStatus(difday, true);
        }


        {
            QString imagepath;
            QString hddsizse = utils::getAvaliableHardSpace();
            int state = getHddSpaceState(hddsizse);
            if (state == 0) 
                imagepath = ":/main/resource/main/bottom_space_good.png";
            else if(state == 2)
                imagepath = ":/main/resource/main/bottom_space_bad.png";
            else
                imagepath = ":/main/resource/main/bottom_space.png";
            
            QString tooltip = QString("PC状態チェックに移動");
            _bottom_space = new MainBottomContainer(imagepath, "容量管理", QString("余った容量\r\n%1").arg(hddsizse), tooltip, this);
            {
                _bottom_space->setGeometry(258, 428, 139, 204);
                //_bottom_space->setGeometry(449, 428, 139, 204);
                _bottom_space->show();

                connect(_bottom_space, &Button::clicked, [=]() {
                    selectViewr(viewIndex::optimize);
                    });
            }
        }

        {
            QString imagepath, status;
            if (SmartOptimizeController::inst()->isReserveData()) {
                imagepath = ":/main/resource/main/bottom_reserv_ok.png";
                status = "オートON";
            }
            else {
                imagepath = ":/main/resource/main/bottom_reserv.png";
                status = "オートOFF";
            }

            QString tooltip = QString("自動最適化予約に移動");
            _bottom_reserv = new MainBottomContainer(imagepath, "予約管理", status, tooltip, this);
            {
                _bottom_reserv->setGeometry(449, 428, 139, 204);
                //_bottom_reserv->setGeometry(640, 428, 139, 204);
                _bottom_reserv->show();

                connect(_bottom_reserv, &Button::clicked, [=]() {
                    selectViewr(viewIndex::smartoptimize);
                    });
            }
        }

        {
            QString tooltip = QString("ブーストPingに移動");
            _bottom_ping = new MainBottomContainer(":/main/resource/main/bottom_ping.png", "ブーストPing", "ブーストOFF", tooltip, this);
            {
                _bottom_ping->setGeometry(640, 428, 139, 204);
                //_bottom_ping->setGeometry(830, 428, 139, 204);
                _bottom_ping->show();

                connect(_bottom_ping, &Button::clicked, [=]() {
                    selectViewr(viewIndex::boostping);
                    });
            }
        }

        }

        baseLayout->addWidget(bodywidget);
    }
     
    auto showWidget = [=]() {
        activateWindow();
        showNormal();
    };

    { //tray
        auto trayicon = new QSystemTrayIcon(this);
        {
            trayicon->setIcon(QIcon(":/resource/resource/icon.ico"));
            trayicon->setToolTip("N-KeepCleaner");

            auto trayMenu = new QMenu();
            trayicon->setContextMenu(trayMenu);
            trayMenu->setStyleSheet(ssdefines::trayMenu);


            connect(trayicon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::DoubleClick)
                {
                    showWidget();
                }
                });

            auto actOpen = trayMenu->addAction(tr("実行"));
            connect(actOpen, &QAction::triggered, this, showWidget);


            auto actClose = trayMenu->addAction(tr("終了"));
            connect(actClose, &QAction::triggered, this, &QCoreApplication::quit, Qt::QueuedConnection);
            QCoreApplication::quit();

            trayicon->show();
        }

    }
    setWindowTitle("N-KeepCleanerWnd");
    setWindowIcon(QIcon(":/resource/resource/icon.png"));


    //StateEventHandler handler(this);
    //StateEventHandler* handler = new StateEventHandler(this);
    //QCoreApplication::instance()->event

    //WndEventFilter filter;
    //qApp->installNativeEventFilter(&filter);
    /*connect(&filter, &WndEventFilter::sendWindowShow, this, showWidget);*/

    {
        //auto windowTaskBarContainer = new WindowTaskBarPrograssContainer(this);
        //connect(windowTaskBarContainer, &WindowTaskBarPrograssContainer::signalShowWidget, this, showWidget);
        //auto traypopup= new TrayPopupComponent(this);
    }

    //pc off check
    //PcEndSesstionEvent shutdownEventFilter(this);
    //qApp->installNativeEventFilter(&shutdownEventFilter);
    //PcEndSesstionEvent* shutdownHandler = new PcEndSesstionEvent(this);
    //QCoreApplication::instance()->installNativeEventFilter(shutdownHandler);

    //set start up
    //if (ConfigData::inst()->getStartupOpt()) {
    //    //QString startuptmp = QString("\"%1\\NKeepCleanerStarter.exe\" /hide").arg(qApp->applicationDirPath());
    //    QString startuptmp = QString("\"%1\\NKeepCleanerStarter.exe\"").arg(qApp->applicationDirPath());
    //    QString startupstr = startuptmp.replace("/", "\\");
    //    utils::setStartup("N-KeepCleaner", startupstr, true);
    //}
        

    /*LocalServer::inst()->setServer();
    connect(LocalServer::inst(), &LocalServer::getup, this, showWidget);*/

    { //service (우선 제거)
        //QString svcname = "N-KeepCleanerSVC";

        ////NKeepClenaerSvc.exe
        //auto svcCmgr = new SvcInstaller();
        ////svcCmgr->deleteService(svcname);
        //if (!svcCmgr->isServiceRunning(svcname)) {
        //    if (!svcCmgr->isServiceInstalled(svcname)) {
        //        //QString path = qApp->applicationDirPath() + "/NKeepClenaerSvc.exe";
        //        QString binaryPath = qApp->applicationDirPath() + "/NKeepClenaerSvc.exe";
        //        if(QFile::exists(binaryPath))
        //            svcCmgr->installService(svcname, binaryPath);
        //    }

        //    svcCmgr->startService(svcname);
        //}
    }

    QObject::connect(SmartOptimizeController::inst(), &SmartOptimizeController::isUseData, [=](bool use) {

        QString imagepath, status;
        if (use) {
            imagepath = ":/main/resource/main/bottom_reserv_ok.png";
            status = "オートON";
        }
        else {
            imagepath = ":/main/resource/main/bottom_reserv.png";
            status = "オートOFF";
        }

        _bottom_reserv->setUpdate(imagepath, status);

        });

    QObject::connect(AutoPcOffController::inst(), &AutoPcOffController::changeSetting, [=](bool use) {

        QString imagepath, status;
        if (use) {
            imagepath = ":/main/resource/main/bottom_speed_good.png";
            status = "オートON";
        }
        else {
            imagepath = ":/main/resource/main/bottom_speed.png";
            status = "オートOFF";
        }


        _bottom_speed->setUpdate(imagepath, status);

        });

    //changeSetting

    QObject::connect(_boostpingview.get(), &BoostPingView::boosPingNotify, [=]() {
        QString imagepath, status;
        imagepath = ":/main/resource/main/bottom_ping_on.png";
        status = "ブーストON";
        _bottom_ping->setUpdate(imagepath, status);

        });

    QObject::connect(_boostpingview.get(), &BoostPingView::boosPingReSetNotify, [=]() {
        QString imagepath, status;
        imagepath = ":/main/resource/main/bottom_ping.png";
        status = "ブーストOFF";
        _bottom_ping->setUpdate(imagepath, status);

        });
    

    QObject::connect(ConfigData::inst(), &ConfigData::setScanDiffDay, [=]() {

        const int diffday = ConfigData::inst()->getDiffDays();
        updateScanDiffStatus(diffday);
        });
    

    { //check smart optimize pcboot
        //if (isTrayRun()) { //초기 윈도우 시작 실행시
            QObject::connect(this, SIGNAL(pconOptimizeCheck()), SmartOptimizeController::inst(), SLOT(slotCheckPcStartOptimize()));
            emit pconOptimizeCheck();
        //}
    }

    QObject::connect(this, SIGNAL(pcoffOptimizeCheck()), SmartOptimizeController::inst(), SLOT(slotCheckPcOffOptimize()));
     

    QObject::connect(this, SIGNAL(n360processCheck()), CommonController::inst(), SLOT(processcheckRun()));
    QObject::connect(CommonController::inst(), &CommonController::nortonProcessChecking, this, [=]() {
        
        const Qt::WindowFlags flags(windowFlags());
        this->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
        show();
        //this->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);

            auto alert = new AlertPopup(tr("他のプログラムをインストール中です。\r\n安全のためアプリを終了します。\r\nインストール後、再度アプリを\r\n起動してください。"), QObject::tr("ok"), "", true, window());
            connect(alert, &AlertPopup::okClicked, [=]() {
                QCoreApplication::quit(); //종료
                });
            alert->show();

        });
    
    QObject::connect(AutoChecker::inst(), &AutoChecker::sendExpreedInfo, this, [=](int days, QString date, QString rescode) {
        SetExparedInfo(days, date, rescode);

        });

    QObject::connect(AutoChecker::inst(), &AutoChecker::resUUIDUpdate, this, [=](bool success, QString rescode, QString errormsg,
        int remainDay, QString expiredDate, bool anyopentype) {

            ConfigData::inst()->setExpiredDays(remainDay);
            ConfigData::inst()->setExpiredDate(expiredDate);

        if (success) {

            //ConfigData::inst()->setUUID(utils::getSystemUUID());
            QString message;
            if (errormsg == "anyopen") { //기존 파일 삭제.
                
                QString appLocalPath = QString("%1/N-KeepCleaner").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
                QFile serialfile(QString("%1/unique").arg(appLocalPath));
                if (serialfile.exists()) {
                    serialfile.remove();
                }
                //인증 확인 완료 되었습니다.
                message = "認証が確認されました";
            }
            else {
                //신규 기기 등록 완료 되었습니다.\r\n\r\n만료까지 남은 기한 : %1\r\n만료 날짜 : %2
                message = QString("新規デバイスの登録が完了しました\r\n\r\n満了までの残りの期間:%1\r\n満了日:%2").arg(remainDay).arg(expiredDate);

                auto alert = new AlertPopup(message, QObject::tr("ok"), "", true, window());
                alert->show();
            }

            if (_authwaitingpopup) {
                _authwaitingpopup->close();
                delete _authwaitingpopup;
                _authwaitingpopup = Q_NULLPTR;
            }

        }
        else {
            //신규 기기 등록에 실패 되었습니다.\r\n다시 실행해주세요.\r\nErrorCode : %1(%2)
            auto alert = new AlertPopup(QString("新規デバイスの登録に失敗しました。\r\n再試行してください。\r\nErrorCode : %1(%2)")
                .arg(rescode).arg(errormsg), QObject::tr("ok"), "", true, window());
            alert->show();

            if (_authwaitingpopup) {
                _authwaitingpopup->close();
                delete _authwaitingpopup;
                _authwaitingpopup = Q_NULLPTR;
            }

            connect(alert, &AlertPopup::okClicked, [=]() {
                QCoreApplication::quit(); //종료
                });
        }

        });

    setClassName();
    
    hide();
    
}

QuickGrip_tmp::~QuickGrip_tmp()
{
    freePointers();
}

bool QuickGrip_tmp::eventFilter(QObject* watched, QEvent* event)
{
   /* if (watched == _mainstatus_gif) {

    }*/
    return QWidget::eventFilter(watched, event);
}

OptimizeView* QuickGrip_tmp::getOptimizeViewInstance() const
{
    if (!_optimizeview) return Q_NULLPTR;
    return _optimizeview.get();
}

SmartOptimizeView* QuickGrip_tmp::getSmartOptimizeViewInstance() const
{
    if (!_smartoptimizeView) return Q_NULLPTR;
    return _smartoptimizeView.get();
}

BoostPingView* QuickGrip_tmp::getBoostPingViewInstance() const
{
    if (!_boostpingview) return Q_NULLPTR;
    return _boostpingview.get();
}

AutoPcOffView* QuickGrip_tmp::getAutoPcOffViewInstance() const
{
    if (!_autopcoffview) return Q_NULLPTR;
    return _autopcoffview.get();
}

StartUpView* QuickGrip_tmp::getStartUpViewInstance() const
{
    if (!_startupview) return Q_NULLPTR;
    return _startupview.get();
}

AppwizView* QuickGrip_tmp::getAppwizViewInstance() const
{
    if (!_appwizview) return Q_NULLPTR;
    return _appwizview.get();
}

PcSpecView* QuickGrip_tmp::getPcSpecViewInstance() const
{
    if (!_pcspecview) return Q_NULLPTR;
    return _pcspecview.get();
}

OptionView* QuickGrip_tmp::getOptionViewInstance() const
{
    if (!_optionview) 
        return Q_NULLPTR;
    return _optionview.get();
}

SupportView* QuickGrip_tmp::getSupportViewInstance() const
{
    if (!_supportview)
        return Q_NULLPTR;
    return _supportview.get();
}

AccountView* QuickGrip_tmp::getAccountViewInstance() const
{
    if (!_accountview)
        return Q_NULLPTR;
    return _accountview.get();
}

bool QuickGrip_tmp::isTrayRun() const
{
    return _tray_run;
}

void QuickGrip_tmp::setTrayRun(bool trayrun)
{
    _tray_run = trayrun;
}

void QuickGrip_tmp::initViewers(QWidget* widget)
{
    if (!_optimizeview)
        _optimizeview = std::make_unique<OptimizeView>(218, 50, widget);
    _optimizeview->hide();

    if (!_smartoptimizeView)
        _smartoptimizeView = std::make_unique<SmartOptimizeView>(widget);
    _smartoptimizeView->hide();

    if (!_boostpingview)
        _boostpingview = std::make_unique<BoostPingView>(widget);
    _boostpingview->hide();

    if (!_autopcoffview)
        _autopcoffview = std::make_unique<AutoPcOffView>(widget);
    _autopcoffview->hide();

    //if (!_startupview)
    //    _startupview = std::make_unique<StartUpView>(widget);
    //_startupview->hide();

    //if (!_appwizview)
    //    _appwizview = std::make_unique<AppwizView>(widget);
    //_appwizview->hide();

    if (!_pcspecview)
        _pcspecview = std::make_unique<PcSpecView>(widget);
    _pcspecview->hide();

    if (!_optionview)
        _optionview = std::make_unique<OptionView>(widget);
    _optionview->hide();

    if (!_supportview)
        _supportview = std::make_unique<SupportView>(widget);
    _supportview->hide();

    if (!_accountview)
        _accountview = std::make_unique<AccountView>(widget);
    _accountview->hide();

    
}

void QuickGrip_tmp::initModels()
{
    //if (!_appwizmanager)
        //_appwizmanager = std::make_unique<AppwizManager>();
}

void QuickGrip_tmp::initControllers()
{

}

void QuickGrip_tmp::selectViewr(viewIndex index)
{
    if (getOptimizeViewInstance() == Q_NULLPTR ||
        getSmartOptimizeViewInstance() == Q_NULLPTR ||
        getBoostPingViewInstance() == Q_NULLPTR ||
        getAutoPcOffViewInstance() == Q_NULLPTR ||
        //getStartUpViewInstance() == Q_NULLPTR ||
        //getAppwizViewInstance() == Q_NULLPTR ||
        getPcSpecViewInstance() == Q_NULLPTR ||
        getOptionViewInstance() == Q_NULLPTR || 
        getSupportViewInstance() == Q_NULLPTR||
        getAccountViewInstance() == Q_NULLPTR
        )
        return;

    if (_oldselect != index) {
        enableMenuBtns(_oldselect, false);
        if(index != viewIndex::none)
            enableMenuBtns(index, true);
        _oldselect = index;
    }

    switch (index)
    {
    case viewIndex::none:
        {
            resourceView(true);
            _contentswidget->show();

            showOptimize();
            showSmartOptimize();
            showBoostPing();
            showAutoPcOff();
            showStartup();
            showAppwiz();
            showPcspec();
            showOption();
            showAS();
            showAccount();
        }
    break;
    case viewIndex::optimize:
        {
            resourceView(false);
            _contentswidget->hide();
        
            showOptimize(true);
            showSmartOptimize();
            showBoostPing();
            showAutoPcOff();
            showStartup();
            showAppwiz();
            showPcspec();
            showOption();
            showAS();
            showAccount();
        }
        break;
    case viewIndex::smartoptimize:
        {
            resourceView(false);
            _contentswidget->hide();

            showOptimize();
            showSmartOptimize(true);
            showBoostPing();
            showAutoPcOff();
            showStartup();
            showAppwiz();
            showPcspec();
            showOption();
            showAS();
            showAccount();
           
        }
        break;
    case viewIndex::boostping:
        {
            resourceView(false);
            _contentswidget->hide();

            showOptimize();
            showSmartOptimize();
            showBoostPing(true);
            showAutoPcOff();
            showStartup();
            showAppwiz();
            showPcspec();
            showOption();
            showAS();
            showAccount();
        }
        break;
    case viewIndex::autopcoff:
        {
            resourceView(false);
            _contentswidget->hide();

            showOptimize();
            showSmartOptimize();
            showBoostPing();
            showAutoPcOff(true);
            showStartup();
            showAppwiz();
            showPcspec();
            showOption();
            showAS();
            showAccount();
        }
        break;
    case viewIndex::startup:
        {
            resourceView(false);
            _contentswidget->hide();

            showOptimize();
            showSmartOptimize();
            showBoostPing();
            showAutoPcOff();
            showStartup(true);
            showAppwiz();
            showPcspec();
            showOption();
            showAS();
            showAccount();
        }
        break;
    case viewIndex::appwiz:
        {
            resourceView(false);
            _contentswidget->hide();

            showOptimize();
            showSmartOptimize();
            showBoostPing();
            showAutoPcOff();
            showStartup();
            showAppwiz(true);
            showPcspec();
            showOption();
            showAS();
            showAccount();
        }
        break;
    case viewIndex::pcspec:
        {
            resourceView(false);
            _contentswidget->hide();

            showOptimize();
            showSmartOptimize();
            showBoostPing();
            showAutoPcOff();
            showStartup();
            showAppwiz();
            showPcspec(true);
            showOption();
            showAS();
            showAccount();
        }
        break;
    case viewIndex::optins:
        {
            resourceView(false);
            _contentswidget->hide();

            showOptimize();
            showSmartOptimize();
            showBoostPing();
            showAutoPcOff();
            showStartup();
            showAppwiz();
            showPcspec();
            showOption(true);
            showAS();
            showAccount();

        }
        break;
    case viewIndex::as:
    {
        resourceView(false);
        _contentswidget->hide();

        showOptimize();
        showSmartOptimize();
        showBoostPing();
        showAutoPcOff();
        showStartup();
        showAppwiz();
        showPcspec();
        showOption();
        showAS(true);
        showAccount();
    }
    break;
    case viewIndex::account:
    {
        resourceView(false);
        _contentswidget->hide();

        showOptimize();
        showSmartOptimize();
        showBoostPing();
        showAutoPcOff();
        showStartup();
        showAppwiz();
        showPcspec();
        showOption();
        showAS();
        showAccount(true);

    }
    break;

    default:
        break;
    }

}

void QuickGrip_tmp::enableMenuBtns(viewIndex index, bool disable)
{
    switch (index)
    {
    case viewIndex::none:
        break;
    case viewIndex::optimize:
        _menu_opti->setDisabled(disable);
        break;
    case viewIndex::smartoptimize:
        _menu_smartopti->setDisabled(disable);
        break;
    case viewIndex::boostping:
        _menu_boostping->setDisabled(disable);
        break;
    case viewIndex::autopcoff:
        _menu_pcoff->setDisabled(disable);
        break;
    //case viewIndex::startup:
    //    _menu_startup->setDisabled(disable);
    //    break;
    //case viewIndex::appwiz:
    //    _menu_appwiz->setDisabled(disable);
    //    break;
    case viewIndex::pcspec:
        _menu_pcspec->setDisabled(disable);
        break;
    case viewIndex::optins:
        _menu_option->setDisabled(disable);
        break;
    case viewIndex::as:
        _menu_as->setDisabled(disable);
        break;
    case viewIndex::account:
        _menu_account->setDisabled(disable);
        break;
    default:
        break;
    }
}

void QuickGrip_tmp::selectBtnType(viewIndex index)
{
    selectViewr(index);
}

void QuickGrip_tmp::setTrayIcon()
{
   
}

void QuickGrip_tmp::resourceView(bool show)
{
    if (show) {
        _mainstatus_gif->show();
        _main_text1->show();
        _main_text2->show();
        _main_text3->show();

        _bottom_speed->show();
        _bottom_space->show();
        _bottom_reserv->show();
        _bottom_ping->show();
        _back_button->hide();
    }
    else {
        _mainstatus_gif->hide();
        _main_text1->hide();
        _main_text2->hide();
        _main_text3->hide();

        _bottom_speed->hide();
        _bottom_space->hide();
        _bottom_reserv->hide();
        _bottom_ping->hide();
        _back_button->show();
    }
}


void QuickGrip_tmp::freePointers()
{
    _contentswidget->deleteLater();
    _mainstatus_gif->deleteLater();
    _main_text1->deleteLater();
    _main_text2->deleteLater();
    _main_text3->deleteLater();
    _mainstatus_level->deleteLater();
    _mainstatus_lastexecute->deleteLater();
    _lastrun_label->deleteLater();
    _space_label->deleteLater();
    _reserve_label->deleteLater();
    _bottom_speed->deleteLater();
    _bottom_space->deleteLater();
    _bottom_reserv->deleteLater();
    _bottom_ping->deleteLater();
}

/*
* return value : good(0), nomal(1), bad(2)
*/
int QuickGrip_tmp::getHddSpaceState(QString space)
{
    if (space.isEmpty())
        return 2;

    if (space.indexOf("TB") > 0)
        return 0;

    if (space.indexOf("GB") > 0) {
        QString GB = space.left(space.indexOf("GB"));
        if (GB.isEmpty())
            return 2;

        int size = (int)GB.toFloat();
        if (size < 30)
            return 2;
        else if (size > 300)
            return 0;
        else return 1;
    }

    return 2;
}

void QuickGrip_tmp::updateScanDiffStatus(int diffday, bool firstrun)
{
    //QString("%1/image/main_status_good.gif").arg(qApp->applicationDirPath());
    QString imagepath, gifpath;
    QString difdaytext;
    if (diffday < 0) {
        difdaytext = "最新化履歴なし";
        imagepath = ":/main/resource/main/bottom_speed_bad.png";
        gifpath = QString("%1/image/main_status_bad.gif").arg(qApp->applicationDirPath());

        _main_text1->setColor("#FF6868");
        _main_text1->setText("悪い");
        _main_text3->setText(QString("Last Scan : %1 ").arg(difdaytext));
    }
    else {

        if(diffday == 0)
            difdaytext = "本日最適化";
        else {
            difdaytext = QString("最適化から %1日").arg(diffday);
        }
            

        if (diffday <= 7) {
            imagepath = ":/main/resource/main/bottom_speed_good.png";
            gifpath = QString("%1/image/main_status_good.gif").arg(qApp->applicationDirPath());

            _main_text1->setColor("#FFD968");
            _main_text1->setText("良い");
        }
        else if (diffday >= 31) {
            imagepath = ":/main/resource/main/bottom_speed_bad.png";
            gifpath = QString("%1/image/main_status_bad.gif").arg(qApp->applicationDirPath());

            _main_text1->setColor("#FF6868");
            _main_text1->setText("悪い");
        }
        else {
            imagepath = ":/main/resource/main/bottom_speed.png";
            gifpath = QString("%1/image/main_status.gif").arg(qApp->applicationDirPath());

            _main_text1->setColor("#FD9230");
            _main_text1->setText("普通");

        }
        _main_text3->setText(QString("Last Scan : %1 %2").arg(ConfigData::inst()->getScanDate()).arg(ConfigData::inst()->getScanTime()));

        _main_text2->setText(difdaytext);
    }

    //最適化から 1日
    //本日最適化

    //_bottom_speed->setUpdate(imagepath, difdaytext);
    
    //gif
    if (!firstrun)
        _mainstatus->stop();
    _mainstatus->setFileName(gifpath);
    _mainstatus->start();

    //last scan label
    //_main_text3->setText("Last Scan : 2023.03.22  14:00:21");
    
}

void QuickGrip_tmp::setClassName()
{
    QMainWindow w;
    // Qt 윈도우 핸들 가져오기
    HWND hwnd = reinterpret_cast<HWND>(w.winId());

    // 윈도우 클래스 이름 설정
    const char* className = "NKeepCleanerCls";
    SetClassLongPtrA(hwnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(className));
}

void QuickGrip_tmp::trayoptioni(bool tray)
{
    _tray_option = tray;
}

bool QuickGrip_tmp::isCheckPcOffOptimize()
{
    if (SmartOptimizeController::inst()->getReservePcOff() == true) {
        showNormal();
        OptimizeController::inst()->setScanDate();
        return true;
    }
    else return false;

}


void QuickGrip_tmp::checkPcOffOptimize()
{
        emit pcoffOptimizeCheck();

}

void QuickGrip_tmp::setGotoShow()
{
    const Qt::WindowFlags flags(windowFlags());
    this->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    activateWindow();
    show();


    this->setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    show();

    //showNormal();
    //show();

}

void QuickGrip_tmp::setAuthCheck()
{
    AutoChecker::inst()->startAuthCheck();
}

void QuickGrip_tmp::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) 
    {
        if (windowState() & Qt::WindowMinimized) {
            qDebug() << "Window minimized";


        }
    }
    QMainWindow::changeEvent(event);
}

void QuickGrip_tmp::SetExparedInfo(int days, QString date, QString rescode)
{

    AuthResType type = resCodeToRestype(rescode);

#ifdef DEBUG
    //type = AuthResType::expiredkey;
#endif // DEBUG

    switch (type)
    {

    case AuthResType::ok:
        {
            ConfigData::inst()->setExpiredDays(days);
            ConfigData::inst()->setExpiredDate(date);

            emit AccountController::inst()->AuthInfoUpdate();
            //등록된 시리얼 코드 : %1\r\n만료까지 남은 기한 : %2\r\n만료 날짜: %3
            auto alert = new AlertPopup(QString("登録済みのシリアル番号 : %1\r\n 満了までの残りの期間 : %2\r\n満了日: %3")
                .arg(ConfigData::inst()->getSerialKey())
                .arg(ConfigData::inst()->getExpiredDays())
                .arg(ConfigData::inst()->getExpiredDate()), QObject::tr("ok"), "", true, window());
            alert->show();
        }
        break;
    case AuthResType::notfindkey: //001
        { //호출 param 자체가 잘못된 경우. 우리쪽 서버 문제일수도..우선 사용하도록 함. 
        }
        break;
    case AuthResType::serialerror: //003
        {

        //기존 등록된 인증키가 잘못된 경우 -> 기존 인증 파일 삭제 후 재인증 시킨다.
        auto alert = new AuthSerialKeyResetPopup(ConfigData::inst()->getSerialKey(), window());

        connect(alert, &AuthSerialKeyResetPopup::okClicked, [=]() {
            //인증파일 삭제
            ConfigData::inst()->clearAuthinfo();

            // 현재 애플리케이션의 파일 경로
            QString appPath = QApplication::applicationFilePath();
            QStringList args = QApplication::arguments();

            // 첫 번째 인자(실행 파일 경로) 제거
            args.removeFirst();

            // 새 프로세스 시작
            QProcess::startDetached(appPath, args);

            // 현재 애플리케이션 종료
            QCoreApplication::quit(); //종료

            });

        connect(alert, &AuthSerialKeyResetPopup::cancelClicked, [=]() {
            QCoreApplication::quit(); //종료
            return;
            });

        alert->show();
        }
        break;
    case AuthResType::expiredkey: //004
        {
        //만료된 경우
        auto alert = new AuthExpiredKeyPopup(ConfigData::inst()->getSerialKey(), window());

        connect(alert, &AuthExpiredKeyPopup::callBilling, [=]() {
           
            QCoreApplication::quit(); //종료

            });

        connect(alert, &AuthExpiredKeyPopup::reStart, [=]() {
            //인증파일 삭제
            ConfigData::inst()->clearAuthinfo();

            // 현재 애플리케이션의 파일 경로
            QString appPath = QApplication::applicationFilePath();
            QStringList args = QApplication::arguments();

            // 첫 번째 인자(실행 파일 경로) 제거
            args.removeFirst();

            // 새 프로세스 시작
            QProcess::startDetached(appPath, args);

            //현재 애플리케이션 종료

            QCoreApplication::quit(); //종료
            return;
            });

        connect(alert, &AuthExpiredKeyPopup::cancelClicked, [=]() {
            QCoreApplication::quit(); //종료
            return;
            });

        alert->show();
        }
    break;
    case AuthResType::uuiderror: //009 UUID 이슈
        {
            
            if (ConfigData::inst()->getOldFileUUID() == "anyopen") { //old version
                
                AutoChecker::inst()->sendUUIDUpdate(date/*id*/, true);

                emit callWaitPopup(true);

                }
            else {
                //이미 등록된 기기가 있습니다. 기존 기기 정보를 제거하고 현재의 기기로 활성화 할까요?\r\n(#가장 먼저 등록된 기기 정보 부터 제거 됩니다.)
                auto alert = new AlertPopup(tr("既に登録されているデバイスがあります。既存のデバイス情報を消去し、現在のデバイスでアクティベーションしますか？\r\n(#デバイス情報は登録された順番に消去されます)"), QObject::tr("削除"), QObject::tr("cancel"), true, window());
                connect(alert, &AlertPopup::okClicked, [=]() {
                    AutoChecker::inst()->sendUUIDUpdate(date/*id*/);

                    emit callWaitPopup(false);
                    });

                connect(alert, &AlertPopup::cancelClicked, [=]() {
                    QCoreApplication::quit(); //종료
                    return;
                    });

                alert->show();
            }
        }
    break;

    default:
        {
        //프로그램 인증에 실패되었습니다. \r\n 다시 시도해주세요.
            auto alert = new AlertPopup(QString("プログラム認証に失敗しました。\r\n再試行してください"), QObject::tr("ok"), "", true, window());
            alert->show();
            connect(alert, &AlertPopup::cancelClicked, [=]() {
                QCoreApplication::quit(); //종료
                return;
                });

        }
        break;
    }
}

void QuickGrip_tmp::callWaitPopup(bool anyopen)
{
    if (_authwaitingpopup) {
        _authwaitingpopup->close();
        delete _authwaitingpopup;
    }


    _authwaitingpopup = new AuthWaitingPopup(anyopen, this);
    _authwaitingpopup->exec();
}

void QuickGrip_tmp::showOptimize(bool view)
{
    _menu_opti->setChecked(view);

    if (view) 
        getOptimizeViewInstance()->show();
    else
        getOptimizeViewInstance()->hide();
}

void QuickGrip_tmp::showSmartOptimize(bool view)
{
    _menu_smartopti->setChecked(view);

    if (view)
        getSmartOptimizeViewInstance()->show();
    else
        getSmartOptimizeViewInstance()->hide();
}

void QuickGrip_tmp::showBoostPing(bool view)
{
    _menu_boostping->setChecked(view);

    if (view)
        getBoostPingViewInstance()->show();
    else
        getBoostPingViewInstance()->hide();
}

void QuickGrip_tmp::showAutoPcOff(bool view)
{
    _menu_pcoff->setChecked(view);

    if (view)
        getAutoPcOffViewInstance()->show();
    else
        getAutoPcOffViewInstance()->hide();
}

void QuickGrip_tmp::showStartup(bool view)
{
    //_menu_startup->setChecked(view);

    //if (view)
    //    getStartUpViewInstance()->show();
    //else
    //    getStartUpViewInstance()->hide();
}

void QuickGrip_tmp::showAppwiz(bool view)
{
    //_menu_appwiz->setChecked(view);

    //if (view)
    //    getAppwizViewInstance()->show();
    //else
    //    getAppwizViewInstance()->hide();
}

void QuickGrip_tmp::showPcspec(bool view)
{
    _menu_pcspec->setChecked(view);

    if (view)
        getPcSpecViewInstance()->show();
    else
        getPcSpecViewInstance()->hide();
}

void QuickGrip_tmp::showOption(bool view)
{
    _menu_option->setChecked(view);

    if (view)
        getOptionViewInstance()->show();
    else
        getOptionViewInstance()->hide();
}

void QuickGrip_tmp::showAS(bool view)
{
    _menu_as->setChecked(view);

    if (view)
        getSupportViewInstance()->show();
    else
        getSupportViewInstance()->hide();
}

void QuickGrip_tmp::showAccount(bool view)
{
    _menu_account->setChecked(view);

    if (view)
        getAccountViewInstance()->show();
    else
        getAccountViewInstance()->hide();
}


//void QuickGrip_tmp::callProcessCheckAlert()
//{
//    auto alert = new AlertPopup(tr("他のプログラムをインストール中です。安全のためアプリを終了します。インストール後、再度アプリを起動してください。"), QObject::tr("ok"), "", false, window());
//    connect(alert, &AlertPopup::okClicked, [=]() {
//        QCoreApplication::quit(); //종료
//        });
//    alert->show();
//}
//


