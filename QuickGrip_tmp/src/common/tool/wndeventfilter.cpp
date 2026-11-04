#include "wndeventfilter.h"


#define CLEANER_SHOW WM_USER + 1001

WndEventFilter::WndEventFilter(QObject *parent)
	: QObject(parent)
{}

WndEventFilter::~WndEventFilter()
{}


bool WndEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
{
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
        
        MSG* msg = static_cast<MSG*>(message);

        if (msg->message == CLEANER_SHOW) {
            qDebug() << "Received message:" << msg->wParam << msg->lParam;
            QMessageBox::information(nullptr, "RECV OK", "HWND");
            QuickGrip_tmp::inst()->setGotoShow();  

            //emit sendWindowShow();
            return true;
        }
    }

    return false;
}

//
//
//bool WndEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
//{
//    QString eventString = QString::fromStdString(eventType.toStdString());
//    MSG* msg = static_cast<MSG*>(message);
//    if (msg->message == WM_QUERYENDSESSION) {
//        qDebug() << "Windows is shutting down.";
//
//        QuickGrip_tmp::inst()->checkPcOffOptimize();
//        return true;
//    }
//    return false;
//}
