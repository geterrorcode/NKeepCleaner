#include "pcendsesstionevent.h"

#include <QMessageBox>
#include <QTimer>

//PcEndSesstionEvent::PcEndSesstionEvent(QObject* receiver)
//	: _receiver(receiver)
//{}

bool PcEndSesstionEvent::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
{
    QString eventString = QString::fromStdString(eventType.toStdString());
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_QUERYENDSESSION) {

        //// 종료를 막고 결과를 FALSE로 설정
        //*result = FALSE;

        //qDebug() << "Windows is shutting down.";

        //if (QuickGrip_tmp::inst()->isCheckPcOffOptimize() == true) {

        //    QuickGrip_tmp::inst()->checkPcOffOptimize();
        //    QTimer::singleShot(10000, [=]() {
        //        *result = TRUE;
        //        });
        //        //Sleep(1000 * 4);
        //}
        //else {
        //    *result = TRUE;
        //}
        //
        return true;
    }
    //else if (msg->message == WM_ENDSESSION) {
    //    qDebug() << "Windows is shutting down.";

    //    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "종료 확(2)인", "프로그램을 종료하시겠습니까?", QMessageBox::Yes | QMessageBox::No
    //    );

    //    if (reply == QMessageBox::Yes) {
    //        // 종료를 원할 경우 프로그램 종료
    //        QApplication::quit();
    //    }


    //    QuickGrip_tmp::inst()->checkPcOffOptimize();
    //    return false;
    //}
    return false;
}
