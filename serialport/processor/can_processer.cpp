#include "can_processer.h"
#include "mvp/mainwindow/chstatemanager.h"

#define BK_MAX              8               // 板卡最大数量
#define CH_NUM              80              //通道数

#define FRAME_PER_MSG       4               // 每个消息需要的can帧数
#define CACHE_CAN_MSG_NUM   60              // 启用多少帧数据缓存
#define MSG_FIFO_LENGTH     (FRAME_PER_MSG*CACHE_CAN_MSG_NUM)  // 队列长度

#define CAN_DUG_PRINTF printf

/**
 * 板卡数据
 */
typedef struct
{
    uint8_t mark;                           // 接收标记 用于多帧拼接时判断数据完整性
    uint8_t data[4 * FRAME_PER_MSG];        // 板卡的通道状态数据
}BKDataTypeDef;

/**
 * CAN消息队列
 */
typedef struct
{
    uint8_t head;
    uint8_t tail;
    uint8_t length;
    CanRxMsg msg[MSG_FIFO_LENGTH];
}CanRxMsgFIFO;


static BKDataTypeDef _bkData[BK_MAX];       // 记录当前板卡数据
static CanRxMsgFIFO _msgFIFO = {0, 0, 0};   // CAN消息队列
static int BkData[BK_MAX];                  // 记录板卡id对应的通道数量

void CAN_ProcesserInit(void) {
    for(int i=0; i<BK_MAX; i++) {
        _bkData[i].mark = 0;
        BkData[i] = -1;
    }
}

void CAN_ProcesserProcess(CanRxMsg RxMessage) {
    uint8_t  ID0    = RxMessage.Data[0];
    uint8_t  index  = RxMessage.Data[1];    // Index
    uint8_t  bk_id  = RxMessage.Data[2];    // 机号/设备号
    // uint8_t  ID3   = RxMessage.Data[3];

#ifdef USE_CAN_DUG_PRINTF
    #if 0
    CAN_DUG_PRINTF("TxMessage.StdId is : 0x%X", RxMessage.StdId);
    CAN_DUG_PRINTF("RxMessage.ExtId is : 0x%X", RxMessage.ExtId);
    CAN_DUG_PRINTF("RxMessage.IDE is   : 0x%X", RxMessage.IDE);
    CAN_DUG_PRINTF("RxMessage.DLC is   : %d",   RxMessage.DLC);
#endif
    for(int i=0; i<RxMessage.DLC; i++) {
        CAN_DUG_PRINTF("RxMessage.Data[%d] is : 0x%02X", i, RxMessage.Data[i]);
    }
#endif

    CAN_DUG_PRINTF("index = 0x%02X, bk_id = 0x%02X", index, bk_id);

    uint8_t type = (ID0>>4)&0x03;
    if((ID0>>6) == 0) { // 校验通过
        if(type == 0) {
            // 单帧
        }
        else if(type == 1) {
            // 非结束多帧
            if(index == 0) {
                for(int i=0; i<4; i++) {
                    _bkData[bk_id].data[4*0 + i] = RxMessage.Data[4+i];
                }
                _bkData[bk_id].mark |= 0x01;
            }
            else if(index == 1) {
                for(int i=0; i<4; i++) {
                    _bkData[bk_id].data[4*1 + i] = RxMessage.Data[4+i];
                }
                _bkData[bk_id].mark |= 0x02;
            }
            else if(index == 2) {
                for(int i=0; i<4; i++) {
                    _bkData[bk_id].data[4*2 + i] = RxMessage.Data[4+i];
                }
                _bkData[bk_id].mark |= 0x04;
            }
        }
        else if(type == 2) {
            // 结束多帧 也是第四帧
            for(int i=0; i<4; i++) {
                _bkData[bk_id].data[4*3 + i] = RxMessage.Data[4+i];
            }

            if(_bkData[bk_id].mark == 0x07) {
                CAN_DUG_PRINTF("成功接收到一条完整数据：");
#ifdef USE_CAN_DUG_PRINTF
                for(int i=0; i<16; i++) {
                    CAN_DUG_PRINTF("_bkData[%02X].data[%d] = 0x%02X", bk_id, i, _bkData[bk_id].data[i]);
                }
#endif

                // 解析_bkData中的数据
                // _bkData[bk_id].data[0];    // always 0x16
                uint8_t  ms  = _bkData[bk_id].data[1];
                uint32_t sec = _bkData[bk_id].data[2];
                sec += _bkData[bk_id].data[3] << (8*1);

                sec += _bkData[bk_id].data[4] << (8*2);
                sec += _bkData[bk_id].data[5] << (8*3);
                // _bkData[bk_id].data[6];    // reserved
                uint8_t chCount = _bkData[bk_id].data[7];

                uint32_t data = _bkData[bk_id].data[8];
                data += _bkData[bk_id].data[9]  << (8*1);
                data += _bkData[bk_id].data[10] << (8*2);
                data += _bkData[bk_id].data[11] << (8*3);

                CAN_DUG_PRINTF("解析如下：ms = %d, sec = %u, bk_id = %d, data = 0x%X", ms, sec, bk_id, data);

                if (0 <= bk_id && bk_id < BK_MAX) {
                    BkData[bk_id] = chCount;

                    uint32_t chStart = 1;
                    for (int i=0; i<bk_id; i++) {
                        int num = BkData[i];
                        if (num < 0) {
                            CAN_DUG_PRINTF("id更小的板卡通道数尚未上报!");
                            return;
                        }
                        chStart += num;
                    }

                    for (int i=0; i<chCount; i++) {
                        uint16_t ch    = chStart + i;
                        uint8_t  state = (data >> i) & 0x01;

                        if (1 <= ch && ch <= CH_NUM) {
                            uint16_t id = ch;     // id为要显示到屏幕的第几行
                            CAN_DUG_PRINTF("ch = %d, state = %d, 要显示到屏幕的绝对位置为：%d", ch, state, id);

                            const ChState lastState = ChStateManager::getInstance()->getState(id);
                            const ChState nowState  = state == 1 ? ChState::YES : ChState::NO;
                            ChStateManager::getInstance()->setState(id, nowState);

//                            if(ChData[id].on) {
//                                // 避免频繁写屏
//                                if (nowState == lastState) {
//                                    CAN_DUG_PRINTF("与上次状态一致,不再更新屏幕!");
//                                } else {
//                                    LCD_SetChState(id, nowState);
//                                    CAN_DUG_PRINTF("状态发生改变,已更新屏幕!");
//                                }

//                                // 重置超时管理
//                                ChData[id].timeout = DATA_DISP_TIMEOUTRESET;
//                                ChData[id].status  = 1;            // 状态启用
//                            }
//                            else {
//                                CAN_DUG_PRINTF("ch = %d, state = %d, 当前通道未启用！", ch, state);
//                            }
                        }
                        else {
                            CAN_DUG_PRINTF("ch = %d, 超出屏幕可显示的范围！", ch);
                        }
                    }
                }
                else {
                    CAN_DUG_PRINTF("bk_id = %d, 板卡ID超出范围！", bk_id);
                }
            }
            else {
                CAN_DUG_PRINTF("数据有丢失!");
                CAN_DUG_PRINTF("_bkData[bk_id].mark = 0x%X", _bkData[bk_id].mark);
            }
            _bkData[bk_id].mark = 0x00;
        }
        else {
            CAN_DUG_PRINTF("帧类型未识别!");
        }
    }
    else {
        CAN_DUG_PRINTF("总线上接收到无效数据! type=%d", type);
    }
}

bool CAN_RxMsg_Enqueue(CanRxMsg RxMessage) {
    if(_msgFIFO.length < MSG_FIFO_LENGTH) {
        _msgFIFO.msg[_msgFIFO.head++] = RxMessage;

        if(_msgFIFO.head >= MSG_FIFO_LENGTH) {
            _msgFIFO.head = 0;
        }
        _msgFIFO.length++;

        // CAN_DUG_PRINTF("Enqueue：队列已使用：%d/%d", _msgFIFO.length, MSG_FIFO_LENGTH);
        return true;
    }
    else {
        CAN_DUG_PRINTF("队列已满！");
        return false;
    }
}

bool CAN_RxMsg_Dequeue(CanRxMsg *msg) {
    if(_msgFIFO.length > 0) {
        *msg = _msgFIFO.msg[_msgFIFO.tail];

        if(++_msgFIFO.tail >= MSG_FIFO_LENGTH) {
            _msgFIFO.tail = 0;
        }

        _msgFIFO.length--;
        // CAN_DUG_PRINTF("Dequeue：队列已使用：%d/%d", _msgFIFO.length, MSG_FIFO_LENGTH);
        return true;
    }
    else {
        return false;
    }
}
