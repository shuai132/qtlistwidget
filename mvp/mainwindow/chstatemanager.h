#ifndef CHSTATEMANAGER_H
#define CHSTATEMANAGER_H

#include "conf/config.h"
#include "maintypedef.h"
#include <functional>
#include <QObject>

typedef struct
{
    ChState state;          // 通道状态

    uint16_t timeout;       // 超时倒计时
    bool status;
    bool on;                // 是否被启用 决定是否显示
}ChDataTypeDef;


class ChStateManager : public QObject
{
    Q_OBJECT

public:
    volatile static ChDataTypeDef ChData[AllChNum + 1];
    static const uint16_t TIMEOUT_RESET = 5;

public:
    static ChStateManager* getInstance();

    typedef std::function<void(chnum_t ch)> ChStateTimeoutCallback;
    void setOnChStateTimeoutCallback(ChStateTimeoutCallback callback);

private:
    ChStateManager();
    void initTimer();

private:
    ChStateTimeoutCallback chStateTimeoutCallback = nullptr;
};

#endif // CHSTATEMANAGER_H
