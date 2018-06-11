#include "mainpresenter.h"
#include "mainmodel.h"
#include <QDebug>

#include "mainwindow.h"

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

    view->setChNameChangeListener([this](chnum_t ch, QString name) {
        this->model->setChName(ch, name);
    });
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
