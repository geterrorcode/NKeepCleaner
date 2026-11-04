#include "AuthSerialKeyResetPopup.h"

#include <QClipboard>
#include <QMessageBox>

#include "../common/makemodal.h"

#include "view/Component/vlayout.h"
#include "view/Component/containerlayout.h"
#include "view/Component/customlabel.h"
#include "view/Component/button.h"

#include "../../common/tool/utils.h"
#include "../../common/defines/ssdefins.h"
#include "../../ui.h"


AuthSerialKeyResetPopup::AuthSerialKeyResetPopup(QString serialcode, QWidget *parent)
	: QWidget(parent)
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

    connect(rcv(this), &Receiver::keyPressed, [this](QKeyEvent* event) {
        if (event->key() == Qt::Key_Escape) {
            close();
        }
        });

    auto mainLayout = new VLayout;
    mainLayout->setContentsMargins(10, 15, 10, 20);
    baseLayout->addLayout(mainLayout);

    mainLayout->addStretch(1);

    {
        //정상적이지 않은 시리얼 코드가 저장되어 있습니다. \r\n시리얼코드를 다시 등록 해주세요.
        QString bodystring = QString("シリアル番号が正常ではありません。\r\nシリアル番号をもう一度登録してください。"
            "\r\n\r\n# 保存済みのシリアル番号 : %1").arg(serialcode);
        auto bodyLabel = new QLabel;
        bodyLabel->setAlignment(Qt::AlignCenter);
        bodyLabel->setWordWrap(true);

        bodyLabel->setStyleSheet(ssdefines::label.arg("17px", "#6a615c", "none"));
        bodyLabel->setText(utils::toRichText(bodystring, 18));

        mainLayout->addWidget(bodyLabel, 2);
        mainLayout->addSpacing(5);

        QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/smartopti/datebtn.png")).arg("QPushButton {font: normal normal 10px; }");
        auto copyButton = new Button;
        copyButton->setFixedSize(150, 21);
        copyButton->setStyleSheet(ss);
        copyButton->setText(QObject::tr("既存のシリアル番号のコピー")); //기존 시리얼 코드 복사하기
        connect(copyButton, &Button::clicked, [=]() {
            QApplication::clipboard()->setText(serialcode);
            copyButton->setText(QObject::tr("コピーが完了しました")); //복사 완료되었습니다.
            //QMessageBox::information(nullptr, "복사 완료", "복사가 완료되었습니다.");
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

            QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton {font: bold normal 15px; }");

            QString oktext = "確認";
            //QString canceltext = "취소";

            okButton->setFixedSize(116, 41);
            okButton->setStyleSheet(ss);
            okButton->setText(oktext);

            connect(okButton, &Button::clicked, [this]() {
                emit okClicked();
                close();
                });
            buttonLayout->addWidget(okButton);
        }

        auto cancelButton = new Button;
        cancelButton->setFixedSize(116, 41);
        QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/cancel.png")).arg("QPushButton {font: bold normal 15px; color:#FFFFFF;}");

        cancelButton->setStyleSheet(ss);

        cancelButton->setText(QObject::tr("キャンセル"));

        connect(cancelButton, &Button::clicked, [this]() {
            emit cancelClicked();
            close();
            });
        buttonLayout->addWidget(cancelButton);


        buttonLayout->addStretch(1);
    }

}

AuthSerialKeyResetPopup::~AuthSerialKeyResetPopup()
{}

void AuthSerialKeyResetPopup::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        //checkSerialkey();
    }
    else {
        // 그 외의 키 이벤트를 무시.
        event->ignore();
    }
}