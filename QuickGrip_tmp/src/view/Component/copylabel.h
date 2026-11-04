#pragma once

#include <QObject>
#include <QTextEdit>

class CopyLabel  : public QTextEdit
{
	Q_OBJECT

public:

public:
	explicit CopyLabel(size_t fontsize, bool bold, QString color,  QWidget* parent = Q_NULLPTR);
	//CopyLabel(QObject *parent);
	~CopyLabel();

protected:
	void keyPressEvent(QKeyEvent* event) override;
};
