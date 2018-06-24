#include "mainpresenter.h"
#include "mainmodel.h"
#include "serialport/processor/can_processer.h"
#include "chstatemanager.h"
#include <QDebug>

MainPresenter::MainPresenter(MainContract::View* view)
    :Presenter(view)
{
    this->model = new MainModel();
    CAN_ProcesserInit();

    ChStateManager::getInstance()->initTimer(this);
    ChStateManager::getInstance()->setOnChStateTimeoutCallback([this](chnum_t ch) {
        this->view->setState(ch, ChState::HIDE);
    });

    setChStateChangedCallback([this](chnum_t ch, ChState state) {
        if(ChStateManager::ChData[ch].on) {
            this->view->setState(ch, state);

            // 重置超时管理
            ChStateManager::ChData[ch].timeout = DATA_DISP_TIMEOUTRESET;
            ChStateManager::ChData[ch].status  = true;          // 状态启用
        }
        else {
            printf("ch = %d, state = %d, 当前通道未启用！", ch, state);
        }
    });

    serialPort = new SerialPort(BAUDRATE, USB_VID, USB_PID);

    connect(serialPort, SIGNAL(onConStateChanged(bool)), this, SLOT(onConStateChanged(bool)));

    connect(serialPort, &SerialPort::onRecvPackage, this, [](QByteArray byteArray) {
        qDebug()<<byteArray.toHex();
        // todo:
        // CAN_ProcesserProcess();
    });

    for (chnum_t i = 0; i < AllChNum; i++) {
        chnum_t ch = i+1;
        auto name = model->getChName(ch);
        view->setChName(ch, name);
        if (name == "") {
            view->setState(ch, ChState::HIDE);
            ChStateManager::ChData[ch].state = ChState::HIDE;
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
            ChStateManager::ChData[ch].on = false;
        } else {
            view->setState(ch, ChStateManager::ChData[ch].state);
        }
    }
}
