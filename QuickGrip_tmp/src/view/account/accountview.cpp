#include "accountview.h"

#include "view/Component/vlayout.h"
#include "view/Component/button.h"
#include "view/Component/customlabel.h"
#include "view/common/alertpopup.h"

#include "view/Container/topinfowidget.h"

#include <controller/account/accountcontroller.h>
#include <common/network/webpagecaller.h>

//#include "../../model/account/mailsender.h"
#include "../../model/configdata/configdata.h"

#include "serialbackupmailview.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"

AccountView::AccountView(QWidget *parent)
	: QWidget(parent)
{
	auto webManager = new WebPageCaller(this);

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
	toplabel->setText(QString("<font color=#FDBB30>契約情報</font>と"
		"<font color=#FDBB30>シリアル番号</font>を確認できます"));//계약정보와 시리얼 번호를 확인 할 수 있습니다.
	toplabel->show();

	auto body = new QLabel(this);
	body->setStyleSheet(ssdefines::imageWidget.arg(":/option/resource/preview/option/gbbody.png"));
	body->setGeometry(258 - marginsize_x, 175 - marginsize_y, 713, 465);
	body->show();

	//auto body = new QLabel(this);
	//body->setStyleSheet(ssdefines::imageWidget.arg(":/option/resource/preview/option/gbbody.png"));
	//body->setGeometry(258 - marginsize_x, 175 - marginsize_y, 713, 230);
	//body->show();

	//auto bodyBottom = new QLabel(this);
	//bodyBottom->setStyleSheet(ssdefines::imageWidget.arg(":/option/resource/preview/option/gbbody.png"));
	//bodyBottom->setGeometry(258 - marginsize_x, 420 - marginsize_y, 713, 235);
	//bodyBottom->show();


	auto subscribelabel = new CustomLabel(20, "#FFFFFF", false, 0, 0, this);
	subscribelabel->setGeometry(313 - marginsize_x, 220 - marginsize_y, 600, 20);
	subscribelabel->show();
	

	auto exprired = new CustomLabel(18, "#FFFFFF", false, 0, 0, this);
	exprired->setGeometry(333 - marginsize_x, 255 - marginsize_y, 400, 20);
	exprired->show();


	//Serial key
	auto serialkeylabel = new CustomLabel(20, "#FFFFFF", false, 0, 0, this);
	serialkeylabel->setGeometry(313 - marginsize_x, 300 - marginsize_y, 400, 20);
	serialkeylabel->show();

	auto serialBackup = new CustomLabel(18, "#FFFFFF", false, 0, 0, this);
	serialBackup->setText("※ シリアル番号のバックアップ : "); //시리얼번호 백업하기
	serialBackup->setGeometry(333 - marginsize_x, 335 - marginsize_y, 400, 20);
	serialBackup->hide();

	auto copyButton = new Button(this);
	//copyButton->setGeometry(393 - marginsize_x, 335 - marginsize_y, 151, 30);
	copyButton->setGeometry(593 - marginsize_x, 331 - marginsize_y, 120, 30);
	{
		QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/datebtn.png")).arg("QPushButton {color: white; font-size: 10pt; }");
		copyButton->setStyleSheet(ss);
		copyButton->setText(QObject::tr("コピー"));//복사하기
		connect(copyButton, &Button::clicked, [=]() {

			QString serialCode = ConfigData::inst()->getSerialKey();
			
			QApplication::clipboard()->setText(ConfigData::inst()->getSerialKey());

			auto alert = new AlertPopup(tr("コピーが完了しました"), QObject::tr("ok"), "", false, window());//복사 완료되었습니다.
			alert->show();
			});
	}
	copyButton->hide();

	auto mailButton = new Button(this);
	mailButton->setGeometry(723 - marginsize_x, 331 - marginsize_y, 120, 30);
	{
		QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/datebtn.png")).arg("QPushButton {color: white; font-size: 10pt; }");
		mailButton->setStyleSheet(ss);
		mailButton->setText(QObject::tr("メールで受け取る")); //메일전송
		connect(mailButton, &Button::clicked, [=]() {

			QString serialCode = ConfigData::inst()->getSerialKey();
			auto alert = new SerialBackupMailView(serialCode, window());
			alert->show();
			});
	}
	mailButton->hide();


	auto comment = new CustomLabel(15, "#FFFFFF", false, 0, 100, this);
	//프로그램 삭제 후 설치 하실때 시리얼코드가 필요합니다. 시리얼 코드 백업 후 삭제 해주세요.
	comment->setText("※プログラム削除後、再インストールをする際はシリアル番号が必要になります\r\n    削除前にシリアル番号を保存してください");
	comment->setGeometry(333 - marginsize_x, 355 - marginsize_y, 600, 100);
	comment->show();


	//auto refreshLabel = new CustomLabel(16, "#FFFFFF", false, 0, 0, this);
	//refreshLabel->setGeometry(333 - marginsize_x, 445 - marginsize_y, 400, 20);
	//refreshLabel->setText("※決済情報の変更後、[更新] ボタンを押してください。");
	//refreshLabel->hide();

	//auto refreshButton = new Button(this);
	//refreshButton->setGeometry(333 - marginsize_x, 475 - marginsize_y, 120, 30);
	//{
	//	QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/smartopti/resource/preview/smartopti/datebtn.png")).arg("QPushButton {color: white; font-size: 10pt; }");
	//	refreshButton->setStyleSheet(ss);
	//	refreshButton->setText(QObject::tr("更新"));//새로고침
	//	connect(refreshButton, &Button::clicked, [=]() {
	//		});
	//}
	//refreshButton->hide();


	 //일반결제
	//결제하기 버튼 우선 제거

	QString ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/okbtn.png")).arg("QPushButton{font-weight:bold;}");
	QString un_ss = QString("%1%2").arg(ssdefines::imageWidget.arg(":/preview/resource/preview/cancel.png")).arg("QPushButton{font-weight:bold;}");

	//구독
	auto subscribeButton = new Button(this);
	subscribeButton->setGeometry(353 - marginsize_x, 450 - marginsize_y, 203, 36);
	{
		subscribeButton->setStyleSheet(ss);
		subscribeButton->setText(QObject::tr("サブスクリプションの有効")); //구독 하기
		connect(subscribeButton, &Button::clicked, [=]() {

			AccountController::inst()->getPayMentURL();

			});
	}
	subscribeButton->hide();

	auto paymentHistoryButton = new Button(this);
	paymentHistoryButton->setGeometry(353 - marginsize_x, 495 - marginsize_y, 203, 36);
	{
		paymentHistoryButton->setStyleSheet(ss);
		paymentHistoryButton->setText(QObject::tr("決済履歴の確認")); //결제이력 확인
		connect(paymentHistoryButton, &Button::clicked, [=]() {

			QUrl url(QString("%1/policy/#/account/%2/paymentList/ForWebView").arg(AUTHSERVER_URL).arg(ConfigData::inst()->getSerialKey()));
			QDesktopServices::openUrl(url);

			});
	}
	paymentHistoryButton->hide();

	auto paymentresetButton = new Button(this);
	paymentresetButton->setGeometry(353 - marginsize_x, 450 - marginsize_y, 203, 36);
	{
		paymentresetButton->setStyleSheet(ss);
		paymentresetButton->setText(QObject::tr("決済情報の変更")); //결제정보 변경
		connect(paymentresetButton, &Button::clicked, [=]() {

			AccountController::inst()->getExPayMentURL();

			});
	}
	paymentresetButton->hide();


	auto unSubscribeButton = new Button(this);
	unSubscribeButton->setGeometry(353 - marginsize_x, 570 - marginsize_y, 203, 36);
	{
		unSubscribeButton->setStyleSheet(un_ss);
		unSubscribeButton->setText(QObject::tr("サブスクリプションの解約")); //구독 취소하기
		connect(unSubscribeButton, &Button::clicked, [=]() {
			//web page call
			//구독 취소 시 기존 구독된 만료일(%1)까지만 사용하실 수 있습니다.\r\n구독 취소 할까요?
			QString alertMsg = QString("サブスクリプションを解約した場合、\r\n満了日(%1)まではご利用可能です。\r\n\r\n解除しますか？").arg(ConfigData::inst()->getExpiredDate());

			auto alert = new AlertPopup(alertMsg, QObject::tr("削除"), QObject::tr("cancel"), true, window());
			connect(alert, &AlertPopup::okClicked, [=]() {

				AccountController::inst()->TerminateAutoPay();

				///api/billing/automaticPaymentTerminate/<str:serialKey>
				//QUrl url(QString("%1api/billing/automaticPaymentTerminate/%1").arg(AUTHSERVER_URL).arg(ConfigData::inst()->getSerialKey()));
				//QDesktopServices::openUrl(url);
				//문구변경
				// 
				//QCoreApplication::quit(); //종료

				});
			alert->show();

			});
	}
	unSubscribeButton->hide();
	//*/

	connect(AccountController::inst(), &AccountController::AuthInfoUpdate, this, [=]() {//receive auth info

		//구독상태
		QString subscribetext_bgcolor;  QString subscribetext_color;
		QString subscribetext;

		//월정액 판단
		bool monthyPay = false;
		if (!ConfigData::inst()->getContractId().isEmpty())
			monthyPay = true;

		if (monthyPay == true) {
			subscribetext_color = "#2E7D32";
			subscribetext = "月額"; //월정액

			exprired->hide();
			subscribeButton->hide();
			paymentresetButton->hide();
			unSubscribeButton->hide();
			paymentHistoryButton->hide();
		}
		else {

			//refreshLabel->show();
			//refreshButton->show();
			paymentHistoryButton->show();
			if (ConfigData::inst()->isSubscribe()) { //구독 상태 확인
			//subscribetext_bgcolor = "#E8F5E9";
				subscribetext_color = "#2E7D32";
				subscribetext = "有効"; //구독
				paymentresetButton->show();
				unSubscribeButton->show();
			}
			else {

				//해약 상태는 우선 제거(2024.03.21)
				//subscribetext_color = "#C62828";
				//subscribetext = "解約"; //해약

				subscribetext_color = "#81C784";
				subscribetext = "無料試用期間"; //무료사용
				subscribeButton->show();
			}


			//만료일
			QString expriredtext = ConfigData::inst()->getExpiredDate();
			QString nextPaymentText;

			//우선 만료일 만 나오도록
			nextPaymentText = "満了日"; //만료일
			/*
			if (ConfigData::inst()->isSubscribe())
				nextPaymentText = "次回の決済日"; //다음 결제 일
			else
				nextPaymentText = "満了日"; //만료일
			*/

			exprired->setText(QString("※ %1 : <span style=\"color:#FDBB30; font-weight:bold;\">%2</span>")
				.arg(nextPaymentText)
				.arg(expriredtext));
			exprired->update();
		}

		subscribelabel->setText(QString("✓ 現在のお客様のサブスクリプション状況は <span style=\"color:%1; font-weight:bold;\">%2</span> です")
			.arg(subscribetext_color)
			.arg(subscribetext));



		//시리얼 코드
		serialkeylabel->setText(QString("✓ シリアル番号 : <span style=\"color:#FDBB30; font-weight:bold;\">%1</span>")
			.arg(ConfigData::inst()->getSerialKey()));
		serialkeylabel->update();

		serialBackup->show();
		copyButton->show();
		mailButton->show();
		});
	

	connect(AccountController::inst(), &AccountController::AuthDetailComplete, this, [=]() {//receive auth info

		//구독상태
		QString subscribetext_bgcolor;  QString subscribetext_color;
		QString subscribetext;

		//월정액 판단
		bool monthyPay = false;
		if (!ConfigData::inst()->getContractId().isEmpty())
			monthyPay = true;

		if (monthyPay == true) {
			subscribetext_color = "#2E7D32";
			subscribetext = "月額"; //월정액

			exprired->hide();
			subscribeButton->hide();
			paymentresetButton->hide();
			unSubscribeButton->hide();
			paymentHistoryButton->hide();
		}
		else {

			if (ConfigData::inst()->isSubscribe()) { //구독 상태 확인
			//subscribetext_bgcolor = "#E8F5E9";
				subscribetext_color = "#2E7D32";
				subscribetext = "有効"; //구독


				subscribeButton->hide();
				paymentresetButton->show();
				unSubscribeButton->show();

			}
			else {
				subscribetext_color = "#81C784";
				subscribetext = "無料試用期間"; //무료사용
				subscribeButton->show();
			}

			//만료일
			QString expriredtext = ConfigData::inst()->getExpiredDate();
			QString nextPaymentText;

			//우선 만료일 만 나오도록
			nextPaymentText = "満了日"; //만료일
			exprired->setText(QString("※ %1 : <span style=\"color:#FDBB30; font-weight:bold;\">%2</span>")
				.arg(nextPaymentText)
				.arg(expriredtext));
			exprired->update();
		}

		subscribelabel->setText(QString("✓ 現在のお客様のサブスクリプション状況は <span style=\"color:%1; font-weight:bold;\">%2</span> です")
			.arg(subscribetext_color)
			.arg(subscribetext));
		});

	connect(AccountController::inst(), &AccountController::OpenPayMentURL, this, [=](const QString url, const int rescode) {

		if (url.isEmpty() || rescode != 0) {

			QString errormsg = QString(tr("決済情報を確認できません。 \r\n再起動後、もう一度お試しください。\r\n\r\nもし同じ状態が続く場合は、\r\nカスタマーサポート Eメール(helpdesk@tni-sc.com)までお問い合わせください。\r\n%1")).arg(url);

			auto alert = new AlertPopup(errormsg, QObject::tr("ok"), "", true, window());//결제정보를 확인 할 수 없습니다. 재시작 후 다시 시도해주세요. 혹시 같은 증상이 계속된다면 고객센터 이메일로 문의 주세요.
			alert->show();
			return;
		}
		else {
			//결제정보 등록 페이지를 실행 합니다.
			auto alert = new AlertPopup(tr("決済情報登録ページを実行します。\r\n決済情報を登録した後、プログラムを再実行してください。\r\n\r\n"
				"# クレジットカードの初回登録の際は、カード確認ため100円が決済され、その後決済は自動で取り消されます。\r\n"
				"# 決済に関するお問い合わせはカスタマーサポート(050-5526-9848)にご連絡ください。"), QObject::tr("ok"), "", true, window());
			alert->show();
			connect(alert, &AlertPopup::okClicked, [=]() {
				QDesktopServices::openUrl(QUrl(url));
				QCoreApplication::quit(); //종료
				});

		}

		
		});

	connect(AccountController::inst(), &AccountController::OpenExPayMentURL, this, [=](const QString url, const int rescode) {
		if (url.isEmpty() || rescode != 0) {

			QString errormsg = QString(tr("決済情報を確認できません。 \r\n再起動後、もう一度お試しください。\r\n\r\nもし同じ状態が続く場合は、\r\nカスタマーサポート Eメール(helpdesk@tni-sc.com)までお問い合わせください。\r\n%1")).arg(url);

			auto alert = new AlertPopup(errormsg, QObject::tr("ok"), "", true, window());//결제정보를 확인 할 수 없습니다. 재시작 후 다시 시도해주세요. 혹시 같은 증상이 계속된다면 고객센터 이메일로 문의 주세요.
			alert->show();
			return;
		}
		else {
			//결제정보 등록 페이지를 실행 합니다.
			auto alert = new AlertPopup(tr("決済情報登録ページを実行します。\r\n決済情報を登録した後、プログラムを再実行してください。\r\n\r\n"
				"# クレジットカードの初回登録の際は、カード確認ため100円が決済され、その後決済は自動で取り消されます。\r\n"
				"# 決済に関するお問い合わせはカスタマーサポート(050-5526-9848)にご連絡ください。"), QObject::tr("ok"), "", true, window());
			alert->show();
			connect(alert, &AlertPopup::okClicked, [=]() {
				QDesktopServices::openUrl(QUrl(url));

				//QCoreApplication::quit(); //종료
				});
		}
		});


	connect(AccountController::inst(), &AccountController::AutoPayTerminateComplete, this, [=](const QString res_msg, const int res_code) {

		if (res_code != 0) {

			QString errormsg = QString(tr("決済情報を確認できません。 \r\n再起動後、もう一度お試しください。\r\n\r\nもし同じ状態が続く場合は、\r\nカスタマーサポート Eメール(helpdesk@tni-sc.com)までお問い合わせください。\r\n%1")).arg(res_msg);

			auto alert = new AlertPopup(errormsg, QObject::tr("ok"), "", true, window());//결제정보를 확인 할 수 없습니다. 재시작 후 다시 시도해주세요. 혹시 같은 증상이 계속된다면 고객센터 이메일로 문의 주세요.
			alert->show();
			return;
		}
		else {

			auto alert = new AlertPopup(QString(tr("購入キャンセルが完了しました")), QObject::tr("ok"), "", false, window());//구독취소 완료 되었습니다.
			alert->show();

			QString subscribetext_color = "#C62828";
			QString subscribetext = "自動購入終了"; //자동 구독 종료

			subscribelabel->setText(QString("✓ 現在のお客様のサブスクリプション状況は <span style=\"color:%1; font-weight:bold;\">%2</span> です")
				.arg(subscribetext_color)
				.arg(subscribetext));
			subscribelabel->update();

			subscribeButton->show();
			paymentresetButton->hide();
			unSubscribeButton->hide();
		}

		});

	setFixedSize(parent->size());


	/*connect(webManager, &WebPageCaller::dataFetched, this, [this](const QString& data, const QUrl& url) {

		if (url.toString() == "https://www.google.com") {
			QMessageBox::information(this, "Result from Google", data.left(500));
		}
		else if (url.toString() == "https://www.bing.com") {
			QMessageBox::information(this, "Result from Bing", data.left(500));
		}
	});*/
}

AccountView::~AccountView()
{}
