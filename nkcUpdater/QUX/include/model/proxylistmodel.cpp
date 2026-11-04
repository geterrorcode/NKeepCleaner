#include "proxylistmodel.h"
#include "../model/filteredlistmodel.h"
#include "../model/sortedlistmodel.h"

namespace Qux {
namespace Model {

ProxyListModel::ProxyListModel(const AbstractListModel* model, QObject* parent)
    : AbstractListModel(parent)
{
    auto filteredModel = new FilteredListModel(model, this);
    auto sortedModel = new SortedListModel(filteredModel, this);

    connect(this, &ProxyListModel::filterChanged, filteredModel, &FilteredListModel::setFilter);
    connect(this, &ProxyListModel::comparerChanged, sortedModel, &SortedListModel::setComparer);

    m_outputModel = sortedModel;
    connect(m_outputModel, &AbstractListModel::inserted, this, &AbstractListModel::inserted);
    connect(m_outputModel, &AbstractListModel::removed, this, &AbstractListModel::removed);
    connect(m_outputModel, &AbstractListModel::updated, this, &AbstractListModel::updated);
    connect(m_outputModel, &AbstractListModel::countChanged, this, &AbstractListModel::countChanged);
}

}
}