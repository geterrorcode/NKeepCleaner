#include "../include/flux/middleware.h"
#include "../include/flux/dispatcher.h"

namespace Qux {
namespace Flux {

Middleware::Middleware(QObject* parent)
    : QObject(parent)
{
    connect(&Dispatcher::inst(), &Dispatcher::received, this, &Middleware::reduce);
}

}}