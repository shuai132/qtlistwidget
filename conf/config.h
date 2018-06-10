#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

typedef uint32_t chnum_t;

const chnum_t ChNumMin = 1;
const chnum_t AllChNum = 80;

typedef enum {
    HIDE  = 0,
    NO    = 1,
    YES   = 2,
} CHState;

#endif // CONFIG_H
