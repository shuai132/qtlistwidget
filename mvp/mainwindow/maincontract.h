#ifndef MAINCONTRACT_H
#define MAINCONTRACT_H

#include "conf/config.h"
#include "maintypedef.h"
#include "../base/mvp.h"
#include <functional>

class MainContract
{
public:
    class Model;

    class View : public MVP<Model, View>::View {
    public:
        typedef std::function<void(chnum_t, const char*)> ChNameChangeListener;

    public:
        virtual void setChName(chnum_t ch, const char* name) = 0;
        virtual void setState(chnum_t ch, ChState state) = 0;
        virtual void setConState(bool isConnected) = 0;

        virtual void setChNameChangeListener(ChNameChangeListener listener) = 0;
    };

    class Model : public MVP<Model, View>::Model {
    public:
        virtual void setChName(chnum_t ch, const char* name) = 0;
        virtual const char* getChName(chnum_t ch) = 0;
    };
};

#endif // MAINCONTRACT_H
