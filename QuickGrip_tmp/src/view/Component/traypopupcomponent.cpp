#include "traypopupcomponent.h"


#include <controller/optimize/optimizecontroller.h>
#include <controller/auto_pcoff/autopcoffcontroller.h>
#include <ui.h>

TrayPopupComponent::TrayPopupComponent(QWidget *parent)
	: QWidget(parent)
{

	hide();

	auto traypopupPtr = Common::createQValue<QPointer<TrayPopup>>(this);

	if (traypopupPtr->isNull() == false)
		traypopupPtr->data()->close();

	auto toastContainer = *traypopupPtr = new TrayPopup("");


    { //pcoff tray popup
        connect(AutoPcOffController::inst(), &AutoPcOffController::pcOffTrayPopup, [=]() {

            QString msg = QString("PCの自動シャットダウン設定により\r\n<font color=#FDBB30>5分後</font>に自動シャットダウンされます");
            auto toastContainer = *traypopupPtr = new TrayPopup(msg);

            auto toastconatinerTimer = new QTimer(toastContainer);
            toastconatinerTimer->setInterval(8000);
            connect(toastconatinerTimer, &QTimer::timeout, [=]() {
                if (traypopupPtr->isNull())
                    return;

                traypopupPtr->data()->close();
                });
            toastconatinerTimer->start();

            traypopupPtr->data()->show();
            });
    }

	connect(OptimizeController::inst(), &OptimizeController::callTraypopup, this, [=](QString states) {
        if(traypopupPtr->isNull() == false)
        {
            traypopupPtr->data()->close();
        }

        auto toastContainer = *traypopupPtr = new TrayPopup(states);
        auto toastconatinerTimer = new QTimer(toastContainer);
        toastconatinerTimer->setInterval(5000);
        connect(toastconatinerTimer, &QTimer::timeout, [=]() {
            if (traypopupPtr->isNull())
                return;

            traypopupPtr->data()->close();
            });
        toastconatinerTimer->start();

        traypopupPtr->data()->show();



		});
}

TrayPopupComponent::~TrayPopupComponent()
{}
