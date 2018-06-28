#ifndef MYLOG_H
#define MYLOG_H

#include <QDebug>
#include <QString>

inline void log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    qDebug()<<QString().vasprintf(fmt, ap);
    va_end(ap);
}

#endif // MYLOG_H
