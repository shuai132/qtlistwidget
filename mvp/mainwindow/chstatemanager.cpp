#include "chstatemanager.h"
#include <QTimer>
#include <cassert>
#include <utils/mylog.h>

#define CHECK_CHNUM(ch) do{assert(ChNumMin <= ch && ch <= ChNumMax);} while(0)

volatile ChDataTypeDef ChStateManager::ChData[AllChNum + 1];

ChStateManager* ChStateManager::getInstance()
{
    static ChStateManager* instance = nullptr;
    if (!instance) {
        instance = new ChStateManager();
    }
    return instance;
}

void ChStateManager::initTimer()
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this](){
        for(chnum_t i=1; i < AllChNum + 1; i++) {
            if(ChData[i].on && ChData[i].status && ChData[i].timeout-- == 0) {
                if (this->chStateTimeoutCallback)
                    this->chStateTimeoutCallback(i);

                ChData[i].status = false;    //状态关闭
            }
        }
    });
    timer->start(1000);
}

void ChStateManager::setOnChStateTimeoutCallback(ChStateManager::ChStateTimeoutCallback callback)
{
    chStateTimeoutCallback = callback;
}

ChStateManager::ChStateManager()
{
    for(chnum_t i=1; i < AllChNum + 1; i++) {
        ChData[i].on = true;
        ChData[i].status = true;
        ChData[i].timeout = TIMEOUT_RESET;
        ChData[i].state = ChState::HIDE;
    }

    initTimer();
}
