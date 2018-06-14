#ifndef MAINCONTRACT_H
#define MAINCONTRACT_H

#include "conf/config.h"
#include "maintypedef.h"
#include "../base/mvp.h"

#include <QString>

namespace MainContract {
    class Model : public MVP::Model {
    public:
        virtual void setChName(chnum_t ch, QString name) = 0;
        virtual QString getChName(chnum_t ch) = 0;
    };

    class Presenter;
    class View : public MVP::View<Presenter> {
    public:
        virtual void setChName(chnum_t ch, QString name) = 0;
        virtual void setState(chnum_t ch, ChState state) = 0;
        virtual void setConState(bool isConnected) = 0;
    };

    class Presenter : public MVP::Presenter<Model, View>
    {
    public:
        explicit Presenter(View* view)
            :MVP::Presenter<Model, View>(view)
        {}

        virtual void restoreChName(chnum_t ch, QString name) = 0;
    };
}

#endif // MAINCONTRACT_H
