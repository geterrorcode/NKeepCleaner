#include "QuickGrip_tmp.h"


#include <QtWidgets/QApplication>
#include <QSingleApplication/SingleApplication>
#include <QAbstractNativeEventFilter>

#include <QSharedMemory>  
#include <QMessageBox>

#include <QLocalServer>
#include <QLocalSocket>
#include <QMessageBox>
#include <QDebug>

#include <Logger.h>
#include <loginstance.h>

#include "src/QSingleApplication/qtsingleapplication.h"

#include "src/view/main/serialkeyinputwidget.h"

#include "common/network/localserver.h"

#include "src/common/tool/wndeventfilter.h"
#include "model/configdata/configdata.h"

//#include "common/tool/UACByPass.h"


bool hiderun = false;

void nkcMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    if (localMsg.contains("Unexpected null receiver")) {
        // Windows의 디버그 출력을 위해 OutputDebugString 함수를 사용합니다.
        QString debugMessage = QString("Stack trace: %1:%2 %3\n")
            .arg(context.file)
            .arg(context.line)
            .arg(context.function);
        OutputDebugString(reinterpret_cast<LPCWSTR>(debugMessage.utf16()));
    }
    else {
        // 나머지 메시지들을 위해 OutputDebugString 사용
        QString formattedMsg = QString("%1\n").arg(msg);
        OutputDebugString(reinterpret_cast<LPCWSTR>(formattedMsg.utf16()));
    }
    
}

//
//void setClassName()
//{
//    QMainWindow w;
//    // Qt 윈도우 핸들 가져오기
//    HWND hwnd = reinterpret_cast<HWND>(w.winId());
//
//    // 윈도우 클래스 이름 설정
//    const char* className = "MyCustomClassName";
//    SetClassLongPtrA(hwnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(className));
//}



int main(int argc, char *argv[])
{


    //int res = UACBypass();
    //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QtSingleApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QtSingleApplication app(argc, argv);

    LogInstance::Inst().Initialize();

    //WndEventFilter filter;
    //app.installNativeEventFilter(&filter);

    //QApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    //if (a.sendMessage("ShowLauncher"))
    //    return 0;

    QString arg = QString::fromLocal8Bit(argv[1]);

    qInstallMessageHandler(nkcMessageOutput);

    QSharedMemory shared("N-KeepCleaner");
    if (!shared.create(512, QSharedMemory::ReadWrite)) 
        return 0;
    else
    {
        QLocalServer server;
        QObject::connect(&server, &QLocalServer::newConnection, [&]() {
            QLocalSocket* socket = server.nextPendingConnection();
            QObject::connect(socket, &QLocalSocket::readyRead, [&, socket]() {
                QByteArray data = socket->readAll();
                qDebug() << "Received:" << data;
                socket->close();
                socket->deleteLater();


                QuickGrip_tmp::inst()->setGotoShow();
                });
            });

        if (!server.listen("MyServer"))
            qDebug() << "Server could not start!";
        else 
            qDebug() << "Server started!";
         
        //LocalServer::inst()->setServer();

        {
            QFile fontFile(":/font/font/Roboto-Regular.ttf");
            if (fontFile.exists() == true)
            {
                int newfontId = QFontDatabase::addApplicationFont(fontFile.fileName());
                QString fontname = QFontDatabase::applicationFontFamilies(newfontId).at(0);

                QFont font(fontname);
                font.setStyleStrategy(QFont::PreferAntialias);

                qApp->setFont(font);
            }
            else
            {
                QFont font("Roboto Gothic");
                qApp->setFont(font);
            }
        }

        { //resource setting

            QString path = qApp->applicationDirPath() + "/n-keepcleaner.rcc";
            bool result = QResource::registerResource(qApp->applicationDirPath() + "/n-keepcleaner.rcc");
            Q_ASSERT(result);
        }

        ConfigData::inst();
        
        if (ConfigData::inst()->getSerialKey().isEmpty()) {
            //SerialKeyInputWidget::inst()->show();
            PLOG_INFO << "N-KeepCleaner serial input widget call";

            SerialKeyInputWidget::inst()->setGotoShow();
            QuickGrip_tmp::inst()->hide();
        }
        else {
            //기존 인증 정보 있는 경우만 인증.
            QuickGrip_tmp::inst()->setAuthCheck();

            //QString arg = argv[1];
            //QString arg = QString::fromLocal8Bit(argv[1]);

            //check serial key
            if (arg.indexOf("hide") > 0) {
                //hiderun = true;
                QuickGrip_tmp::inst()->setTrayRun(true);
                QuickGrip_tmp::inst()->hide();
            }
            else {
                QuickGrip_tmp::inst()->setGotoShow();
                //QuickGrip_tmp::inst()->show();
                //QuickGrip_tmp::inst()->activateWindow();
            }
                
        }

        int result = app.exec();
        QtSingleApplication::processEvents(QEventLoop::AllEvents, 100);
        return result;
    }

}
