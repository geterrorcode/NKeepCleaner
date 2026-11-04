#include "qss.h"

//#include "Network/loadsync.h"

#include <QString>
#include <QFile>
#include <QRegularExpression>

Qss::Qss()
{
    //auto qssText = QString(loadSync(QUrl::fromLocalFile(":/Launcher/Launcher.qss")));
    auto qssText = ":/src/common/ssdefines.qss";
    

    auto itor = QRegularExpression("[$](\\S+)\\s+([^$]+)").globalMatch(qssText);
    while (itor.hasNext()) {
        auto match = itor.next();
        auto name = match.captured(1), ss = match.captured(2);
        m_styleSheetMap[name] = ss;
    }

    
}
