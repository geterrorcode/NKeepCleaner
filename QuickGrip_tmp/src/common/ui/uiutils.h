#pragma once

#include <QObject>
#include <QLayout>

#include "view/Component/containerlayout.h"

namespace Common {
	namespace UI {

		QLayout* exChangeWidget(QWidget* widget, int left = 0, int top = 0, int right = 0, int bottom = 0, Qt::Alignment alignment = Qt::Alignment());

		QWidget* findWindow(QWidget* widget);

		bool isInHierarchy(const QWidget* parent, const QWidget* child);
		



	}
}



//namespace Common {
//	namespace UI {
//		QLayout* exChangeWidget(QWidget* widget, int left, int top, int right, int bottom, Qt::Alignment alignment);
//	}
//}

