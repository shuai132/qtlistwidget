#ifndef MAINCONTRACT_H
#define MAINCONTRACT_H

#include "conf/config.h"
#include "maintypedef.h"
#include "../base/mvp.h"

#include <QString>
#include <functional>

namespace MainContract {
    class Model : public MVP::Model {
    public:
        virtual void setChName(chnum_t ch, QString name) = 0;
        virtual QString getChName(chnum_t ch) = 0;
    };

    class View : public MVP::View<MVP::Presenter<Model, View>> {
    public:
        virtual void setChName(chnum_t ch, QString name) = 0;
        virtual void setState(chnum_t ch, ChState state) = 0;
        virtual void setConState(bool isConnected) = 0;

        typedef std::function<void(chnum_t, QString)> ChNameChangeListener;
        virtual void setChNameChangeListener(ChNameChangeListener listener) = 0;
    };

    class Presenter : public MVP::Presenter<Model, View>
    {
    public:
        explicit Presenter(View* view)
            :MVP::Presenter<Model, View>(view)
        {}
    };
}

#endif // MAINCONTRACT_H
