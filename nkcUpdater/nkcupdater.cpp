#include "nkcupdater.h"

#include "Container/downloadcontainer.h"
#include "downloader/client.h"

#include "define/errordefine.h"

//#include "../QuickGrip_tmp/src/view/common/alertpopup.h"
//#include "Component/alertpopup.h"

#include "ui.h"



nkcUpdater::nkcUpdater(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    setFixedSize(303, 200);

    auto centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    setCentralWidget(centralWidget);

    //QString aa = qApp->applicationDirPath();
    bool result = QResource::registerResource(qApp->applicationDirPath() + "/n-keepcleaner.rcc");
    //ui.setupUi(this);
    setWindowIcon(QIcon(":/resource/resource/icon.png"));

    auto baseLayout = new ContainerLayout(centralWidget);
    {
        // <background
        auto backgroundFrame = new QFrame;
        backgroundFrame->setStyleSheet("background-color:#2E3135; border:1px solid #cecece; border-radius:3px;");
        baseLayout->addWidget(backgroundFrame);

    }

    auto downContainer = new DownloadContainer;
    baseLayout->addWidget(downContainer);

    connect(Client::Inst().getDownLoader(), &DownLoader::signalError, this, [=](int errorType, int errorCode, QString errorMsg, QString FileName) {

        QString msg = tr("An error occurred during the update.");  //업데이트 중 오류가 발생했습니다.
        QString msgError = tr("Error Code"); 

        if (errorType == DOWNLOAD_ERROR::ERROR_METAFILE)
            errorType = 490; 

        QString errorText = QString("%1\r\n[%2 : %3(%4:%5)]").arg(msg).arg(msgError).arg(errorType).arg(errorMsg).arg(FileName);

        PLOG_ERROR << errorText;
        /*auto alert = new AlertPopup(tr("Update Error \r\n%1").arg(errorText), QObject::tr("ok"), "", true, window());
        connect(alert, &AlertPopup::okClicked, [=]() {
            close();
            });
        alert->show();*/

        /*auto modal = new Modal(errorText, "ok", "", this);

        connect(modal, &Modal::okClicked, [=]() {
            close();
            });
        modal->show();*/

        });


}

nkcUpdater::~nkcUpdater()
{}
