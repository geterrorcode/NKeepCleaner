#pragma once

#include <QObject>
#include <QApplication>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QObject>
#include <QEvent>
#include <QHelpEvent>
#include <QToolTip>

class OptimizeFilesHintEventHandler  : public QObject
{
	Q_OBJECT

public:
	OptimizeFilesHintEventHandler(QObject *parent = Q_NULLPTR);
	~OptimizeFilesHintEventHandler();

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;
};
