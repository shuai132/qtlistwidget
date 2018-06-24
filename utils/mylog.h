#ifndef MYLOG_H
#define MYLOG_H

#include <QDebug>
#include <QString>

#define log qDebug()<<QString().sprintf

#endif // MYLOG_H
