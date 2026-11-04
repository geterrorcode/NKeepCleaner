#pragma once

#include <QWidget>
#include <QLabel>
#include <QPointer>
#include <QTimer>

class TrayPopup  : public QWidget
{
	Q_OBJECT

public:
	TrayPopup(const QString& body, QWidget* parent = nullptr);
	~TrayPopup();


private:
	QPointer<QLabel> _bodyLabel;
	QPointer<QTimer> _traytimer;
	

public:
	void setBody(QString body);
	void startTimer(int msec);
	void stopTimer();

};
