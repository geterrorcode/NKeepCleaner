#include "tablecheckbox.h"


#include <QHBoxLayout>


#include "../../common/defines/ssdefins.h"

TableCheckBox::TableCheckBox(QWidget *parent)
	: QWidget(parent), _checkbox(Q_NULLPTR), _check_status(false)
{
    _checkbox = new QCheckBox();      // We declare and initialize the checkbox
    _checkbox->setStyleSheet(ssdefines::checkboxTable);
    _checkbox->setFixedSize(16,16);



    QHBoxLayout* layoutCheckBox = new QHBoxLayout(this); // create a layer with reference to the widget
    layoutCheckBox->addWidget(_checkbox);            // Set the checkbox in the layer
    layoutCheckBox->setAlignment(Qt::AlignCenter);  // Center the checkbox
    layoutCheckBox->setContentsMargins(0, 0, 0, 0);    // Set the zero padding
}

TableCheckBox::~TableCheckBox()
{
    if(_checkbox)
        _checkbox->deleteLater();
}

bool TableCheckBox::isChecked()
{
    
    return  _checkbox->isChecked();
    //return _check_status;
}

void TableCheckBox::setChecked(bool check)
{
    _checkbox->setChecked(check);
}