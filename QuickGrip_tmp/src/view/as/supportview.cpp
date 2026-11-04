#include "supportview.h"

#include "view/Component/vlayout.h"
#include "view/Component/button.h"
#include "view/Component/customlabel.h"
#include "view/Container/topinfowidget.h"
#include "view/Component/copylabel.h"

#include <common/defines/ssdefins.h>
#include <ui.h>

SupportView::SupportView(QWidget*parent)
	: QWidget(parent)
{
    int marginsize_x = 218;
    int marginsize_y = 50;

    auto baseLayout = new ContainerLayout(this);
    {
        auto backgroundFrame = new QFrame;
        backgroundFrame->setStyleSheet(ssdefines::imageWidget.arg(":/main/resource/main/mainbg.png"));
        baseLayout->addWidget(backgroundFrame);
    }

    auto topbase = new QLabel(this);
    topbase->setStyleSheet(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/top.png"));
    topbase->setGeometry(258 - marginsize_x, 76 - marginsize_y, 710, 74);
    topbase->show();

    auto toplabel = new CustomLabel(20, "#FFFFFF", true, 0, 0, this);
    toplabel->setGeometry(258 - marginsize_x, 98 - marginsize_y, 710, 24);
    toplabel->setAlignment(Qt::AlignCenter);
    toplabel->setText(QString("アプリを使用中に問題が発生しましたら以下にお問い合わせください"));
    toplabel->show();

    auto body = new QLabel(this);
    body->setStyleSheet(ssdefines::imageWidget.arg(":/option/resource/preview/option/gbbody.png"));
    body->setGeometry(258 - marginsize_x, 175 - marginsize_y, 713, 465);
    body->show();

    //お問い合わせ先：N-KeepCreanerカスタマーサポートチーム
    auto supportteam = new CustomLabel(20, "#FFFFFF", true, 0, 0, this);
    supportteam->setText("お問い合わせ先：N-KeepCleanerカスタマーサポートチーム");
    supportteam->setGeometry(313 - marginsize_x, 220 - marginsize_y, 600, 20);
    supportteam->show();

    auto phoneimg = new QLabel(this);
    phoneimg->setStyleSheet(ssdefines::imageWidget.arg(":/as/resource/as/phone.png"));
    phoneimg->setGeometry(328 - marginsize_x, 279 - marginsize_y, 26, 26);
    phoneimg->show();

    auto phonenum = new CustomLabel(20, "#FFFFFF", true, 0, 0, this);
    phonenum->setGeometry(354 - marginsize_x, 280 - marginsize_y, 250, 30);
    //phonenum->setText("電話でのお問い合わせ :  <font color=#FDBB30>050-55526-9859</font>");
    phonenum->setText("電話でのお問い合わせ :  ");
    phonenum->show();

    auto numtext = new CopyLabel(20, true, "#FDBB30", this);
    numtext->setGeometry(584 - marginsize_x, 276 - marginsize_y, 450, 30);
    numtext->setText("050-5526-9848");
    numtext->show();

    //auto phonecopy = new Button(this);
    //phonecopy->setText("전화번호 클립보드에 복사");
    //phonecopy->setGeometry(328 - marginsize_x, 410 - marginsize_y, 203, 40);
     
    auto phonetime = new CustomLabel(15, "#FDFDFD", false, 0, 0, this);
    phonetime->setGeometry(336 - marginsize_x, 320 - marginsize_y, 600, 40);
    phonetime->setText("※平日9時～18時が対応時間となっており、土日祝は対応外となります");
    phonetime->show();

    auto emailimg = new QLabel(this);
    emailimg->setStyleSheet(ssdefines::imageWidget.arg(":/as/resource/as/email.png"));
    emailimg->setGeometry(328 - marginsize_x, 375 - marginsize_y, 28, 28);
    emailimg->show();

    auto email = new CustomLabel(20, "#FFFFFF", true, 0, 0, this);
    email->setGeometry(354 - marginsize_x, 376 - marginsize_y, 550, 30);
    email->setText("メールでのお問い合わせ： ");
    email->show();

    auto emailtext = new CopyLabel(20, true, "#FDBB30", this);
    emailtext->setGeometry(584 - marginsize_x, 372 - marginsize_y, 450, 30);
    emailtext->setText("helpdesk@tni-sc.com");
    emailtext->show();

    auto as_btn = new Button(this);
    as_btn->setText("メールでお問い合わせ");
    as_btn->setGeometry(328 - marginsize_x, 410 - marginsize_y, 203, 40);

    QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton{font-weight:bold;}");
    as_btn->setStyleSheet(ss);
    as_btn->show();
    connect(as_btn, &Button::clicked, [=]() {
        QDesktopServices::openUrl(QUrl("mailto:?to=helpdesk@tni-sc.com&subject=N-KeepCleaner お問い合わせメール&body=ここにお問い合わせ内容を入力し送信してください。\r\n\r\n", QUrl::TolerantMode));
        });
    as_btn->show();

    auto emailtime = new CustomLabel(15, "#FDFDFD", false, 0, 0, this);
    emailtime->setGeometry(336 - marginsize_x, 460 - marginsize_y, 450, 20);
    emailtime->setText("※対応時間：24時間受付");
    emailtime->show();

    auto emailreturn = new CustomLabel(15, "#FDFDFD", false, 0, 0, this);
    emailreturn->setGeometry(336 - marginsize_x, 490 - marginsize_y, 450, 20);
    emailreturn->setText("※営業日3日以内にご返信させていただきます");
    emailreturn->show();

    auto emaildesc = new CustomLabel(15, "#FDFDFD", false, 0, 0, this);
    emaildesc->setWordWrap(true);
    emaildesc->setGeometry(336 - marginsize_x, 520 - marginsize_y, 650, 20);
    emaildesc->setText("※[メールでお問い合わせ]ボタンを押すとメールが自動で作成されます");
    emaildesc->show();

    setFixedSize(parent->size());
}

SupportView::~SupportView()
{}
