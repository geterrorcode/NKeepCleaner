#include "../include/flux/simplemiddleware.h"
#include "../include/flux/dispatcher.h"

namespace Qux {
namespace Flux {

SimpleMiddleware::SimpleMiddleware(std::function<void (const Action& action)> reducer, QObject* parent)
    : QObject(parent)
{
    connect(&Dispatcher::inst(), &Dispatcher::received, this, reducer);
}

}}