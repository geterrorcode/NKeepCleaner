#include "alertpopup.h"

#include "makemodal.h"

#include "view/Component/vlayout.h"
#include "view/Component/containerlayout.h"
#include "view/Component/customlabel.h"
#include "view/Component/button.h"

#include "../../common/tool/utils.h"
#include "../../common/defines/ssdefins.h"
#include "../../ui.h"

AlertPopup::AlertPopup(QString body, QString ok, QString cancel, bool big, QWidget* parent, const QSize customsize)
	: QWidget(parent)
{

	makeModal(this);

	setAttribute(Qt::WA_TranslucentBackground);

    if (customsize.width() > 0 && customsize.height() > 0) {
        setFixedSize(customsize);
    }
    else {
        if (big) {
            setFixedWidth(405);
            setMinimumHeight(280);
        }
        else {
            setFixedWidth(245);
            setMinimumHeight(190);
        }
    }

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
            if (!_resultEmitted) {
                _resultEmitted = true;
                emit cancelClicked();
            }
            close();
        }
        });


    auto mainLayout = new VLayout;
    mainLayout->setContentsMargins(10, 15, 10, 20);
    baseLayout->addLayout(mainLayout);

    mainLayout->addStretch(1);

    {
        auto bodyLabel = new QLabel;
        bodyLabel->setAlignment(Qt::AlignCenter);
        bodyLabel->setWordWrap(true);

        //bodyLabel->setStyleSheet("padding-bottom: 1px; color: #6a615c; font: normal normal 17px; border=none solid #d9d9d9; background-color:red");
        
        //boost->setStyleSheet(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png"));
        bodyLabel->setStyleSheet(ssdefines::label.arg("17px", "#6a615c", "none"));
        bodyLabel->setText(utils::toRichText(body, 18));


        mainLayout->addWidget(bodyLabel, 2);
        mainLayout->addSpacing(5);
    }

    mainLayout->addStretch(1);


    {
        auto buttonLayout = new HLayout;
        buttonLayout->setSpacing(8);
        mainLayout->addLayout(buttonLayout);

        buttonLayout->addStretch(1);

        {
            auto okButton = new Button;

            //QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton{color:#23262A; font-weight:bold;}");

            QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton {font: bold normal 15px; }");

            QString okmsg; 
            if (ok == "ok")
                okmsg = "確認";
            else
                okmsg = ok;

            if (cancel.isEmpty() == true)
            {
                okButton->setFixedSize(160, 41);
                okButton->setStyleSheet(ss);
                //okButton->setStyleSheet(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png"));
                okButton->setText(okmsg);
            }
            else
            {
                okButton->setFixedSize(116, 41);
                okButton->setStyleSheet(ss);
                //okButton->setStyleSheet(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png"));
                okButton->setText(okmsg);
            }


            connect(okButton, &Button::clicked, [this]() {
                _resultEmitted = true;
                emit okClicked();
                close();
                });
            buttonLayout->addWidget(okButton);
        }

        if (!cancel.isEmpty()) {
            auto cancelButton = new Button;
            cancelButton->setFixedSize(116, 41);
            QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/cancel.png")).arg("QPushButton {font: bold normal 15px; color:#FFFFFF;}");

            cancelButton->setStyleSheet(ss);

            cancelButton->setText(QObject::tr("キャンセル"));

            connect(cancelButton, &Button::clicked, [this]() {
                _resultEmitted = true;
                emit cancelClicked();
                close();
                });
            buttonLayout->addWidget(cancelButton);
        }


        buttonLayout->addStretch(1);
    }



}

AlertPopup::~AlertPopup()
{}


void AlertPopup::keyPressEvent(QKeyEvent* event)
{
    //if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
    //    checkSerialkey();
    //}
    //else {
    //    // 그 외의 키 이벤트를 무시.
    //    event->ignore();
    //}
}

void AlertPopup::closeEvent(QCloseEvent* event)
{
    if (!_resultEmitted) {
        _resultEmitted = true;
        emit cancelClicked();
    }

    QWidget::closeEvent(event);
}
