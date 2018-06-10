#ifndef MAINCONTRACT_H
#define MAINCONTRACT_H

#include "conf/config.h"
#include "maintypedef.h"
#include "../base/mvp.h"

#include <QString>
#include <functional>

class MainContract
{
public:
    class Model;

    class View : public MVP<Model, View>::View {
    public:
        typedef std::function<void(chnum_t, QString)> ChNameChangeListener;

    public:
        virtual void setChName(chnum_t ch, QString name) = 0;
        virtual void setState(chnum_t ch, ChState state) = 0;
        virtual void setConState(bool isConnected) = 0;

        virtual void setChNameChangeListener(ChNameChangeListener listener) = 0;
    };

    class Model : public MVP<Model, View>::Model {
    public:
        virtual void setChName(chnum_t ch, QString name) = 0;
        virtual QString getChName(chnum_t ch) = 0;
    };
};

#endif // MAINCONTRACT_H
