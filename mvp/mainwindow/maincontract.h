#ifndef MAINCONTRACT_H
#define MAINCONTRACT_H

#include "conf/config.h"
#include "../base/mvp.h"

class MainContract
{
public:
    class Model;

    class View : public MVP<Model, View>::View {
    public:
        virtual void setChName(chnum_t ch, const char* name) = 0;
        virtual void setState(chnum_t ch, CHState state) = 0;
    };

    class Model : public MVP<Model, View>::Model {
    public:
        virtual void setChName(chnum_t ch, const char* name) = 0;
        virtual const char* getChName(chnum_t ch) = 0;
    };

public:
    MainContract();
};

#endif // MAINCONTRACT_H
