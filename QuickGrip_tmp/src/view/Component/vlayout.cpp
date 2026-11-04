#include "vlayout.h"

VLayout::VLayout(QWidget* parent)
    : QVBoxLayout(parent)
{
    setContentsMargins(0, 0, 0, 0);
    setSpacing(0);
}