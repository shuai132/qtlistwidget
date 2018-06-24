#ifndef CANDATAPROCESSER_H
#define CANDATAPROCESSER_H

#include "mvp/mainwindow/chstatemanager.h"

#include <stdbool.h>
#include <stdint.h>
#include <functional>

#define BK_MAX              8               // 板卡最大数量
#define FRAME_PER_MSG       4               // 每个消息需要的can帧数

class CanDataProcesser
{
public:
    CanDataProcesser();

    typedef std::function<void(chnum_t ch, ChState state)> ChStateChangedCallback;
    void setChStateChangedCallback(ChStateChangedCallback callback);

    void process(uint8_t* rxData);

private:
    /**
     * 板卡数据
     */
    typedef struct
    {
        uint8_t mark;                           // 接收标记 用于多帧拼接时判断数据完整性
        uint8_t data[4 * FRAME_PER_MSG];        // 板卡的通道状态数据
    }BKDataTypeDef;

    BKDataTypeDef _bkData[BK_MAX];              // 记录当前板卡数据
    int BkData[BK_MAX];                         // 记录板卡id对应的通道数量

    ChStateChangedCallback chStateChangedCallback = nullptr;

};

#endif
