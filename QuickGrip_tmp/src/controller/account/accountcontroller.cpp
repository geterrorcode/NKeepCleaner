#include "accountcontroller.h"

#include <src/common/network/autochecker.h>

#include <view/account/accountview.h>

AccountController::AccountController(QObject* parent)
	: QObject(parent)
{

	QObject::connect(AutoChecker::inst(), &AutoChecker::resCallPaymentURL, this, [=](const QString url, const int rescode) {
		emit OpenPayMentURL(url, rescode);
		});

	QObject::connect(AutoChecker::inst(), &AutoChecker::resCallExPaymentURL, this, [=](const QString url, const int rescode) {
		emit OpenExPayMentURL(url, rescode);
		});


	QObject::connect(AutoChecker::inst(), &AutoChecker::resAutoPayTerComplete, this, [=](const QString res_msg, const int res_code) {
		emit AutoPayTerminateComplete(res_msg, res_code);
		});


	//connect(AccountController::inst(), &AccountController::expiredDateUpdate, [=]() {

	//	emit expiredDateLabelUpdate();
	//	});

}

AccountController::~AccountController()
{}


void AccountController::getPayMentURL()
{
	AutoChecker::inst()->getPaymentURL();
}

void AccountController::getExPayMentURL()
{
	AutoChecker::inst()->getExchangePaymentURL();
}

void AccountController::TerminateAutoPay()
{
	AutoChecker::inst()->SetAutoPayTerminate();
}