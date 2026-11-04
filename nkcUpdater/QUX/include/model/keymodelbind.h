#ifndef QUX_MODEL_KEYMODELBIND_H
#define QUX_MODEL_KEYMODELBIND_H

#include "abstractkeymodel.h"

#include <functional>

namespace Qux {
namespace Model {

void bind(const AbstractKeyModel* model, const QString& key, QObject* context, std::function<void (const ModelItem&)> func);

template <typename T, typename UnaryFunc>
void bind(const AbstractKeyModel* model, const QString& key, QObject* context, UnaryFunc func)
{
    bind(model, key, context, [func] (const ModelItem& value) {
        func(value.value<T>());
    });
}

template <typename T, typename UnaryOp>
void bind(const AbstractKeyModel* model, const QString& key, QObject* context, const QString& contextProperty, UnaryOp op)
{
    bind<T>(model, key, context, [context, contextProperty, op] (const T& value) {
        context->setProperty(qPrintable(contextProperty), op(value));
    });
}

}
}

#endif