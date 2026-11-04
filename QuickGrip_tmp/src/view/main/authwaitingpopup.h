#pragma once

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QKeyEvent>


class AuthWaitingPopup : public QDialog
{
	Q_OBJECT

public:
	AuthWaitingPopup(bool anyopen,  QWidget *parent = Q_NULLPTR);
	~AuthWaitingPopup();

private:
};
