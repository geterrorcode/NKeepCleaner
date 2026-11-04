#include "modal.h"
#include "shadow.h"
#include "../ui.h"

#include "../tool/makemodal.h"

Modal::Modal(/*QString header,*/ QString body, QString ok, QString cancel, QWidget* parent)
    : QWidget(parent)
{
    makeModal(this);

    //setStyleSheet(ss("widget").arg(QString("Malgun Gothic")));
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedWidth(305);
    setMinimumHeight(180);

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

    //{
    //    // <close
    //    auto closeButton = new Button;
    //    closeButton->setFixedSize(20, 20);
    //    closeButton->setStyleSheet(ss("imageButton").arg(":/images/close-20-nor.png", ":/images/close-20-hover.png", ":/images/close-20-press.png"));
    //    connect(closeButton, &Button::clicked, this, &QWidget::close);
    //    baseLayout->addLayout(wrapWidget(closeButton, 0, 8, 8, 0, Qt::AlignTop | Qt::AlignRight));
    //}   // close>


    connect(rcv(this), &Receiver::keyPressed, [this](QKeyEvent* event) {
        if (event->key() == Qt::Key_Escape) {
            close();
        }
    });


    auto mainLayout = new VLayout;
    mainLayout->setContentsMargins(10, 10, 10, 24);    
    baseLayout->addLayout(mainLayout);

    mainLayout->addStretch(1);

    {
        //if (!header.isEmpty()) {
        //    auto headerLabel = new QLabel;
        //    headerLabel->setAlignment(Qt::AlignHCenter);
        //    headerLabel->setStyleSheet(ss("boldLabel").arg("14px", "#333333"));
        //    headerLabel->setText(header);
        //    mainLayout->addWidget(headerLabel);
        //    mainLayout->addSpacing(13);
        //}

        auto bodyLabel = new QLabel;
        bodyLabel->setAlignment(Qt::AlignCenter);
        bodyLabel->setWordWrap(true);
        
        //bodyLabel->setStyleSheet("padding-bottom: 1px; color: #6a615c; font: normal normal 17px; border=none solid #d9d9d9; background-color:red");
        bodyLabel->setStyleSheet(ss("label").arg("17px", "#6a615c", "none"));
        bodyLabel->setText(body);
      
        
        mainLayout->addWidget(bodyLabel, 2);
        mainLayout->addSpacing(10);
    }

    mainLayout->addStretch(1);
 

    {
        auto buttonLayout = new HLayout;        
        buttonLayout->setSpacing(8);
        mainLayout->addLayout(buttonLayout);

        buttonLayout->addStretch(1);

        {
            auto okButton = new Button;


            if (cancel.isEmpty() == true)
            {
                okButton->setFixedSize(160, 41);
                okButton->setStyleSheet(ss("imageButtonNoneText").arg(":/modalpopup/image/modalpopup/okone.png", ":/modalpopup/image/modalpopup/okone_hover.png", ":/modalpopup/image/modalpopup/okone_press.png"
                    , "normal", "15", "#ffffff"));

                okButton->setText(QObject::tr("Ok"));
            }
            else
            {
                okButton->setFixedSize(116, 41);
                okButton->setStyleSheet(ss("imageButtonNoneText").arg(":/modalpopup/image/modalpopup/ok.png", ":/modalpopup/image/modalpopup/ok_over.png", ":/modalpopup/image/modalpopup/ok_press.png"
                    , "normal", "15", "#ffffff"));

                okButton->setText(QObject::tr("Ok"));

            }


            connect(okButton, &Button::clicked, [this]() {
                emit okClicked();
                close();
                });
            buttonLayout->addWidget(okButton);
        }

        if (!cancel.isEmpty()) {
            auto cancelButton = new Button;
            cancelButton->setFixedSize(116, 41);
            cancelButton->setStyleSheet(ss("imageButtonNoneText").arg(":/modalpopup/image/modalpopup/cancel.png", ":/modalpopup/image/modalpopup/cancel_over.png", ":/modalpopup/image/modalpopup/cancel_press.png"
                                                                    , "normal", "15", "#ffffff"));
            cancelButton->setText(QObject::tr("Cancle"));

            connect(cancelButton, &Button::clicked, [this]() {
                emit cancelClicked();
                close();
            });
            buttonLayout->addWidget(cancelButton);
        }


        buttonLayout->addStretch(1);
    }

    {
        auto shadow = new ::Shadow(this, this);
        shadow->setTargetRadius(3);
    }
}