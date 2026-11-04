#pragma once

#include <QObject>
#include <QWidget>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QSize>

class AlertPopup  : public QWidget
{
	Q_OBJECT

public:
	AlertPopup(QString body, QString ok, QString cancel, bool big,
		QWidget* parent = Q_NULLPTR, const QSize customsize = QSize());
	~AlertPopup();

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

private:
	bool _resultEmitted = false;

signals:
	void okClicked();
	void cancelClicked();
};
