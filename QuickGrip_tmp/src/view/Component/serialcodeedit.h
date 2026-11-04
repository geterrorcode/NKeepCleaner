#pragma once

#include <QApplication>
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>

class SerialCodeEdit  : public QLineEdit
{
	Q_OBJECT

public:
	SerialCodeEdit(QWidget* parent = Q_NULLPTR);
	~SerialCodeEdit();

protected:
    void keyPressEvent(QKeyEvent* event) override; 

private slots:
    void onTextChanged(const QString& text); 

};
