#ifndef QUX_BIND_H
#define QUX_BIND_H

#include "utility.h"

#include <QObject>
#include <functional>

namespace Qux {

#define REF(owner, name) owner, #name
#define BOUND REF

void bind(const QList<QPair<const QObject*, QString>>& propertyRefs, QObject* context, std::function<void (const QList<QVariant>& properties)> func);

template <typename T0, typename UnaryOp>
void bind(const QObject* sender0, const QString& property0,
          QObject* context, const QString& contextProperty, UnaryOp op)
{
    bind(toList(qMakePair(sender0, property0)), context,
    [context, contextProperty, op] (const QList<QVariant>& properties) {
        context->setProperty(qPrintable(contextProperty), op(properties[0].value<T0>()));
    });
}

template <typename T0, typename T1, typename BinaryOp>
void bind(const QObject* sender0, const QString& property0,
          const QObject* sender1, const QString& property1,
          QObject* context, const QString& contextProperty, BinaryOp op)
{
    bind(toList(qMakePair(sender0, property0), qMakePair(sender1, property1)), context,
    [context, contextProperty, op] (const QList<QVariant>& properties) {
        context->setProperty(qPrintable(contextProperty), op(properties[0].value<T0>(), properties[1].value<T1>()));
    });
}

template <typename T0, typename T1, typename T2, typename TernaryOp>
void bind(const QObject* sender0, const QString& property0,
          const QObject* sender1, const QString& property1,
          const QObject* sender2, const QString& property2,
          QObject* context, const QString& contextProperty, TernaryOp op)
{
    bind(toList(qMakePair(sender0, property0), qMakePair(sender1, property1), qMakePair(sender2, property2)), context,
    [context, contextProperty, op] (const QList<QVariant>& properties) {
        context->setProperty(qPrintable(contextProperty), op(properties[0].value<T0>(), properties[1].value<T1>(), properties[2].value<T2>()));
    });
}

template <typename T0, typename MemFunc, typename UnaryOp>
void bind(const QObject* sender0, const QString& property0,
          typename QtPrivate::FunctionPointer<MemFunc>::Object* context, MemFunc setter, UnaryOp op)
{
    bind(toList(qMakePair(sender0, property0)), context,
    [context, setter, op] (const QList<QVariant>& properties) {
        std::mem_fn(setter)(context, op(properties[0].value<T0>()));
    });
}

template <typename T0, typename T1, typename MemFunc, typename BinaryOp>
void bind(const QObject* sender0, const QString& property0,
          const QObject* sender1, const QString& property1,
          typename QtPrivate::FunctionPointer<MemFunc>::Object* context, MemFunc setter, BinaryOp op)
{
    bind(toList(qMakePair(sender0, property0), qMakePair(sender1, property1)), context,
    [context, setter, op] (const QList<QVariant>& properties) {
        std::mem_fn(setter)(context, op(properties[0].value<T0>()), properties[1].value<T1>());
    });
}

template <typename T0, typename T1, typename T2, typename MemFunc, typename TernaryOp>
void bind(const QObject* sender0, const QString& property0,
          const QObject* sender1, const QString& property1,
          const QObject* sender2, const QString& property2,
          typename QtPrivate::FunctionPointer<MemFunc>::Object* context, MemFunc setter, TernaryOp op)
{
    bind(toList(qMakePair(sender0, property0), qMakePair(sender1, property1), qMakePair(sender2, property2)), context,
    [context, setter, op] (const QList<QVariant>& properties) {
        std::mem_fn(setter)(context, op(properties[0].value<T0>()), properties[1].value<T1>(), properties[2].value<T2>());
    });
}

template <typename T0, typename UnaryFunc>
void bind(const QObject* sender0, const QString& property0,
          QObject* context, UnaryFunc func)
{
    bind(toList(qMakePair(sender0, property0)), context,
    [func] (const QList<QVariant>& properties) {
        func(properties[0].value<T0>());
    });
}

template <typename T0, typename T1, typename BinaryFunc>
void bind(const QObject* sender0, const QString& property0,
          const QObject* sender1, const QString& property1,
          QObject* context, BinaryFunc func)
{
    bind(toList(qMakePair(sender0, property0), qMakePair(sender1, property1)), context,
    [func] (const QList<QVariant>& properties) {
        func(properties[0].value<T0>(), properties[1].value<T1>());
    });
}

template <typename T0, typename T1, typename T2, typename TernaryFunc>
void bind(const QObject* sender0, const QString& property0,
          const QObject* sender1, const QString& property1,
          const QObject* sender2, const QString& property2,
          QObject* context, TernaryFunc func)
{
    bind(toList(qMakePair(sender0, property0), qMakePair(sender1, property1), qMakePair(sender2, property2)), context,
    [func] (const QList<QVariant>& properties) {
        func(properties[0].value<T0>(), properties[1].value<T1>(), properties[2].value<T2>());
    });
}

}

#endif