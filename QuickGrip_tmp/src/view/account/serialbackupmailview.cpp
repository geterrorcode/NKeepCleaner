#include "serialbackupmailview.h"

#include <QLineEdit>

#include "../common/makemodal.h"

#include "view/Component/vlayout.h"
#include "view/Component/containerlayout.h"
#include "view/Component/customlabel.h"
#include "view/Component/button.h"
#include "view/Component/dragger.h"

#include "view/common/alertpopup.h"

#include "../../model/account/mailsender.h"

#include <common/defines/ssdefins.h>
#include <common/tool/utils.h>
#include <ui.h>

namespace
{
QString mailSendResultMessage(MailSendResultCode code)
{
    switch (code) {
    case MailSendResultCode::InvalidEmail:
        return QStringLiteral("メールアドレスの形式を確認してください。");

    case MailSendResultCode::InvalidSerial:
    case MailSendResultCode::InvalidUuid:
    case MailSendResultCode::InvalidLicense:
        return QStringLiteral(
            "製品の認証情報を確認できませんでした。\r\n"
            "プログラムを再起動してから、もう一度お試しください。");

    case MailSendResultCode::RateLimited:
        return QStringLiteral(
            "メールの送信回数が制限されています。\r\n"
            "しばらく時間を空けてから、もう一度お試しください。");

    case MailSendResultCode::AuthServerUnavailable:
        return QStringLiteral(
            "製品認証サーバーに接続できませんでした。\r\n"
            "しばらく時間を空けてから、もう一度お試しください。");

    case MailSendResultCode::MailServiceError:
    case MailSendResultCode::InternalServerError:
        return QStringLiteral(
            "現在メールサービスを利用できません。\r\n"
            "しばらく時間を空けてから、もう一度お試しください。");

    case MailSendResultCode::NetworkError:
        return QStringLiteral(
            "サーバーに接続できませんでした。\r\n"
            "インターネット接続を確認してから、もう一度お試しください。");

    case MailSendResultCode::Timeout:
        return QStringLiteral(
            "メールの送信結果を確認できませんでした。\r\n"
            "メールが送信されている可能性がありますので、\r\n"
            "受信トレイをご確認ください。");

    case MailSendResultCode::InvalidJson:
    case MailSendResultCode::InvalidResponse:
    case MailSendResultCode::UnknownError:
        return QStringLiteral(
            "サーバーから正常な応答を受信できませんでした。\r\n"
            "しばらく時間を空けてから、もう一度お試しください。");

    case MailSendResultCode::Success:
        break;
    }

    return QString();
}
}

SerialBackupMailView::SerialBackupMailView(QString serialcode, QWidget *parent)
	: QWidget(parent)
{
	makeModal(this);

	setAttribute(Qt::WA_TranslucentBackground);

	setFixedWidth(455);
	setMinimumHeight(280);

    auto baseLayout = new ContainerLayout(this);
    {
        // <background
        auto backgroundFrame = new QFrame;

        //QString ss = QString("QWidget { border-image: url(%1); background-color: transparent; border: 1px solid #cecece; border-radius: 3px;}").arg(":/main/resource/main/mainbg.png");
        //backgroundFrame->setStyleSheet(ss);
        backgroundFrame->setStyleSheet("background-color: #23262b; border: 1px solid #cecece; border-radius: 3px;");
        baseLayout->addWidget(backgroundFrame);

        // background>
        auto innerLayout = new ContainerLayout;
        innerLayout->setContentsMargins(3, 3, 3, 3);
        baseLayout->addLayout(innerLayout);

        auto innerbackgroundFrame = new QFrame;
        innerbackgroundFrame->setStyleSheet("background-color: #23262b; border: 1px solid #cecece; border-radius: 3px;");
        innerLayout->addWidget(innerbackgroundFrame);

        auto dragger = new Dragger(this);
        dragger->setFocusPolicy(Qt::StrongFocus);
        baseLayout->addWidget(dragger);

    }

    connect(rcv(this), &Receiver::keyPressed, [this](QKeyEvent* event) {
        if (event->key() == Qt::Key_Escape) {
            close();
        }
        });

    auto mainLayout = new VLayout;
    mainLayout->setContentsMargins(10, 15, 20, 20);
    baseLayout->addLayout(mainLayout);

    //mainLayout->addStretch(1);

    {
        //고객님 이메일로 본제품 시리얼코드를 백업하세요.
        QString bodystring = QString("お客様のメールで本製品の\r\nシリアル番号をバックアップしましょう。"); 
        auto bodyLabel = new QLabel;
        bodyLabel->setAlignment(Qt::AlignCenter);
        bodyLabel->setWordWrap(true);
        bodyLabel->setStyleSheet(ssdefines::boldLabel.arg("17px", "#eeeeee", "none"));
        bodyLabel->setText(utils::toRichText(bodystring, 18));
        mainLayout->addWidget(bodyLabel);
        //mainLayout->addWidget(bodyLabel, 2);
        //mainLayout->addSpacing(5);

        auto hlayout = new HLayout();
        hlayout->setAlignment(Qt::AlignCenter);
        hlayout->setContentsMargins(10, 0, 0, 20);
        //mainLayout->addLayout(hlayout,1);
        
        auto emailEdit = new QLineEdit();
        emailEdit->setPlaceholderText("メールアドレスを入力してください");
        emailEdit->setFixedSize(280, 30);
        hlayout->addWidget(emailEdit);
        //QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton {font: bold normal 15px; }");
        QString ss = QString("%1%2").arg("QAbstractButton {color:#FFFFFF; font: normal normal 14px;}").arg(ssdefines::imageButton.arg(":/common/resource/common/linebtn_normal_small.png",
            ":/common/resource/common/linebtn_over_small.png",
            ":/common/resource/common/linebtn_press_small.png",
            ":/common/resource/common/linebtn_normal_small.png"));

        hlayout->addSpacing(10);
        auto sendButton = new Button;
        sendButton->setFixedSize(120, 30);
        sendButton->setStyleSheet(ss);
        sendButton->setText(QObject::tr("メール送信")); //이메일 전송
        connect(sendButton, &Button::clicked, [=]() {
            const QString email = emailEdit->text().trimmed();
            if (email.isEmpty()) {
                //メールアドレスを入力してください
                auto alert = new AlertPopup(tr("メールアドレスを入力してください"), QObject::tr("ok"), "", true, window());
                alert->show();
            }
            else {

                //메일 확인
                QString msg = QString(tr("入力したメールアドレスは<span style=\"color:#2E7D32; font-weight:bold;\">%1</span>です。\r\nメールを送信しますか？")).arg(email.toHtmlEscaped());
                auto alert = new AlertPopup(msg, QObject::tr("確認"), QObject::tr("cancel"), true, window());
                alert->show();
                connect(alert, &AlertPopup::okClicked, [=]() {
                   
                    sendButton->setText(QObject::tr("送信中です..")); //에메일 전송 중
                    sendButton->setEnabled(false);
                    QTimer::singleShot(1000, sendButton, [=]() { //ui반영을 위한 timer
                        MailSender sender;
                        const MailSendResult result = sender.sendSerialCodeEmail(email, serialcode);

                        sendButton->setEnabled(true);
                        sendButton->setText(QObject::tr("メール送信"));

                        if (result.isSuccess()) {
                            //QApplication::clipboard()->setText(serialcode);
                            auto alert = new AlertPopup(tr("お客様のメールアドレスにシリアル番号が送信されました。\r\n"
                                "メールの受信に時間がかかる場合がございます。\r\n　受信できない場合、迷惑メールフォルダをご確認ください。"), QObject::tr("ok"), "", true, window());
                            connect(alert, &AlertPopup::okClicked, [=]() {
                                close();
                                });
                            alert->show();
                        }
                        else {
                            auto alert = new AlertPopup(
                                mailSendResultMessage(result.code),
                                QObject::tr("ok"), "", true, window());
                            alert->show();
                        }
                        });
                    });
            }
            });

        hlayout->addWidget(sendButton);
        mainLayout->addLayout(hlayout);

        auto cancelButton = new Button;
        cancelButton->setFixedSize(120, 30);
        cancelButton->setStyleSheet(ss);
        cancelButton->setText(QObject::tr("キャンセル")); //이메일 전송
        connect(cancelButton, &Button::clicked, [=]() {
            close();
            });

        mainLayout->addWidget(cancelButton, 0, Qt::AlignRight);
    }
}

SerialBackupMailView::~SerialBackupMailView()
{}
