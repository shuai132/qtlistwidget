#include "mainpresenter.h"
#include "mainmodel.h"
#include <QDebug>

MainPresenter::MainPresenter(MainContract::View* view)
    :Presenter(view)
{
    this->model = new MainModel();

    serialPort = new SerialPort(BAUDRATE, USB_VID, USB_PID);

    connect(serialPort, SIGNAL(onConStateChanged(bool)), this, SLOT(onConStateChanged(bool)));

    for (chnum_t i = 0; i < AllChNum; i++) {
        chnum_t ch = i+1;
        view->setChName(ch, model->getChName(ch));
    }
}

MainPresenter::~MainPresenter()
{
    qDebug()<<"~MainPresenter";
    delete serialPort;
}

void MainPresenter::onConStateChanged(bool isConnected)
{
    view->setConState(isConnected);
}

void MainPresenter::restoreChName(chnum_t ch, QString name)
{
    model->setChName(ch, name);
}
