#pragma once

#include <QWidget>
#include <QLabel>

#include "view/Component/containerlayout.h"

#include "../../common/defines/ssdefins.h"


class TopInfoWidget  : public QWidget
{
	Q_OBJECT

public:
	TopInfoWidget(const QString bgimg, const QString detailimg, QWidget *parent = Q_NULLPTR);
	~TopInfoWidget();
};
