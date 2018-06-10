#include "mainmodel.h"
#include "data/sharedpreferences/sharedpreferences.h"
#include <QDebug>

MainModel::MainModel()
{
    auto sp = SharedPreferences::getInstance();
    bool isInited = sp->settings->contains("inited");
    if (!isInited) {
        sp->settings->setValue("inited", "yes");
        for (chnum_t i = 0; i < AllChNum; i++) {
            chnum_t ch = i + 1;
            sp->settings->setValue(QString::number(ch), QString("name").append(QString::number(ch)));
        }
    }
}

void MainModel::setChName(chnum_t ch, const char *name)
{
    qDebug()<<__func__<<ch<<name;
    SharedPreferences::getInstance()->settings->setValue(QString::number(ch), name);
}

const char* MainModel::getChName(chnum_t ch)
{
    return SharedPreferences::getInstance()->settings->value(QString::number(ch)).toString().toLatin1();
}
