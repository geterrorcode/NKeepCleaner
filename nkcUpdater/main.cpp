#include "nkcupdater.h"
#include <QtWidgets/QApplication>

#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <thread>
#include <windows.h>
#include <Shlobj.h>
#include <QFontDatabase>
#include <QSslSocket>
#include <QSharedMemory> 

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


#include <QLocalSocket>


#include <Logger.h>

#include "SingleApplication/include/qtsingleapplication.h"

#include "downloader/client.h"
#include "tool/utils.h"

#pragma comment(lib,"Shell32.lib")

#define CLEANER_SHOW WM_USER + 1001

bool isInternetConnectionAvailable()
{
    QNetworkAccessManager netManager;
    QNetworkRequest request(QUrl("https://download.tnilab.com/files/updater.json"));
    QNetworkReply* reply = netManager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->bytesAvailable())
        return true; //인터넷 연결이 있습니다.
    else
        return false; //인터넷 연결이 없습니다.
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);
    if (lpdwProcessId == lParam) {
        

        SendMessage(hwnd, CLEANER_SHOW, 123, 456);

        //QMessageBox::information(nullptr, "SendMessage OK", QString(" PID :  %1").arg(lpdwProcessId));


        return FALSE;
    }
    return TRUE;
}


int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    QString arg = QString::fromLocal8Bit(argv[1]);
    //QString arg2 = "";

    Client::Inst().Initialize();
    PLOG_INFO << ">>>>>>>> NPUPDATE START ";

    //N-KeepCleanerWnd
    //HWND hwnd = FindWindowA("NKeepCleanerCls", NULL); // 다른 윈도우의 타이틀을 입력하십시오.
    //HWND hwnd = FindWindowA(NULL, "N-KeepCleanerWnd"); // 다른 윈도우의 타이틀을 입력하십시오.
    //if (hwnd) {
    //    
    //    SendMessage(hwnd, CLEANER_SHOW,123, 456); // WM_USER + 1는 사용자 정의 메시지입니다.
    //    QMessageBox::information(nullptr, "SendMessage OK", "HWND");
    //}

    INT64 pid = updaterUtils::getPID(L"N-KeepCleaner.exe");
    if (pid > 0) {
        QLocalSocket socket;
        socket.connectToServer("MyServer");
        if (socket.waitForConnected()) {
            socket.write("1");
            socket.flush();
            Sleep(300);
        }
        else {
            qDebug() << "Could not connect to server!";
            qDebug() << "Socket state:" << socket.state();
            qDebug() << "Socket error:" << socket.errorString();
        }

        return 1;
    }

    bool res = QSslSocket::supportsSsl();
    if (res == false) {
        PLOG_ERROR << "supportsSsl Fail!";
        Client::Inst().startLauncher();
        return 0;
    }

    if (!isInternetConnectionAvailable()) {
        PLOG_ERROR << "Internet Connect Fail!";

        //3초 체크 후
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        PLOG_ERROR << "Internet 3SEC";
        if (!isInternetConnectionAvailable()) {
            Client::Inst().startLauncher();
            return 0;
        }
    }

    if (arg == "restart")
    {
        PLOG_INFO << "Waiting...";
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        //hide check

    }

    PLOG_INFO << ">>>>>>>> NPUPDATE START [" << arg << "]";

    if (app.isRunning() == true)
    {
        PLOG_INFO << ">>>>>>>> Already Running Launcher";
        //QMessageBox::information(nullptr, "Execution Error", "Updater is already running.");
        Client::Inst().startLauncher();
        return 0;
    }

    int result = 0;

    if (Client::Inst().isUpdate(arg) == true)
    {
        Client::Inst().createWidget();
        Client::Inst().update();

        result = app.exec();
        if (result == Client::ReStartUpdate)
        {
            PLOG_INFO << "Update Complate Run Update";

            QStringList argList;
            argList.append("restart");
            auto exeName = QDir().absoluteFilePath("N-KeepCleanerRun.exe");
            PLOG_INFO << exeName ;
            QProcess::startDetached(exeName, argList);
        }
        
    }
    else
    {
        PLOG_INFO << "ARG" << arg;
        if (arg.isEmpty() == false)
        {
            PLOG_INFO << "Update Complate [ Restart Update ]";
        }
        else
        {
            PLOG_INFO << "Already Update";
        }

        // 런처 시작 
        Client::Inst().startLauncher();
    }
    return result;
  /*  nkcUpdater w;
    w.show();
    return app.exec();*/
}
