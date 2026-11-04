#pragma once

#include <QWidget>
#include <QMouseEvent>

#include "view/Component/customlabel.h"

class OptimizeCompleteResultWidget  : public QWidget
{
	Q_OBJECT

public:
	OptimizeCompleteResultWidget(const QString imagpath, const QString title, QWidget *parent = Q_NULLPTR);
	~OptimizeCompleteResultWidget();


	void setOptiCompleteText(QString success_count, QString success_size, QString fail_count, QString fail_size);

private:
	void mousePressEvent(QMouseEvent* event) override;

	CustomLabel* _success_count;
	CustomLabel* _success_size;
	CustomLabel* _fail_count;
	CustomLabel* _fail_size;

	CustomLabel* _successlabel;
	CustomLabel* _faillabel;
	

Q_SIGNALS:
	void sendCompleteResultClicked();

};
