

#include "smartoptimizeview.h"

#include "pconoffcontainer.h"
#include "setschedulecontainer.h"
#include "schedulecontainer.h"

#include "view/Component/button.h"
#include "view/common/alertpopup.h"
#include "../Container/topinfowidget.h"

#include "../../controller/smart_optimize/smartoptimizecontroller.h"


#include "../../ui.h"
#include "../../common/defines/ssdefins.h"



SmartOptimizeView::SmartOptimizeView(QWidget *parent)
	: QWidget(parent), _listempty_label(Q_NULLPTR)
{
    _reserve_lists.clear();
    list_pos.clear();

    int marginsize_x = 218;
    int marginsize_y = 50;

    auto baseLayout = new ContainerLayout(this);
    {
        auto backgroundFrame = new QFrame;
        backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/mainbg.png"));
        baseLayout->addWidget(backgroundFrame);
    }
	
    auto topbase = new QLabel(this);
    topbase->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/top.png"));
    topbase->setGeometry(258 - marginsize_x, 76 - marginsize_y, 710, 74);
    topbase->show();

    auto toplabel = new CustomLabel(20, "#FFFFFF", true, 0, 0, this);
    toplabel->setGeometry(258 - marginsize_x, 98 - marginsize_y, 710, 24);
    toplabel->setAlignment(Qt::AlignCenter);
    //toplabel->setText(QString("ユーザーが<font color=#FDBB30>%1</font>を進めます。").arg("希望する時間に自動的に最適化"));
    toplabel->setText(QString("<font color=#FDBB30>%1</font>に自動で最適化を行います").arg("設定した時間"));
    toplabel->show();

    auto pconoff_container = new PcOnOffContainer(SmartOptimizeController::inst()->getReservePcOn(), 
        SmartOptimizeController::inst()->getReservePcOff(), this);
    pconoff_container->setGeometry(258 - marginsize_x, 175 - marginsize_y, 465, 150);
    pconoff_container->show();

    auto setschedule_container = new SetScheduleContainer(this);
    setschedule_container->setGeometry(258 - marginsize_x, 350 - marginsize_y, 465, 266);
    setschedule_container->show();

    auto desc = new CustomLabel(15, "#FFFFFF", false, 0, 50, this);
    desc->setText("※各項目につき1つのみ設定可能です。\r\n    例)PCシャットダウン時に最適化・毎日12時・毎週(火)12時・毎月末12時");
    desc->setGeometry(258 - marginsize_x, 606 - marginsize_y, 465, 50);
    desc->setWordWrap(true);
    desc->show();


    auto reserv = new QLabel(this);
    reserv->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/gbreserv.png"));
    //reserv->setGeometry(748 - marginsize_x, 175 - marginsize_y, 220, 376);
    reserv->setGeometry(748 - marginsize_x, 195 - marginsize_y, 220, 376);
    reserv->show();


    //設定されたリスト
    //auto listtext = new QLabel(this);
    //listtext->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/listtext.png"));
    auto listtext = new CustomLabel(20,"#FFFFFF", true, 0, 0, this);
    listtext->setText("設定済みのリスト");
    listtext->setGeometry(768- marginsize_x, 205 - marginsize_y, 180, 24);
    listtext->show();

    auto delbtn = new Button(this);
    QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/cancel.png")).arg("QPushButton{color:#ffffff; font-weight:bold;}");
    delbtn->setStyleSheet(ss);
    delbtn->setText("削除");
    delbtn->setGeometry(755 - marginsize_x, 526 - marginsize_y, 200, 40);
    delbtn->show();
    
    connect(delbtn, &Button::clicked, [=]() {

        bool pconoff = false;
        QList<int> list;
        for (int i = 0; i < _reserve_lists.count(); i++)
        {
            auto checkbox =_reserve_lists[i];
            if (checkbox) {
                if (checkbox->isChecked()) {
                    list.append(i + 1);

                    if (i == 0)
                        pconoff = true;
                    //checkbox->setText("");
                }
            }
        }
        if (list.size() == 0) {
            auto alert = new AlertPopup(tr("項目が選択されていません。\r\n削除する項目を\r\n選択してください。"), QObject::tr("ok"), "", false, window());
            alert->show();
        }
        else {
            emit deleteSmartOptimizeData(list);

            if (pconoff)
                pconoff_container->setUncheckAll();
        }
            
        //updateReserveList();
        });


    _listempty_label = new CustomLabel(20, "#FFFFFF", true, 0, 90, this);
    _listempty_label->setText("設定された項目が\r\n ありません");
    _listempty_label->setWordWrap(true);
    _listempty_label->setGeometry(760 - marginsize_x, 350 - marginsize_y, 200, 120);
    _listempty_label->setAlignment(Qt::AlignCenter);
    _listempty_label->hide();


    { //set list pos
        list_pos.clear();
        int plusposY = 35;
        int posY = 246 - marginsize_y;
        list_pos.append(QRect(760 - marginsize_x, posY, 210, 30));
        list_pos.append(QRect(760 - marginsize_x, posY += plusposY, 210, 30));
        list_pos.append(QRect(760 - marginsize_x, posY += plusposY, 210, 30));
        list_pos.append(QRect(760 - marginsize_x, posY += plusposY, 210, 30));
    }

    
    
    { //set reserve data
        for (size_t i = 0; i < 4; i++)
        {
            auto checkbox = new QCheckBox(this);
            checkbox->setStyleSheet(ssdefines::checkboxFixImage);
            checkbox->setCheckable(true);
            checkbox->hide();
            _reserve_lists.insert(i, checkbox);
        }
    }

    updateReserveList();
    setFixedSize(parent->size());

    QObject::connect(pconoff_container, &PcOnOffContainer::sendPcOnOff, [=](bool pcon) {
        emit setSmartOptimizePcOnOffData(pcon, true);
        
        });

    QObject::connect(setschedule_container, &SetScheduleContainer::insertResve, [=](QString text, ReserveData data) {
        int key = 0;
        emit setSmartOptimizeData((int)data.type - 1, data);


        });

    QObject::connect(this, SIGNAL(getSmartOptimizeData()), SmartOptimizeController::inst(), SLOT(slotgetOptiReserveData()));
    QObject::connect(this, SIGNAL(setSmartOptimizePcOnOffData(bool, bool)), SmartOptimizeController::inst(), SLOT(slotsetOptiReservePcOnOff(bool, bool)));
    QObject::connect(this, SIGNAL(setSmartOptimizeData(int, ReserveData)), SmartOptimizeController::inst(), SLOT(slotsetOptiReserve(int, ReserveData)));
    QObject::connect(this, SIGNAL(deleteSmartOptimizeData(QList<int>)), SmartOptimizeController::inst(), SLOT(slotdelOptiReserve(QList<int>)));

    QObject::connect(SmartOptimizeController::inst(), &SmartOptimizeController::signalsetOptiReserveData, [=](QMap<int, QString> viewdata) {

        QMapIterator<int, QString> iter(viewdata);
        while (iter.hasNext()) {
            iter.next();

            int key = iter.key();
            auto checkbox = _reserve_lists[key - 1];
            if (checkbox) {
                checkbox->setText(iter.value());
            }
        }
        updateReserveList();

        });

    QObject::connect(SmartOptimizeController::inst(), &SmartOptimizeController::optimizeReserveSetAlert, [=](bool del) {
        QString msg;
        if(del)
            msg = tr("設定された項目を削除しました");
        else 
            msg = tr("自動最適化予約が\r\n設定されました");

        auto alert = new AlertPopup(msg, QObject::tr("確認"), "", false, window());
        alert->show();
      
        });

    

    emit getSmartOptimizeData();
}

SmartOptimizeView::~SmartOptimizeView()
{

}

void SmartOptimizeView::updateReserveList()
{
    _listempty_label->hide();

    int count = 0;
    for (auto data : _reserve_lists)
    {
        if (data == Q_NULLPTR)
            continue;

        if (data->text().isEmpty()) {
            data->hide();
            continue;
        }

        if (data->text() == "") {
            data->hide();
            continue;
        }

        data->setGeometry(list_pos[count]);
        data->show();
        count++;
    }
    
    if (count == 0) {
        _listempty_label->show();
    }


}
