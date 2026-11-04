#pragma once

#include <QObject>
#include <QTimer>
#include <QTimerEvent>
#include <QNetworkReply>

#include "../defineproperty/defineproperty.h"

class ReplyTimeout : public QObject {
    Q_OBJECT

    PROPERTY_NOCHANGE(QBasicTimer, timer)
    
public:
    ReplyTimeout(QNetworkReply* reply, const int timeout) 
        : QObject(reply)
    {
        Q_ASSERT(reply);

        if (reply && reply->isRunning())
            m_timer.start(timeout, this);
    }

    static void set(QNetworkReply* reply, const int timeout)
    {
        new ReplyTimeout(reply, timeout);
    }

protected:
    void timerEvent(QTimerEvent * ev) override
    {
        if (m_timer.isActive() && ev->timerId() == m_timer.timerId())
        {
            auto reply = static_cast<QNetworkReply*>(parent());
            if (reply->isRunning())
            {
                //reply->error(QNetworkReply::TimeoutError);//reply->close();                
                reply->error();
            }
            m_timer.stop();
        }
    }
};
