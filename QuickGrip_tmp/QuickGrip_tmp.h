#pragma once

#include <QtWidgets/QMainWindow>
#include <QPointer>
#include <QMouseEvent>

#include <QEvent>
#include <QDebug>

#include "view/optimize/optimizeview.h"
#include "view/smart_optimize/smartoptimizeview.h"
#include "view/boostping/boostpingview.h"
#include "view/auto_pcoff/autopcoffview.h"
#include "view/startup/startupview.h"
#include "view/appwiz/appwizview.h"
#include "view/pcspec/pcspecview.h"
#include "view/option/optionview.h"
#include "view/as/supportview.h"
#include "view/account/accountview.h"
#include "view/main/serialkeyinputwidget.h"
#include "view/main/authwaitingpopup.h"


#include "model/startup/StartupSearcher.h"
#include "model/appwiz/appwizmanager.h"
#include "model/configdata/configdata.h"

#include "controller/smart_optimize/smartoptimizecontroller.h"

#include "view/main/mainbottomcontainer.h"
#include "view/Component/button.h"
#include "view/Component/customlabel.h"
#include "view/Container/backbuttonwidget.h"

//#include "src/common/tool/ProcessDetector.h"
#include "src/common/defines/commondefine.h"
#include "src/common/ux/singleton.h"
//#include "ui_QuickGrip_tmp.h"


class QuickGrip_tmp : public QMainWindow, public common::ux::Singleton<QuickGrip_tmp>
{
    Q_OBJECT

public:
    QuickGrip_tmp(QWidget *parent = nullptr);
    ~QuickGrip_tmp();

    bool eventFilter(QObject* watched, QEvent* event);

    bool _tray_run;
private:

    //friend QuickGrip_tmp& common::ux::Singleton<QuickGrip_tmp>::inst();
    
private:
    //Ui::QuickGrip_tmpClass ui;
    bool _tray_option;
    bool _firstClick_startup;
    bool _firstClick_appwiz;
    int _type;

    viewIndex _oldselect;
    
    QPointer<QWidget> _contentswidget;

    QPointer<QMovie> _mainstatus;

    QPointer<QLabel> _mainstatus_gif;
    QPointer<CustomLabel> _main_text1;
    QPointer<CustomLabel> _main_text2;
    QPointer<CustomLabel> _main_text3;

    QPointer<CustomLabel> _mainstatus_level;
    QPointer<CustomLabel> _mainstatus_lastexecute;

    QPointer<CustomLabel> _lastrun_label;
    QPointer<CustomLabel> _space_label;
    QPointer<CustomLabel> _reserve_label;

    QPointer<BackButtonWidget> _back_button;


    QPointer<Button> _menu_opti;
    QPointer<Button> _menu_smartopti;
    QPointer<Button> _menu_boostping;
    QPointer<Button> _menu_pcoff;
    QPointer<Button> _menu_startup;
    QPointer<Button> _menu_appwiz;
    QPointer<Button> _menu_pcspec;
    QPointer<Button> _menu_option;
    QPointer<Button> _menu_as;
    QPointer<Button> _menu_account;


    /*QPointer<Button> _bottom_speed;
    QPointer<Button> _bottom_space;
    QPointer<Button> _bottom_reserv;
    QPointer<Button> _bottom_ping;*/

    QPointer<MainBottomContainer> _bottom_speed;
    QPointer<MainBottomContainer> _bottom_space;
    QPointer<MainBottomContainer> _bottom_reserv;
    QPointer<MainBottomContainer> _bottom_ping;
    
    
    std::unique_ptr<OptimizeView> _optimizeview;
    std::unique_ptr<SmartOptimizeView> _smartoptimizeView;
    std::unique_ptr<BoostPingView> _boostpingview;
    std::unique_ptr<AutoPcOffView> _autopcoffview;
    std::unique_ptr<StartUpView> _startupview;
    std::unique_ptr<AppwizView> _appwizview;
    std::unique_ptr<PcSpecView> _pcspecview;
    std::unique_ptr<OptionView> _optionview;
    std::unique_ptr<SupportView> _supportview;
    std::unique_ptr<AccountView> _accountview;


    //UUID Udpate Waiting Popup
    QPointer<AuthWaitingPopup> _authwaitingpopup;
    

    //QPointer<ProcessDetector> _processchecker;
    
    //std::unique_ptr<StartupController> _startup_controller;

protected:
    void changeEvent(QEvent* event) Q_DECL_OVERRIDE;

private:
    void initViewers(QWidget *widget);
    void initModels();
    void initControllers();
    void selectViewr(viewIndex index);
    void enableMenuBtns(viewIndex index, bool disable);

    void setTrayIcon();

    void resourceView(bool show);

    void freePointers();

    int getHddSpaceState(QString space);
    void updateScanDiffStatus(int diffday, bool firstrun = false);

    void setClassName();

    void SetExparedInfo(int days, QString date, QString rescode);

    void showOptimize(bool view = false);
    void showSmartOptimize(bool view = false);
    void showBoostPing(bool view = false);
    void showAutoPcOff(bool view = false);
    void showStartup(bool view = false);
    void showAppwiz(bool view = false);
    void showPcspec(bool view = false);
    void showOption(bool view = false);
    void showAS(bool view = false);
    void showAccount(bool view = false);

public:
    QString getCurrentPath() {
        return qApp->applicationDirPath();
    }

    OptimizeView* getOptimizeViewInstance() const;
    SmartOptimizeView* getSmartOptimizeViewInstance() const;
    BoostPingView* getBoostPingViewInstance() const;
    AutoPcOffView* getAutoPcOffViewInstance() const;
    StartUpView* getStartUpViewInstance() const;
    AppwizView* getAppwizViewInstance() const;
    PcSpecView* getPcSpecViewInstance() const;
    OptionView* getOptionViewInstance() const;
    SupportView* getSupportViewInstance() const;
    AccountView* getAccountViewInstance() const;

    bool isTrayRun() const;
    void setTrayRun(bool trayrun);

    void checkPcOffOptimize();
    bool isCheckPcOffOptimize();

    void setGotoShow();

    //void setAuthCheck();

signals:
    void signal_getStartup();
    void signalgetStartup();
    //StartupController* getStartupControllerInstance() const;

    void pconOptimizeCheck();
    void pcoffOptimizeCheck();

    void n360processCheck();

public slots:
    void selectBtnType(viewIndex);
    void trayoptioni(bool tray);

    void callWaitPopup(bool anyopen);

    void setAuthCheck();
    //void callProcessCheckAlert();
    
};

//inline const QuickGrip_tmp* getMainApp() { return &QuickGrip_tmp::cinst(); }
