#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "maincontract.h"
#include "data/sharedpreferences/sharedpreferences.h"

class MainModel : public MainContract::Model
{
public:
    MainModel();

    // Model interface
public:
    void setChName(chnum_t ch, QString name) override;
    QString getChName(chnum_t ch) override;

private:
    SharedPreferences* sp = SharedPreferences::getInstance();
};

#endif // MAINMODEL_H
