#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include "maincontract.h"

class MainPresenter : public MVP<MainContract::Model, MainContract::View>::Presenter
{
public:
    MainPresenter(MainContract::View* view);
};

#endif // MAINPRESENTER_H
