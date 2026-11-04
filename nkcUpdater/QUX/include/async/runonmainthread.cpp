#include "../../include/async/runonmainthread.h"

#include <QApplication>
#include <QThread>

namespace Qux { 
namespace Async {

namespace {

class Caller : public QObject
{
    Q_OBJECT

public:
    Caller(std::function<void()> func, QObject* parent = nullptr)
        : QObject(parent)
    {
        QObject::connect(this, &Caller::ready, this, [this, func] () {
            func();
            deleteLater();
        });
    }

signals:
    void ready();

};

}

void runOnMainThread(std::function<void()> func)
{
    auto mainThread = QApplication::instance()->thread();
    if (QThread::currentThread() == mainThread) {
        func();
        return;
    }

    auto caller = new Caller(func);
    caller->moveToThread(mainThread);
    emit caller->ready();
}

#include "runonmainthread.moc"

}}