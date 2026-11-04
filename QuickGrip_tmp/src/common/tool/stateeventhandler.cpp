#include "stateeventhandler.h"



StateEventHandler::StateEventHandler(QWidget *parent)
	: QMainWindow(parent)
{}

StateEventHandler::~StateEventHandler()
{}


void StateEventHandler::changeEvent(QEvent* event) 
{
    if (event->type() == QEvent::WindowStateChange) {
        if (windowState() & Qt::WindowMinimized) {
            qDebug() << "Window minimized";


        }
    }
    QMainWindow::changeEvent(event);
}