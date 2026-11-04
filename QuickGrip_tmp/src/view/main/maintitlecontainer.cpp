#include "maintitlecontainer.h"


#include "view/Component/button.h"
#include "view/Component/dragger.h"

#include "../../ui.h"
#include "../../common/defines/ssdefins.h"

MainTitleContainer::MainTitleContainer(QWidget *parent)
	: QWidget(parent)
{
    setFixedHeight(50);
    setStyleSheet("background-color: #123456; border: 1px solid #666666; ");

    auto topLayout = new ContainerLayout(this);
    auto dragger = new Dragger(this);
    dragger->setFocusPolicy(Qt::StrongFocus);
    topLayout->addWidget(dragger);

    auto homebtn = new Button;
    homebtn->setFixedSize(30, 40);
    topLayout->addWidget(homebtn);

}

MainTitleContainer::~MainTitleContainer()
{}
