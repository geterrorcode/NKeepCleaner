#include "../include/flux/store.h"
#include "../include/flux/dispatcher.h"

namespace Qux {
namespace Flux {

Store::Store(QObject* parent)
    : QObject(parent)
{
    connect(&Dispatcher::inst(), &Dispatcher::received, this, &Store::reduce);
}

}}