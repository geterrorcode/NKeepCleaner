#pragma once

//#include <Qux/include/singleton.h>
#include "../ux/singleton.h"
#include <QVariantMap>

//class Qss : public Qux::Singleton<Qss>
class Qss : public common::ux::Singleton<Qss>
{
public:
    QString getStyleSheet(QString name) const
    {
        return m_styleSheetMap[name].toString();
    };

private:
    friend Qss& common::ux::Singleton<Qss>::inst();
    Qss();

    QVariantMap m_styleSheetMap;

};

inline QString ss(QString name) { return Qss::inst().getStyleSheet(name); }
