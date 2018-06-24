#ifndef __CAN_PROCESSER_H
#define __CAN_PROCESSER_H

#include <stdbool.h>
#include <stdint.h>

/**
  * @brief  CAN Rx message structure definition
  */
typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that
                        will be received. This parameter can be a value of
                        @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to
                        0xFF. */

  uint8_t FMI;     /*!< Specifies the index of the filter the message stored in
                        the mailbox passes through. This parameter can be a
                        value between 0 to 0xFF */
} CanRxMsg;

void CAN_ProcesserInit(void);

/**
 * 处理消息帧 进行解析拼接显示等处理
 */
void CAN_ProcesserProcess(CanRxMsg RxMessage);

/**
 * CAN消息帧入队列 成功返回true 已满返回false
 */
bool CAN_RxMsg_Enqueue(CanRxMsg RxMessage);

/**
 * 从消息队列中获取一帧
 */
bool CAN_RxMsg_Dequeue(CanRxMsg *msg);

#endif
