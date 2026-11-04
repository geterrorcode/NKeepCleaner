#include "main_view.h"

#include <QApplication>


MainView::MainView(QWidget *parent)
	: QMainWindow(parent)
{

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setFixedSize(1275, 747);

}

MainView::~MainView()
{}
