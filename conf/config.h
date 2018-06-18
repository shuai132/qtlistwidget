#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

typedef uint32_t chnum_t;

const chnum_t ChNumMin = 1;
const chnum_t AllChNum = 80;
const chnum_t AllChMax = AllChNum;

const uint16_t USB_VID = 0x00;
const uint16_t USB_PID = 0X00;

const uint32_t BAUDRATE = 115200;

#endif // CONFIG_H
