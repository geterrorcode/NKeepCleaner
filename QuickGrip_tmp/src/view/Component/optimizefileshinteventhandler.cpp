#include "optimizefileshinteventhandler.h"
#include <common/defines/commondefine.h>
//#include <QTimer>

OptimizeFilesHintEventHandler::OptimizeFilesHintEventHandler(QObject *parent)
	: QObject(parent)
{}

OptimizeFilesHintEventHandler::~OptimizeFilesHintEventHandler()
{}


bool OptimizeFilesHintEventHandler::eventFilter(QObject* obj, QEvent* event) 
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        //QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(obj);
         // viewport()의 부모를 통해 QTreeWidget을 얻습니다.
        QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(obj->parent());
        
        if (treeWidget) {
            QPoint point = helpEvent->pos();
            QTreeWidgetItem* item = treeWidget->itemAt(point);
            if (item) {
                QVariant tooltipData = item->data(0, Qt::UserRole);
                if (tooltipData.isValid()) {

                    auto extractedStruct = item->data(0, Qt::UserRole).value<OptimizeListInfo>();
                    
                    // 조회된 툴팁 텍스트를 표시
                    QToolTip::showText(helpEvent->globalPos(), extractedStruct.hintText);
                    //QToolTip::showText(helpEvent->globalPos(), tooltipData.toString());
                    //QTimer::singleShot(3000, []() {
                    //    QToolTip::hideText();
                    //    });

                    return true; // 이벤트 처리 완료
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}