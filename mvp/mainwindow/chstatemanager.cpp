#include "chstatemanager.h"
#include <QTimer>
#include <cassert>

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

void ChStateManager::initTimer(QObject* parent)
{
    if (timerInited.test_and_set()) return;

    QTimer *timer = new QTimer(parent);
    connect(timer, &QTimer::timeout, this, [this](){
        for(int i=1; i < AllChNum + 1; i++) {
            if(ChData[i].on == 1) {
                if(ChData[i].status == 1) {
                    if(ChData[i].timeout-- == 0) {
                        if (this->chStateTimeoutCallback)
                            this->chStateTimeoutCallback(i);

                        ChData[i].status = false;    //状态关闭
                    }
                }
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
}
