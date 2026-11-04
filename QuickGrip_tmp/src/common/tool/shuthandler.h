#include <QObject>
#include <QDebug>
#include <Windows.h>
class ShutdownHandler : public QObject
{
    Q_OBJECT

public:
    //explicit ShutdownHandler(QObject* parent = nullptr)
    //    : QObject(parent)
    //{
    //    if (!SetProcessShutdownParameters(0x100, SHUTDOWN_NORETRY))
    //    {
    //        qWarning() << "Failed to set process shutdown parameters.";
    //    }
    //}

    ////virtual bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) Q_DECL_OVERRIDE;
    //bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) Q_DECL_OVERRIDE
    //{
    //    Q_UNUSED(eventType);

    //    MSG* msg = reinterpret_cast<MSG*>(message);
    //    if (msg->message == WM_ENDSESSION)
    //    {
    //        // PC 종료 이벤트가 발생하여 종료 처리
    //        qDebug() << "PC 종료 이벤트 발생";

    //        // 종료 처리를 수행하는 코드 작성

    //        // 종료를 원할 경우 결과를 TRUE로 설정
    //        *result = TRUE;

    //        // 이벤트 처리를 완료하였음을 알림
    //        return true;
    //    }

    //    return false;
    //}
};