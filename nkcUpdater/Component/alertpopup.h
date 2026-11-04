#pragma once

#include <QObject>
#include <QWidget>
#include <QKeyEvent>

class AlertPopup  : public QWidget
{
	Q_OBJECT

public:
	AlertPopup(QString body, QString ok, QString cancel, bool big, QWidget*parent = Q_NULLPTR);
	~AlertPopup();

protected:
	void keyPressEvent(QKeyEvent* event) override;
	QString toRichText(const QString& txt, const int lineHeight);

	void runmodal();
	void closemodal();

signals:
	void okClicked();
	void cancelClicked();
};
