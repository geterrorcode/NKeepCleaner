#include "boostpingview.h"

#include <QTimer>

#include "view/Component/containerlayout.h"
#include "view/Component/button.h"
#include "view/common/alertpopup.h"

#include "../../controller/boostping/boostpingcontroller.h"

#include "../../ui.h"
#include "../../common/ui/uiutils.h"
#include "../../common/defines/ssdefins.h"

BoostPingView::BoostPingView(QWidget* parent)
    : QWidget(parent), _status_label(Q_NULLPTR), _boostping_gif(Q_NULLPTR), _first(true)
    , _restartAfterBeforePing(false), _boostRunning(false)
    , _boost(Q_NULLPTR), _unboost(Q_NULLPTR), _unboostlabel(Q_NULLPTR)
{
    //_pingcheck = new PingChecker(this);

    int marginsize_x = 218;
    int marginsize_y = 50;

    auto baseLayout = new ContainerLayout(this);
    auto backgroundFrame = new QFrame;
    backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/boostping/resource/boostping/boostping_before.png"));
    baseLayout->addWidget(backgroundFrame);

    //ping speed
    auto beforeping = new CustomLabel(18, "#FFD968", false, 0, 0, this);
    //beforeping->setText("(前)ネットワーク　反応時間");
    beforeping->setText("ネットワーク反応時間(前)");
    beforeping->setGeometry(250- marginsize_x, 100, 250, 30);

    auto beforepingvalue = new CustomLabel(16, "#FFD968", false, 0, 0, this);
    beforepingvalue->setText(": 速度チェック中...");
    beforepingvalue->setGeometry(260 - marginsize_x, 140, 200, 30);

    auto afterping = new CustomLabel(18, "#FFD968", false, 0, 0, this);
    //afterping->setText("Network反応時間(後)");
    afterping->setText("ネットワーク反応時間(後)");
    afterping->setGeometry(790 - marginsize_x, 100, 250, 30);

    auto afterpingvalue = new CustomLabel(16, "#FFD968", false, 0, 0, this);
    afterpingvalue->setText(": ブーストPing　適用前");
    afterpingvalue->setGeometry(800 - marginsize_x, 140, 200, 30);
    //=================================================
    auto descping = new CustomLabel(20, "#FFFFFF", true, 0, 40, this);
    //数値が小さいほど
    descping->setText(QString("ネットワーク反応時間の<font color=#FDBB30>%1</font>、ラグのない速い状態を意味します").arg("数値が小さいほど"));
    descping->setAlignment(Qt::AlignHCenter);
    //네트웍반응시간이 작은 값일 수록 PC의 랙이없는 빠른 상태 입니다.
    descping->setGeometry(0, 20, 1008-217, 40);


    QString gifpath = QString("%1/image/boostping.gif").arg(qApp->applicationDirPath());
    QMovie* movie = new QMovie(gifpath);
    if (!movie->isValid()) {
        qDebug() << "gif error";
    }

    _boostping_gif = new QLabel(this);
    _boostping_gif->setFixedSize(215, 228);
    _boostping_gif->setAttribute(Qt::WA_NoSystemBackground);
    _boostping_gif->setMovie(movie);
    movie->setScaledSize(QSize(215, 228));
    movie->start();
    _boostping_gif->setGeometry(505 - marginsize_x, 178 - marginsize_y, 215, 228);
    _boostping_gif->show();

    auto progress = new QProgressBar(this);
    progress->setStyleSheet("QProgressBar {background:#202224; border:none; border-radius:8px; color: white;}"
        "QProgressBar::chunk {background-color: #FDBB30; border-radius:8px;}");
    progress->setGeometry(931 - marginsize_x - 619, 476 - marginsize_y, 619, 19);
    progress->setTextVisible(true);
    progress->setAlignment(Qt::AlignCenter);
    progress->hide();

    _unboostlabel = new CustomLabel(13, "#DDDDDD", true, 0, 40, this);
    _unboostlabel->setText("#ブーストPingの設定後に問題があれば、設定を初期化してください");//부스트핑 설정 후 문제 있으면 설정 초기화 해주세요.
    _unboostlabel->setAlignment(Qt::AlignLeft);
    _unboostlabel->hide();
    _unboostlabel->setGeometry(150, 618 - marginsize_y, 600, 35);

    _boost = new Button(this);
    QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton{font-weight:bold;}");
    _boost->setStyleSheet(ss);
    _boost->setText("オン");
    _boost->show();
    _boost->setGeometry(537 - marginsize_x, 436 - marginsize_y, 153, 40);
    connect(_boost, &Button::clicked, [=]() {

        if (_boostRunning)
            return;

        _unboost->hide();
        _unboostlabel->hide();

        movie->stop();

        QString gifpath = QString("%1/image/boostping_ing.gif").arg(qApp->applicationDirPath());
        movie->setFileName(gifpath);
        movie->start();

        _boost->hide();

        progress->setValue(0);
        progress->show();

        _status_label->setText("");

        if (!_first) {
            auto alert = new AlertPopup(tr("再度ブーストPingを実行する際に\r\n実行前のネットワーク反応時間を再度測定します。\r\nよって、数値の変動がある場合があります"), QObject::tr("確認"), QObject::tr(""), true, window());
            connect(alert, &AlertPopup::okClicked, [=]() {
                backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/boostping/resource/boostping/boostping_before.png"));
                beforepingvalue->setText(QString(": 速度チェック中..."));//속도 체크 중
                afterpingvalue->setText(QString(": 速度チェック中..."));
                _restartAfterBeforePing = true;
                emit getBeforePingSpeed();
                });

            alert->show();
                       
        }
        else {
            _boostRunning = true;
            emit boostPingOn();
        }


        });

    _status_label = new CustomLabel(15, "#ffffff", true, 0, 40, this);
    //클릭 한 번으로 인터넷 속도를 향상시키고 쾌적한 PC 환경을 유지하세요.
    _status_label->setText("ワンクリックでインターネット速度を向上させ、快適なPCで環境を維持します");
    _status_label->setAlignment(Qt::AlignCenter);
    _status_label->setGeometry(50, 488 - marginsize_y, 700, 24);
    _status_label->show();



    _unboost = new Button(this);
    QString unboost_ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/refreshbtn.png")).arg("QPushButton{font-weight:bold; color: #DDDDDD;}");
    _unboost->setStyleSheet(unboost_ss);
    _unboost->setText("設定の初期化");//설정 초기화
    _unboost->hide();
    _unboost->setGeometry(20, 608 - marginsize_y, 123, 35);
    connect(_unboost, &Button::clicked, [=]() {
        //부스트 핑의 설정된 모든값이 해제 초기화 됩니다. \r\n설정 초기화 진행 할까요?
        auto alert = new AlertPopup(tr("ブーストピングの設定された全ての値が初期化されます。\r\n設定の初期化をしますか？"), QObject::tr("確認"), QObject::tr("cancel"), true, window());
        alert->show();
        connect(alert, &AlertPopup::okClicked, [=]() {
            //_boost->setDisabled(true);
            movie->stop();

            QString gifpath = QString("%1/image/boostping_ing.gif").arg(qApp->applicationDirPath());
            movie->setFileName(gifpath);
            movie->start();

            _boost->hide();
            _unboost->hide();
            _unboostlabel->hide();

            progress->setValue(0);
            progress->show();

            emit startReSetBoostPingTimer();
            });

        });


    setFixedSize(parent->size());

    //ping speed
    QObject::connect(this, SIGNAL(getBeforePingSpeed()), BoostPingController::inst(), SLOT(slotBeforePingCheck()));

    QObject::connect(BoostPingController::inst(), &BoostPingController::signalSetBeforePingSpeed, [=](int speed) {
        beforepingvalue->setText(QString(": %1 ms").arg(speed));

        if (_restartAfterBeforePing && !_boostRunning) {
            _restartAfterBeforePing = false;
            _boostRunning = true;
            emit boostPingOn();
        }
        });

    QObject::connect(BoostPingController::inst(), &BoostPingController::signalSetAfterPingSpeed, [=](int speed) {
        if (speed <= 0) {
            afterpingvalue->setText(QString(": 速度チェック中..."));//속도 체크 중
        }
        else {
            afterpingvalue->setText(QString(": %1 ms").arg(speed));
            //boost->setText("再試行");
            //boost->setEnabled(true);
        }

        });
    emit getBeforePingSpeed();

    QObject::connect(this, SIGNAL(boostPingOn()), BoostPingController::inst(), SLOT(slotBoostPing()));
    QObject::connect(BoostPingController::inst(), SIGNAL(signalStatusUpdate(QString)), this, SLOT(updateStatusMsg(QString)));
    QObject::connect(BoostPingController::inst(), &BoostPingController::signalboostProgress, [=](int per) {
        if (per < 0)
            per = 0;

        if (per > 100)
            per = 100;

        progress->setValue(per);

        });

    QObject::connect(this, SIGNAL(startBoostPingTimer(int)), BoostPingController::inst(), SLOT(slotBoostPingStep(int)));
    QObject::connect(BoostPingController::inst(), &BoostPingController::signalInternetOffAlert, this, [this]() {
        auto alert = new AlertPopup(tr("ブーストPing設定が完了しました。\r\n正常に適用するためネットワークの\r\n自動再接続を推奨します。\r\n一時的にインターネットが途切れますので\r\n作業中のものがありましたら保存後\r\n再接続を行ってください。\r\n\r\n再接続を行いますか？"), QObject::tr("再接続"), QObject::tr("cancel"), true, window());
            connect(alert, &AlertPopup::okClicked, this, [this]() {
                BoostPingController::inst()->setStepCount(9);
                emit startBoostPingTimer(9);
                });

            connect(alert, &AlertPopup::cancelClicked, this, [this]() {
                BoostPingController::inst()->setStepCount(13);
                emit startBoostPingTimer(13);
                });

            alert->show();


        });


    QObject::connect(BoostPingController::inst(), &BoostPingController::signalboostEnd, [=]() {

        _boostRunning = false;
        _restartAfterBeforePing = false;

        backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/boostping/resource/boostping/boostping_after.png"));

        movie->stop();

        QString gifpath = QString("%1/image/boostping_end.gif").arg(qApp->applicationDirPath());
        movie->setFileName(gifpath);
        movie->start();
        
        progress->hide();

        //boost->setText("再試行");
        _boost->setText("チェック中");
        _boost->setEnabled(false);
        //_boost->setGeometry(537 - marginsize_x, 566 - marginsize_y, 153, 40);
        _boost->setGeometry(537 - marginsize_x, 546 - marginsize_y, 153, 40);
        _boost->show();

       

        //main 에 전송
        emit boosPingNotify();

        checktimer();

        _first = false;
        });


    QObject::connect(this, SIGNAL(startReSetBoostPingTimer()), BoostPingController::inst(), SLOT(slotUnSetBoostPingStep()));
    QObject::connect(BoostPingController::inst(), &BoostPingController::signalboostReSetEnd, [=]() {

        backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/boostping/resource/boostping/boostping_before.png"));

        movie->stop();

        QString gifpath = QString("%1/image/boostping.gif").arg(qApp->applicationDirPath());
        movie->setFileName(gifpath);
        movie->start();

        progress->hide();

        _boost->setText("再試行");
        _boost->setEnabled(true);
        _boost->setGeometry(537 - marginsize_x, 546 - marginsize_y, 153, 40);
        _boost->show();

        _unboost->hide();
        _unboostlabel->hide();

        //main 에 전송
        emit boosPingReSetNotify();
        
        });

}

BoostPingView::~BoostPingView()
{

}


void BoostPingView::updateStatusMsg(QString msg)
{
    if (_status_label) {
        _status_label->setText(msg);
        //_status_label->update();
    }
}


void BoostPingView::checktimer()
{
    auto timer = new QTimer(this);
    timer->setInterval(300);
    timer->setTimerType(Qt::VeryCoarseTimer);
    connect(timer, &QTimer::timeout, this, [=]()
        {
            if (BoostPingController::inst()->lastpingcheck() == true)
            {
                timer->stop();

                _boost->setText("再試行");
                _boost->setEnabled(true);
                
                //unboost show
                _unboost->show();
                _unboostlabel->show();
                
                return;
            }
        });
    timer->start();

}
