#ifndef QUX_UI_DYNAMICLISTVIEW_H
#define QUX_UI_DYNAMICLISTVIEW_H

#include "../model/abstractlistmodel.h"
#include "../defineproperty.h"
#include "../functiontype.h"

#include <QWidget>

namespace Qux {
namespace UI {

class DynamicListView : public QWidget
{
    Q_OBJECT

    PROPERTY(const Model::AbstractListModel*, model, setModel)
    PROPERTY(int, widgetHeight, setWidgetHeight)
    PROPERTY_NOCOMP(UnaryOp<Model::ModelItem>::Ret<QWidget*>::Type, widgetCreator, setWidgetCreator)
    PROPERTY(QRect, visibleArea, setVisibleArea)

public:
    DynamicListView(QWidget* parent = nullptr); \

};

}
}

#endif
