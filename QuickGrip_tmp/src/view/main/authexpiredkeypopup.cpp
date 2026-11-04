#include "authexpiredkeypopup.h"

#include <QClipboard>
#include <QMessageBox>

#include "../common/makemodal.h"

#include "view/Component/vlayout.h"
#include "view/Component/containerlayout.h"
#include "view/Component/customlabel.h"
#include "view/Component/button.h"
#include "view/common/alertpopup.h"

#include "../../common/tool/utils.h"
#include "../../common/defines/ssdefins.h"
#include "../../ui.h"


AuthExpiredKeyPopup::AuthExpiredKeyPopup(QString serialcode, QWidget *parent)
	: QWidget(parent)
{

    makeModal(this);

    setAttribute(Qt::WA_TranslucentBackground);

    setFixedWidth(600);
    setMinimumHeight(340);

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

    //connect(rcv(this), &Receiver::keyPressed, [this](QKeyEvent* event) {
    //    if (event->key() == Qt::Key_Escape) {
    //        close();
    //    }
    //    });

    auto mainLayout = new VLayout;
    mainLayout->setContentsMargins(10, 15, 10, 20);
    baseLayout->addLayout(mainLayout);

    mainLayout->addStretch(1);

    {
        //만료된 시리얼 코드 입니다. 현재 시리얼 코드(%1)로 연장 하거나\r\n새로운 시리얼 코드를 입력 할 수 있습니다.
        QString bodystring = QString("満了したシリアル番号です。現在のシリアル番号(%1)を延長するか、新しいシリアル番号を入力してください"
            "\r\n\r\n# 保存済みのシリアル番号 : %1").arg(serialcode); //저장된 시리얼 코드
        auto bodyLabel = new QLabel;
        bodyLabel->setAlignment(Qt::AlignCenter);
        bodyLabel->setWordWrap(true);

        bodyLabel->setStyleSheet(ssdefines::label.arg("17px", "#6a615c", "none"));
        bodyLabel->setText(utils::toRichText(bodystring, 18));

        mainLayout->addWidget(bodyLabel, 2);
        mainLayout->addSpacing(5);

        QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/datebtn.png")).arg("QPushButton {color: white; font-size: 10pt; }");
        auto copyButton = new Button;
        copyButton->setFixedSize(180, 24);
        copyButton->setStyleSheet(ss);
        copyButton->setText(QObject::tr("既存のシリアル番号のコピー")); //기존 시리얼 코드 복사하기
        connect(copyButton, &Button::clicked, [=]() {
            QApplication::clipboard()->setText(serialcode);
            copyButton->setText(QObject::tr("コピーが完了しました")); //복사 완료되었습니다.
            });


        mainLayout->addWidget(copyButton, 0, Qt::AlignHCenter);
    }

    mainLayout->addStretch(1);
    {
        auto buttonLayout = new HLayout;
        buttonLayout->setSpacing(8);
        mainLayout->addLayout(buttonLayout);

        buttonLayout->addStretch(1);

        {
            auto okButton = new Button;

            QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton {font: bold normal 14px; }");

            QString oktext = "確認";

            okButton->setFixedSize(180, 40);
            okButton->setStyleSheet(ss);
            okButton->setText(oktext);

            connect(okButton, &Button::clicked, [this]() {
                emit callBilling();
                close();
                });
            buttonLayout->addWidget(okButton);
        }

        {
            auto reStartButton = new Button;
            reStartButton->setFixedSize(180, 40);
            QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/cancel.png")).arg("QPushButton {font: bold normal 15px; color:#FFFFFF;}");

            reStartButton->setStyleSheet(ss);
            reStartButton->setText(QObject::tr("他のシリアル番号を認証する")); //다른 키 인증

            connect(reStartButton, &Button::clicked, [this]() {
                //다른 키 인증 하시면 저장된 기존 시리얼 키는 삭제 됩니다.\r\n다른 시리얼 키로 인증 하시겠습니까?
                auto alert = new AlertPopup(tr("他のシリアル番号を認証すると、既存のシリアル番号は削除されます。\r\n他のシリアル番号で認証しますか？"), QObject::tr("ok"), QObject::tr("cancel"), true, window());
                connect(alert, &AlertPopup::okClicked, [=]() {
                    emit reStart();
                    close();
                    });
                alert->show();

                });
            buttonLayout->addWidget(reStartButton);

        }

        {
            auto cancelButton = new Button;
            cancelButton->setFixedSize(180, 40);
            QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/cancel.png")).arg("QPushButton {font: bold normal 15px; color:#FFFFFF;}");

            cancelButton->setStyleSheet(ss);
            cancelButton->setText(QObject::tr("キャンセル"));
            connect(cancelButton, &Button::clicked, [this]() {
                emit cancelClicked();
                close();
                });
            buttonLayout->addWidget(cancelButton);
        }

       


        buttonLayout->addStretch(1);
    }
}

AuthExpiredKeyPopup::~AuthExpiredKeyPopup()
{

    qDebug() << "AuthExpiredKeyPopup Destroy";
}



void AuthExpiredKeyPopup::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        //checkSerialkey();
    }
    else {
        // 그 외의 키 이벤트를 무시.
        event->ignore();
    }
}