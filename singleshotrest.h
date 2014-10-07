#ifndef TIMEOFDAY_H
#define TIMEOFDAY_H

#include "rest.h"

class SingleShotRest : public Rest
{
public:
    SingleShotRest(QDateTime triggerTime, QTime duration)
        : Rest(duration),
          triggerTime(triggerTime)
    {}

    virtual bool triggeredAt(const QDateTime &t) const
    {
        return t.secsTo(triggerTime) == 0;
    }

    virtual bool expiresAt(const QDateTime &t) const
    {
        return triggeredAt(t);
    }

    QDateTime triggerTime;
};

#endif // TIMEOFDAY_H
