#ifndef UTIL_TIME_TIME_H_2019_11_30_17_21
#define UTIL_TIME_TIME_H_2019_11_30_17_21

#include <stdint.h>

typedef struct Duration{
    uint64_t seconds;
    int32_t nanos;
} Duration;
typedef struct Instant{
    uint64_t seconds;
    int32_t nanos;
} Instant;


#endif
