#include "candataprocesser.h"
#include "utils/mylog.h"

#define CAN_DUG_PRINTF log

CanDataProcesser::CanDataProcesser()
{
    for(int i=0; i<BK_MAX; i++) {
        _bkData[i].mark = 0;
        BkData[i] = -1;
    }
}

void CanDataProcesser::setOnChStateChangedCallback(OnChStateChangedCallback callback) {
    onChStateChangedCallback = callback;
}

void CanDataProcesser::setOnReceivedChStateCallback(OnReceivedChStateCallback callback) {
    onReceivedChStateCallback = callback;
}

void CanDataProcesser::process(uint8_t* rxData) {
    uint8_t  ID0    = rxData[0];
    uint8_t  index  = rxData[1];    // Index
    uint8_t  bk_id  = rxData[2];    // 机号/设备号
    // uint8_t  ID3   = rxData[3];

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
                    _bkData[bk_id].data[4*0 + i] = rxData[4+i];
                }
                _bkData[bk_id].mark |= 0x01;
            }
            else if(index == 1) {
                for(int i=0; i<4; i++) {
                    _bkData[bk_id].data[4*1 + i] = rxData[4+i];
                }
                _bkData[bk_id].mark |= 0x02;
            }
            else if(index == 2) {
                for(int i=0; i<4; i++) {
                    _bkData[bk_id].data[4*2 + i] = rxData[4+i];
                }
                _bkData[bk_id].mark |= 0x04;
            }
        }
        else if(type == 2) {
            // 结束多帧 也是第四帧
            for(int i=0; i<4; i++) {
                _bkData[bk_id].data[4*3 + i] = rxData[4+i];
            }

            if(_bkData[bk_id].mark == 0x07) {
                CAN_DUG_PRINTF("成功接收到一条完整数据：");

                // 解析_bkData中的数据
                // _bkData[bk_id].data[0];    // always 0x16
                uint8_t  ms  = _bkData[bk_id].data[1];
                uint32_t sec = _bkData[bk_id].data[2];
                sec += _bkData[bk_id].data[3] << (8*1);

                sec += _bkData[bk_id].data[4] << (8*2);
                sec += _bkData[bk_id].data[5] << (8*3);
                // _bkData[bk_id].data[6];    // reserved
                uint8_t chCount = _bkData[bk_id].data[7];

                uint64_t data = _bkData[bk_id].data[8];
                data += (uint64_t) _bkData[bk_id].data[9]  << (8*1);
                data += (uint64_t) _bkData[bk_id].data[10] << (8*2);
                data += (uint64_t) _bkData[bk_id].data[11] << (8*3);
                data += (uint64_t) _bkData[bk_id].data[12] << (8*4);
                data += (uint64_t) _bkData[bk_id].data[13] << (8*5);
                data += (uint64_t) _bkData[bk_id].data[14] << (8*6);

                CAN_DUG_PRINTF("解析如下：ms = %d, sec = %u, bk_id = %d, data = 0x%02llX", ms, sec, bk_id, data);

                if (bk_id < BK_MAX) {
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

                        if (1 <= ch && ch <= AllChNum) {
                            const ChState lastState = ChStateManager::ChData[ch].state;
                            const ChState nowState  = state == 1 ? ChState::YES : ChState::NO;
                            ChStateManager::ChData[ch].state = nowState;

                            if (onReceivedChStateCallback)
                                onReceivedChStateCallback(ch, nowState);

                            if (nowState != lastState) {
                                if (onChStateChangedCallback)
                                    onChStateChangedCallback(ch, nowState);
                            }
                        }
                        else {
                            CAN_DUG_PRINTF("ch = %d, 通道超出范围！", ch);
                        }
                    }
                }
                else {
                    CAN_DUG_PRINTF("bk_id = %d, 板卡ID超出范围！", bk_id);
                }
            }
            else {
                CAN_DUG_PRINTF("数据有丢失!");
                CAN_DUG_PRINTF("_bkData[bk_id].mark = 0x%02X", _bkData[bk_id].mark);
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
