#include "authwaitingpopup.h"

#include <QLabel>

#include "../common/makemodal.h"

#include "view/Component/vlayout.h"
#include "view/Component/containerlayout.h"
#include "view/Component/customlabel.h"
#include "view/Component/button.h"

#include "../../common/tool/utils.h"
#include "../../common/defines/ssdefins.h"
#include "../../ui.h"


AuthWaitingPopup::AuthWaitingPopup(bool anyopen, QWidget *parent)
	: QDialog(parent)
{
    makeModal(this);

    setAttribute(Qt::WA_TranslucentBackground);

    setFixedWidth(405);
    setMinimumHeight(280);

    auto baseLayout = new ContainerLayout(this);
    {
        // <background
        auto backgroundFrame = new QFrame;
        backgroundFrame->setStyleSheet("background-color: #ffffff; border: 1px solid #cecece; border-radius: 3px;");
        baseLayout->addWidget(backgroundFrame);

        // background>
        auto innerLayout = new ContainerLayout;
        innerLayout->setContentsMargins(3, 3, 3, 3);
        baseLayout->addLayout(innerLayout);

        auto innerbackgroundFrame = new QFrame;
        innerbackgroundFrame->setStyleSheet("background-color: #ffffff; border: 1px solid #cecece; border-radius: 3px;");
        innerLayout->addWidget(innerbackgroundFrame);
    }

    auto mainLayout = new VLayout;
    mainLayout->setContentsMargins(10, 10, 10, 20);
    baseLayout->addLayout(mainLayout);
    mainLayout->addStretch(1);


    QString bodystring;
    if (anyopen) 
        bodystring = QString("인증 확인 중입니다.\r\n잠시만 기다려 주세요.");
    else 
        bodystring = QString("인증 처리 중입니다.\r\n잠시만 기다려 주세요.");

    auto bodyLabel = new QLabel;
    bodyLabel->setAlignment(Qt::AlignCenter);
    bodyLabel->setWordWrap(true);

    bodyLabel->setStyleSheet(ssdefines::label.arg("17px", "#6a615c", "true"));
    bodyLabel->setText(utils::toRichText(bodystring, 20));

    mainLayout->addWidget(bodyLabel, 2);

    ///QString gifpath = QString("%1/image/main_status_bad.gif").arg(qApp->applicationDirPath());

    //mainLayout->addSpacing(5);

    mainLayout->addStretch(1);

    auto movie = new QMovie();
    auto gif = new QLabel(this);
    gif->setFixedSize(100, 100);
    gif->setMovie(movie);
    gif->setAttribute(Qt::WA_NoSystemBackground);
    movie->setScaledSize(QSize(100, 100));

    movie->setFileName(":/main/resource/main/waiting.gif");

    auto startGif = [=]() {
        movie->start();
    };


    mainLayout->addWidget(gif, 0, Qt::AlignHCenter);

    startGif();

}

AuthWaitingPopup::~AuthWaitingPopup()
{}
