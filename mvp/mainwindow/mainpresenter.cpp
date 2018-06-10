#include "mainpresenter.h"
#include "mainmodel.h"

MainPresenter::MainPresenter(MainContract::View* view)
    :Presenter(view)
{
    this->model = new MainModel();

    this->view->setChName(2, "are you ok?");
}
