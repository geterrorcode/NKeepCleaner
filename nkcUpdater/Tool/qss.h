#pragma once

//#include <Qux/include/singleton.h>

#include "../QUX/include/singleton.h"

#include <QVariantMap>

class Qss : public Qux::Singleton<Qss>
{
public:
    QString getStyleSheet(QString name) const
    {
        return m_styleSheetMap[name].toString();
    };

private:
    friend Qss& Qux::Singleton<Qss>::inst();
    Qss();

    QVariantMap m_styleSheetMap;

};

inline QString ss(QString name) { return Qss::inst().getStyleSheet(name); }
