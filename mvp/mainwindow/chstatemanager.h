#ifndef CHSTATEMANAGER_H
#define CHSTATEMANAGER_H

#include "conf/config.h"
#include "maintypedef.h"
#include <functional>
#include <QObject>
#include <atomic>

#define DATA_DISP_TIMEOUTRESET      5       // 显示超过多少秒就清零

typedef struct
{
    uint16_t ch_id;         // 通道id
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

public:
    static ChStateManager* getInstance();

    void initTimer(QObject* parent = nullptr);

    typedef std::function<void(chnum_t ch)> ChStateTimeoutCallback;
    void setOnChStateTimeoutCallback(ChStateTimeoutCallback callback);

private:
    ChStateManager();

private:
    std::atomic_flag timerInited;
    ChStateTimeoutCallback chStateTimeoutCallback = nullptr;
};

#endif // CHSTATEMANAGER_H
