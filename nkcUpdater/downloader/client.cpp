#include "client.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <shlwapi.h>
#include <windows.h>
#include <QJsonObject>
#include <QTimer>
#include <QSettings>
#include <QLocalSocket>

//#include <Qux/include/async/runonnextframe.h>
#include "../QUX/include/async/runonnextframe.h"

#include "Logger.h"
//#include "nlog/nlog.h"

#include "../define/moduledefine.h"
#pragma comment(lib,"version.lib")

Client::Client(QObject *parent /*= nullptr*/)
	: QObject(parent)
    , _gLogger(std::make_unique<Log::Logger>())
{
    _download.reset(new DownLoader(this));

}

Client::~Client()
{
}

Client& Client::Inst()
{
    static Client* client = new Client;
    return *client;
}

void Client::Initialize()
{
    // 현재 디렉토리 설정 
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    //NLog::inst().initialize();

    // 로그 설정
    QDir dir;
    QDateTime curDate = QDateTime::currentDateTime();
    QString date = curDate.toString("yyyyMMdd");
    QString FolderName = QString("Log") + QDir::separator() + date;

    dir.mkpath(FolderName);
    
    QString FolderPath = dir.absoluteFilePath(FolderName);
    QString logPath = FolderPath + QDir::separator() + "N-KeepCleanerRun.log";
    logPath = QDir::toNativeSeparators(logPath);
    _gLogger->Init(LOG_LEVEL_INFO, logPath.toStdWString().c_str());

    

}

void Client::createWidget(QWidget* parentWidget /*= nullptr*/)
{
    // initialize impl
    m_impl = new Impl;
    m_impl->mainContainer = new nkcUpdater(parentWidget);
    m_impl->mainContainer->show();

 }

int Client::getLauncherExeVersion(const QString& exeFilePath)
{

    int version = 0;
    DWORD dwHandle;
    DWORD dwLen = GetFileVersionInfoSize(exeFilePath.toStdWString().c_str(), &dwHandle);

    // GetFileVersionInfo
    LPVOID lpData = new BYTE[dwLen];
    if (GetFileVersionInfo(exeFilePath.toStdWString().c_str(), dwHandle, dwLen, lpData) == false)
    {
        qDebug() << "error in GetFileVersionInfo";
        delete[] lpData;
        return version;
    }

    // VerQueryValue
    VS_FIXEDFILEINFO* lpBuffer = NULL;
    UINT uLen;

    if (!VerQueryValue(lpData,
        QString("\\").toStdWString().c_str(),
        (LPVOID*)&lpBuffer,
        &uLen))
    {

        qDebug() << "error in VerQueryValue";
        delete[] lpData;
        return version;
    }
    else
    {
            //QString::number((lpBuffer->dwFileVersionMS >> 16) & 0xffff) + "." +
            //QString::number((lpBuffer->dwFileVersionMS) & 0xffff) + "." +
            //QString::number((lpBuffer->dwFileVersionLS >> 16) & 0xffff) + "." +
            version = (lpBuffer->dwFileVersionLS) & 0xffff;
    }
    return version;

}

int Client::getLauncherVersion()
{
    int version = -1;
    QString buildFilePath = QString("%1/.version.result").arg(qApp->applicationDirPath());//QDir().absoluteFilePath(".version.result");
    buildFilePath = QDir::toNativeSeparators(buildFilePath);
    
    QFile buildFile(buildFilePath);
    if (buildFile.open(QIODevice::ReadOnly) == true)
    {
        auto bytearray = buildFile.readAll();
        version = bytearray.toInt();

        buildFile.close();
    }

    // exe파일 버전
    QString launcherExeFilePath = QString("%1/N-KeepCleaner.exe").arg(qApp->applicationDirPath()); //QDir().absoluteFilePath("N-KeepCleaner.exe");
    launcherExeFilePath = QDir::toNativeSeparators(launcherExeFilePath);

    int exeVersion = getLauncherExeVersion(launcherExeFilePath);

    PLOG_INFO << "File Version : " << version << LOG_TAB()
                << "EXE Version : " << exeVersion;

    if (exeVersion != version)
        version = 0;


    return version;
        
}

void Client::connectLocalServer()
{
    QLocalSocket socket;
    socket.connectToServer("MyServer");
    if (socket.waitForConnected()) {
        qDebug() << "Connected to server!";
        socket.write("show");
        socket.flush();
    }
    else {
        qDebug() << "Could not connect to server!";
        qDebug() << "Socket state:" << socket.state();
        qDebug() << "Socket error:" << socket.errorString();
    }
}


void Client::setLauncherVersion(int version)
{
    QString buildFilePath = QDir::currentPath() + QDir::separator() + ".version.result";

    QFile buildFile(buildFilePath);
    if (buildFile.open(QIODevice::WriteOnly | QIODevice::Truncate) == true)
    {
        auto saveVersion = QString::number(version);
        buildFile.write(saveVersion.toStdString().c_str());
        buildFile.close();

        SetFileAttributes(buildFilePath.toStdWString().c_str(), FILE_ATTRIBUTE_NORMAL);
    }



    return;
}

void Client::update()
{
    connect(getDownLoader(), &DownLoader::signalComplate, this, [=]() {
        
        int version = getDownLoader()->getVersion();
        
        PLOG_INFO << "Last Verison : " << version;
        setLauncherVersion(version);

        // 바로 종료하면 로그 등 정보가 찍히지 않아 0.5초 딜레이 후 런처 시작.                    
        QTimer::singleShot(500, this, [=]() {
            QApplication::exit(Client::ReStartUpdate);
        });
        
    });

    connect(getDownLoader(), &DownLoader::signalError, this, [=](int errorType, int errorCode, QString errorMsg, QString FileName) {
        
        int version = getDownLoader()->getVersion();
        setLauncherVersion(0);

        //error
        QApplication::exit();

        PLOG_INFO << "Last Verison (Error): " << 0;
        QTimer::singleShot(500, this, [=]() {
            QApplication::exit(Client::ReStartUpdate);
            });

        
    });

    Qux::Async::runOnNextFrame(this, [=]() {
        _download->update();
    });
    
}

bool Client::isUpdate(const QString& arg)
{
    bool update = true;
    if (_download->getDownVersionInfo() == false)
    {
        PLOG_ERROR << "Update version not found ";
        return true;
    }
    

    // Delete File 
    _download->deleteOldFile();


    int version = _download->getVersion();
    int localVersion = getLauncherVersion();

    PLOG_INFO << "Local Launcher Version [ " << localVersion << " ]"; 
    PLOG_INFO << "Server Launcher Version [ " << version << " ]";

    
    if (version == localVersion) {
        PLOG_INFO << "New Version [ " << version << " ]";
        update = false;
    }

    if (update == true &&
        arg == "restart")
    {
        PLOG_ERROR << "Update <=> Arg Not Empty(Restart)";
        update = false;
    }

    return update;
}

bool Client::startLauncher()
{
    
    QDir dir;
    QString exePath = dir.absoluteFilePath("N-KeepCleaner.exe") ;
    exePath = QDir::toNativeSeparators(exePath);
    
    QString foleder = exePath.section("\\", 0, -2);
    
    TCHAR tchParams[512] = { 0 };

    //std::wstring wstr = param.toStdWString();
    //LPCWSTR lpcwstr = wstr.c_str();

//#ifdef _UNICODE
//    std::wstring wStr = param.toStdWString();
//    const TCHAR* tStr = wStr.c_str();
//#else
//    std::string sStr = qStr.toStdString();
//    const TCHAR* tStr = sStr.c_str();
//#endif

    PLOG_INFO << "NKeepCleaner Execute" << LOG_TAB()
        << "FOLDER : " << foleder << LOG_TAB()
        << "EXENAME : " << exePath;

    
    bool hiderun = isStartupOption();
    //hiderun = true;

    SHELLEXECUTEINFO si;
    ZeroMemory(&si, sizeof(SHELLEXECUTEINFO));
    si.cbSize = sizeof(SHELLEXECUTEINFO);
    si.lpFile = reinterpret_cast<LPCWSTR>(exePath.utf16());
    if (hiderun) {
        si.lpParameters = L"/hide";
        PLOG_INFO << "param run  : " << si.lpParameters;
    }
    else {
        //si.lpParameters = L"/show";
        si.lpParameters = tchParams;
        PLOG_INFO << "param run  : " << si.lpParameters;
    }
        

    si.nShow = SW_SHOWNORMAL;
    si.fMask = SEE_MASK_NOCLOSEPROCESS;
    si.lpVerb = TEXT("open");
    si.lpDirectory = reinterpret_cast<LPCWSTR>(foleder.utf16());

    if (TRUE == ShellExecuteEx(&si))
    {
        DWORD ret = WaitForSingleObject(si.hProcess, 3000); 
        ShowWindow(si.hwnd, SW_SHOWNORMAL);
        SetActiveWindow(si.hwnd);
        SetForegroundWindow(si.hwnd);
        
        return false;
    }


    return true;
}


QWidget* Client::GetMainWidget() const
{
    if (!m_impl) return nullptr;

    return m_impl->mainContainer;
}

bool Client::isStartupOption()
{
    //reg check 
    bool res = false;
    QString keypath = "HKEY_CURRENT_USER\\Software\\TNI\\NKC";
    QSettings settings(keypath, QSettings::NativeFormat);
    QString value = settings.value("hide").toString();
    if (value == "1") {
        res = true;
        settings.setValue("hide", "0");
    }
    else res  = false;

    return res;

    //QString startup = QString("%1/startup.txt").arg(qApp->applicationDirPath());
    //QFile txtfile(startup);
    //if (!txtfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //    qWarning() << "Could not open file:" << txtfile.fileName();
    //    return false;
    //}

    //QTextStream in(&txtfile);
    //QString line;
    //while (!in.atEnd()) {
    //    line = in.readLine();
    //    qDebug() << line;
    //
    //}
    //txtfile.close();

    //bool res = false;
    //if (line == "1") {
    //    res = true;
    //    QFile::remove(startup);
    //}

    //
    //return res;

}