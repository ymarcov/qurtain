#ifndef INTERVALREST_H
#define INTERVALREST_H

#include "rest.h"

class IntervalRest : public Rest
{
public:
    IntervalRest(QDateTime start, QDateTime end, QTime interval, QTime duration)
        : Rest(duration),
          start(start),
          end(end),
          interval(interval)
    {}

    virtual bool triggeredAt(const QDateTime &t) const
    {
        const qint64 window = 250;
        const qint64 msecsFromStart = start.msecsTo(t);

        if (msecsFromStart < 0)
            return false;

        if (msecsFromStart <= window)
            return true;

        const qint64 intervalMsecs = QTime(0, 0, 0).msecsTo(interval);
        return msecsFromStart % intervalMsecs <= window;
    }

    virtual bool expiresAt(const QDateTime &t) const
    {
        return t.secsTo(end) == 0;
    }

    QDateTime start;
    QDateTime end;
    QTime interval;
};

#endif // INTERVALREST_H
