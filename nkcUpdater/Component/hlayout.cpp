#include "hlayout.h"

HLayout::HLayout(QWidget* parent)
    : QHBoxLayout(parent)
{
    setContentsMargins(0, 0, 0, 0);
    setSpacing(0);
}