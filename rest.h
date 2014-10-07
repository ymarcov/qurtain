#ifndef REST_H
#define REST_H

#include <QDateTime>
#include <QTime>

class Rest
{
public:
    Rest(QTime duration)
        : duration(duration)
    {}

    virtual ~Rest() {}

    virtual bool triggeredAt(const QDateTime &) const = 0;
    virtual bool expiresAt(const QDateTime &) const = 0;

    QTime duration;
};

#endif // REST_H
