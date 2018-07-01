#include "mainpresenter.h"
#include "mainmodel.h"
#include "chstatemanager.h"
#include "utils/mylog.h"

MainPresenter::MainPresenter(MainContract::View* view)
    :Presenter(view)
{
    this->model = new MainModel();

    ChStateManager::getInstance()->setOnChStateTimeoutCallback([this](chnum_t ch) {
        log("通道超时: ch = %d", ch);
        ChStateManager::ChData[ch].state = ChState::HIDE;
        this->view->setState(ch, ChState::HIDE);
    });

    canDataProcesser = new CanDataProcesser();
    canDataProcesser->setOnChStateChangedCallback([this](chnum_t ch, ChState state) {
        if(ChStateManager::ChData[ch].on) {
            this->view->setState(ch, state);
        }
        else {
            log("ch = %d, 当前通道未启用！", ch);
        }
    });
    canDataProcesser->setOnReceivedChStateCallback([this](chnum_t ch, ChState state) {
        (void)state;
        if(ChStateManager::ChData[ch].on) {
            // 重置超时管理
            ChStateManager::ChData[ch].timeout = ChStateManager::TIMEOUT_RESET;
            ChStateManager::ChData[ch].status  = true;          // 状态启用
        }
    });

    serialPort = new SerialPort(BAUDRATE, USB_VID, USB_PID);

    connect(serialPort, &SerialPort::onConStateChanged, this, [this](bool isConnected) {
        this->view->setConState(isConnected);
    });

    connect(serialPort, &SerialPort::onRecvPackage, this, [this](QByteArray byteArray) {
        log("接收到数据包: len=%d, data=%s", byteArray.length(), byteArray.toHex().toStdString().c_str());

        if (byteArray.length() != 16) return;

        const int dataOffset = 4;
        const int dataLength = 8;
        uint8_t data[dataLength];
        for (int i=0; i<dataLength; i++) {
            data[i] = byteArray[i + dataOffset];
        }
        this->canDataProcesser->process(data);
    });

    for (chnum_t i = 0; i < AllChNum; i++) {
        chnum_t ch = i+1;
        auto name = model->getChName(ch);
        view->setChName(ch, name);
        view->setState(ch, ChState::HIDE);
        ChStateManager::ChData[ch].state = ChState::HIDE;
        ChStateManager::ChData[ch].on = (name != "");
    }
}

MainPresenter::~MainPresenter()
{
    log("~MainPresenter");
    delete serialPort;
    delete canDataProcesser;
    delete model;
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
