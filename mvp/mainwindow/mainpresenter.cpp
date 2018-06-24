#include "mainpresenter.h"
#include "mainmodel.h"
#include <QDebug>

MainPresenter::MainPresenter(MainContract::View* view)
    :Presenter(view)
{
    this->model = new MainModel();

    serialPort = new SerialPort(BAUDRATE, USB_VID, USB_PID);

    connect(serialPort, SIGNAL(onConStateChanged(bool)), this, SLOT(onConStateChanged(bool)));

    connect(serialPort, &SerialPort::onRecvPackage, this, [](QByteArray byteArray) {
        qDebug()<<byteArray.toHex();
    });

    for (chnum_t i = 0; i < AllChNum; i++) {
        chnum_t ch = i+1;
        auto name = model->getChName(ch);
        view->setChName(ch, name);
        if (name == "") {
            view->setState(ch, ChState::HIDE);
            chStateManager->setState(ch, ChState::HIDE);
        }
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
    auto oldName = model->getChName(ch);
    model->setChName(ch, name);
    if (name != oldName) {
        if (name.trimmed() == "") {
            view->setState(ch, ChState::HIDE);
            chStateManager->setState(ch, ChState::HIDE);
        } else {
            view->setState(ch, chStateManager->getState(ch));
        }
    }
}
