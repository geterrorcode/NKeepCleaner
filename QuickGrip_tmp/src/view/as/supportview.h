#pragma once

#include <QObject>
#include <QWidget>

class SupportView  : public QWidget
{
	Q_OBJECT

public:
	SupportView(QWidget* parent = Q_NULLPTR);
	~SupportView();
};
