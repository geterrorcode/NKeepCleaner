#include "keymodelbind.h"

namespace Qux {
namespace Model {

void bind(const AbstractKeyModel* model, const QString& key, QObject* context, std::function<void (const ModelItem&)> func)
{
    {
        auto update = [key, func] (const QList<QPair<ModelItem, ModelItem>>& pairs) {
            foreach (auto& pair, pairs) {
                if (key == pair.first) {
                    func(pair.second);
                    return;
                }
            }
        };
        QObject::connect(model, &AbstractKeyModel::inserted, context, update);
        QObject::connect(model, &AbstractKeyModel::updated, context, update);
    }

    QObject::connect(model, &AbstractKeyModel::removed, context, [key, func] (const QList<ModelItem> keys) {
        if (keys.contains(key)) {
            func(ModelItem());
        }
    });

    func(model->value(key));
}

}
}