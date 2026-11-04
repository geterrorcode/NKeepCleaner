#include "listmodel.h"
#include "../utility.h"

namespace Qux {
namespace Model {

ListModel::ListModel(QObject* parent)
    : AbstractListModel(parent)
{}

void ListModel::append(const ModelItem& value)
{
    append(toList(value));
}

void ListModel::append(const QList<ModelItem>& values)
{
    insert(count(), values);
}

void ListModel::clear()
{
    if (m_list.empty()) return;

    auto end = m_list.size() - 1;
    m_list.clear();

    emit removed(0, end);
    emit countChanged(count());
}

void ListModel::insert(int index, const ModelItem& value)
{
    insert(index, toList(value));
}

void ListModel::insert(int index, const QList<ModelItem>& values)
{
    if (values.empty()) return;

    index = std::max(std::min(index, count()), 0);

    foreach (auto& value, reverseList(values)) {
        m_list.insert(index, value);
    }

    emit inserted(index, values);
    emit countChanged(count());
}

void ListModel::removeAt(int index)
{
    if (index < 0 || index >= count()) return;

    m_list.removeAt(index);

    emit removed(index, index);
    emit countChanged(index);
}

void ListModel::update(int index, const ModelItem& value)
{
    if (index < 0 || index >= count()) return;

    m_list[index] = value;

    emit updated(index, toList(value));
}

}
}