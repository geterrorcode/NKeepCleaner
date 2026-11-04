#ifndef QUX_ASYNC_WAITFORSIGNAL_H
#define QUX_ASYNC_WAITFORSIGNAL_H

#include <QEventLoop>

namespace Qux {
namespace Async {

template <typename T, typename Func0>
void waitForSignal(const T* sender, Func0 signal0)
{
    QEventLoop loop;
    QObject::connect(sender, signal0, &loop, &QEventLoop::quit);
    loop.exec();
}

template <typename T, typename Func0, typename Func1>
void waitForSignal(const T* sender, Func0 signal0, Func1 signal1)
{
    QEventLoop loop;
    QObject::connect(sender, signal0, &loop, &QEventLoop::quit);
    QObject::connect(sender, signal1, &loop, &QEventLoop::quit);
    loop.exec();
}

template <typename T, typename Func0, typename Func1, typename Func2>
void waitForSignal(const T* sender, Func0 signal0, Func1 signal1, Func2 signal2)
{
    QEventLoop loop;
    QObject::connect(sender, signal0, &loop, &QEventLoop::quit);
    QObject::connect(sender, signal1, &loop, &QEventLoop::quit);
    QObject::connect(sender, signal2, &loop, &QEventLoop::quit);
    loop.exec();
}

}
}

#endif