#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "maincontract.h"

class MainModel : public MainContract::Model
{
public:
    MainModel();

    // Model interface
public:
    void setChName(chnum_t ch, const char *name);
    const char* getChName(chnum_t ch);
};

#endif // MAINMODEL_H