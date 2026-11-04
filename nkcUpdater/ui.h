#pragma once

#include "QUX/include/utility/qvalue.h"
#include "Qux/include/ui/rcv.h"

#include "Component/button.h"
#include "Component/customlabel.h"
//#include "Component/containerlayout.h"
#include "../QuickGrip_tmp/src/view/Component/containerlayout.h"

#include "Component/hlayout.h"
#include "Component/vlayout.h"


#include "tool/qss.h"
#include <QtWidgets>


using Qux::UI::Receiver;
using Qux::UI::rcv;


//QLayout* wrapWidget(QWidget* widget, int left = 0, int top = 0, int right = 0, int bottom = 0, Qt::Alignment alignment = Qt::Alignment());
QWidget* findWindow(QWidget* widget);
bool isInHierarchy(const QWidget* parent, const QWidget* child);
