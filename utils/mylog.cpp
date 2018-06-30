#include "mylog.h"

#include <QDebug>
#include <QString>

void log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    qDebug()<<QString().vasprintf(fmt, ap);
    va_end(ap);
}
