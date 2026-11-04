#include "rcv.h"

#include <QMap>

namespace Qux {
namespace UI {

const Receiver* rcv(QWidget* widget) 
{    
    static auto receiverMap = new QMap<QWidget*, Receiver*>;
    
    if (receiverMap->contains(widget)) {
        return receiverMap->value(widget);
    }

    auto receiver = new Receiver(widget);
    receiverMap->insert(widget, receiver);

    QObject::connect(widget, &QObject::destroyed, [widget] () {
        receiverMap->remove(widget);
    });

    return receiver;
}

}}