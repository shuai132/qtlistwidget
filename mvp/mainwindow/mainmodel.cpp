#include "mainmodel.h"
#include <QDebug>

MainModel::MainModel()
{
    const char* initFlagKey = "inited";
    bool isInited = sp->contains(initFlagKey);
    if (!isInited) {
        sp->setValue(initFlagKey, true);
        for (chnum_t i = 0; i < AllChNum; i++) {
            chnum_t ch = i + 1;
            sp->setValue(QString::number(ch), QString("name").append(QString::number(ch)));
        }
    }
}

void MainModel::setChName(chnum_t ch, QString name)
{
    SharedPreferences::getInstance()->setValue(QString::number(ch), name);
}

QString MainModel::getChName(chnum_t ch)
{
    return sp->getvalue(QString::number(ch)).toString();
}
