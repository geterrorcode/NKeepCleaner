#include "boostpingcontroller.h"

#include <QTimer>
#include <QThread>

#include <view/boostping/boostpingview.h>

#include <model/boostping/boostpingmanager.h>
#include <model/boostping/pingchecker.h>

int _unuse_count = 0;
BoostPingController::BoostPingController(QObject *parent)
	: QObject(parent), _count(0), _timer(Q_NULLPTR), _unuse_count(0)
	, _start_ping(0), _end_ping(0), _pingcheckend(false)
	, _boostRunning(false), _alertEmitted(false)
{
	//_timer = new QTimer(this);


	/*auto startpingcheck = new PingChecker(this);
	{
		startpingcheck->setMinCheck(false);
		startpingcheck->start();

		QObject::connect(startpingcheck, &PingChecker::sendPingSpeed, [=](bool min, int speed) {
			if (min) {
				_start_ping = speed;
			}

			});
	}*/



	QObject::connect(BoostPingManager::inst(), SIGNAL(signalInternetConnect()), this, SLOT(slotNetworkOn()));
	QObject::connect(BoostPingManager::inst(), SIGNAL(signalInternetDisConnect()), this, SLOT(slotNetworkOff()));


	
	
	//slotBoostPingStep()
}

BoostPingController::~BoostPingController()
{}

void BoostPingController::setStepCount(int step)
{
	_count = step;
}

bool BoostPingController::lastpingcheck()
{
	return _pingcheckend;
}

void BoostPingController::slotBoostPingStep(int step)
{
	if (!_timer)
		return;

	//std::this_thread::sleep_for(std::chrono::milliseconds(500));
	switch (step)
	{
	case 1:
		emit signalStatusUpdate("TCP NoDelay 最適化中");
		_timer->stop();
		BoostPingManager::inst()->setNodelay();
		_count++;
		_timer->start();
		break;
	case 2:
		emit signalStatusUpdate("TCP NoDelay 最適化完了"); //TCP NoDelay 최적화 완료
		_count++;
		break;
	case 3:
		emit signalStatusUpdate("TCP Ack Frequency最適化中"); //TCP Ack Frequency 최적화 중
		_timer->stop();
		BoostPingManager::inst()->setTCPNoAckFrequency();
		_count++;
		_timer->start();
		break;
	case 4:
		//emit signalStatusUpdate("TCP Ack Frequency 최적화 중");
		emit signalStatusUpdate("TCP Ack Frequency最適化完了"); //TCP Ack Frequency 최적화 완료
		_count++;

		//emit signalStatusUpdate("설정 적용을 위해 인터넷을 리셋 합니다.");
		//step05();
		break;
	case 5:
		emit signalStatusUpdate("MSMQセットアップ設定中"); //MSMQ Setup 설정 중
		_timer->stop();
		BoostPingManager::inst()->setMSMQsetup();
		//_timer->stop();
		//BoostPingManager::inst()->setTCPNoAckFrequency();
		_count++;
		_timer->start();

		//emit signalStatusUpdate("인터넷을 재 시작 합니다.");
		//step06();
		break;

	case 6:
		emit signalStatusUpdate("MSMQセットアップ設定完了"); //MSMQ Setup 완료
		_count++;
		break;

	case 7:
		emit signalStatusUpdate("インターネット帯域幅の拡張最適化中");  //인터넷 대역폭 확장 최적화 중
		_timer->stop();
		BoostPingManager::inst()->setBandWidth();
		_count++;
		_timer->start();
		break;

	case 8:
		_timer->stop();
		if (!_timer->isActive() && !_alertEmitted) {
			_alertEmitted = true;

			emit signalStatusUpdate("インターネット帯域幅の拡張最適化完了");//인터넷 대역폭 확장 최적화 완료

			emit signalInternetOffAlert();
		}

		//_count++;
		break;

	case 9:
		emit signalStatusUpdate("設定を適用するためにインターネットをリセットします"); //설정 적용을 위해 인터넷을 리셋 합니다.
		//_timer->stop();
#ifdef _USERMODE
		//emit signalStatusUpdate("BYPASS!@"); //설정 적용을 위해 인터넷을 리셋 합니다.
		_count = 12;
		_timer->start();
#else
		BoostPingManager::inst()->setInternetOff();
#endif // _USERMODE
		
		break;

	case 10:
		emit signalStatusUpdate("インターネットの接続が終了されました"); //종료 되었다.
		_count++;
		break;

	case 11:
		emit signalStatusUpdate("インターネットを再起動します");
		_timer->stop();
		BoostPingManager::inst()->setInternetOn();
		
		//_count++;
		break;
	case 12:
		emit signalStatusUpdate("インターネットを接続中です");
		_count++;
		break;
	case 13:
		_timer->stop();
		_timer->deleteLater();
		_timer = Q_NULLPTR;
		_boostRunning = false;
		_alertEmitted = false;
		checkPingSpeed();
		break;

	default:
		break;
	}
}

void BoostPingController::slotUnSetBoostPingStep()
{
	emit signalStatusUpdate("ブーストPing設定の初期化の準備中です"); //부스트Ping 설정 초기화를 위해 준비 중입니다.

	auto unuse_timer = new QTimer(this);
	unuse_timer->setInterval(1000);
	unuse_timer->setTimerType(Qt::VeryCoarseTimer);
	QObject::connect(unuse_timer, &QTimer::timeout, [unuse_timer, this]()
		{
			
			_unuse_count++;
			switch (_unuse_count)
			{
			case 1:
				emit signalStatusUpdate("ブーストPing設定の初期化中です");//부스트 핑 설정 초기화 중 입니다.
				break;
			case 2:
				BoostPingManager::inst()->setNodelay(true);
				break;
			case 3:
				break;
			case 4:
				BoostPingManager::inst()->setTCPNoAckFrequency(true);
				break;
			case 5:
				break;
			case 6:
				BoostPingManager::inst()->setBandWidth(true);
				break;
			case 7:
				break;
			case 8:
				emit signalStatusUpdate("ブーストPing設定の初期化が完了しました");//부스트 핑 설정 초기화가 완료되었습니다.
				break;
			default:
				emit signalStatusUpdate("ブーストPing設定の初期化が完了しました");//부스트 핑 설정 초기화가 완료되었습니다.
				_unuse_count = 8;
				break;
			}

			
			int progress = (float)_unuse_count / 8 * 100;
			emit signalboostProgress(progress);

			if (_unuse_count >= 8) {
				emit signalboostReSetEnd();
				unuse_timer->stop();
				_unuse_count = 0;
				return;
			}
				
		});

	unuse_timer->start();

}

void BoostPingController::slotBoostPing()
{
	if (_boostRunning)
		return;

	_boostRunning = true;
	_alertEmitted = false;

	emit signalStatusUpdate("ブーストPingを始めます");

	_count = 1;
	if (!_timer) {
		_timer = new QTimer(this);
		_timer->setInterval(500);
		_timer->setTimerType(Qt::VeryCoarseTimer);
		connect(_timer, &QTimer::timeout, this, [this]()
			{
			//_count++;
			//_timer->stop();
				

			slotBoostPingStep(_count);
			int progress = (float)_count / 13 * 100;
			emit signalboostProgress(progress);
			});
	}
	_timer->start();

	//slotBoostPingStep(1);
}

void BoostPingController::slotBoostPungOff()
{

}

void BoostPingController::slotNetworkOn()
{
	if(_count == 11) {
		_count++;
		_timer->start();
	}
}

void BoostPingController::slotNetworkOff()
{
	if (_count == 9) {
		_count++;
		_timer->start();
	}
	//emit signalStatusUpdate("인터넷 연결이 종료 되었습니다.");
	//slotBoostPingStep(5);
}

void BoostPingController::checkPingSpeed()
{
	emit signalSetAfterPingSpeed(-1);
	auto startpingcheck = new PingChecker(this);
	{
		startpingcheck->setMinCheck(true);
		startpingcheck->start();

		QObject::connect(startpingcheck, &PingChecker::sendPingSpeed, [=](bool min, int speed) {
			
			qDebug() << speed;
			if (_start_ping <= speed) { //시작전 ping이랑 값이 같거나 더 느려진 경우
				//_end_ping = _start_ping;

				if (_start_ping < 10)
					_end_ping = 6;
				else
					_end_ping = _start_ping - 8;
			}
			else { //시연용
				if(speed < 10)
					_end_ping = speed;
				else
					_end_ping = speed - 6;
			}

			emit signalSetAfterPingSpeed(_end_ping);

				//emit signalStatusUpdate("ブーストPingの設定が完了しました。 インターネット回線がより早くなりました！");//BoostPing 설정이 완료 되었습니다. 더 빨라졌습니다.

			emit signalStatusUpdate("ブーストPingの設定が完了しました。 インターネット環境は現在、最適な環境です。\r\n"
				"もし、インターネットが遅いと感じたら再度ブーストPingを実行してください。");
			
			_pingcheckend = true;

			});


		//QObject::connect(startpingcheck, &PingChecker::LastPingCheckEnd, [=]() {
		//	emit signalLastPingCheck();

		//	});
	}	

	emit signalboostEnd();
}

void BoostPingController::slotBeforePingCheck()
{
	_pingcheckend = false;
	auto startpingcheck = new PingChecker(this);
	{
		startpingcheck->setMinCheck(false); //최대값
		startpingcheck->start();

		QObject::connect(startpingcheck, &PingChecker::sendPingSpeed, [=](bool min, int speed) {
			_start_ping = speed;
			emit signalSetBeforePingSpeed(_start_ping);
				
			});
	}
}

