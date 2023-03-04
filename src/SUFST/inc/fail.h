#ifndef FAIL_H
#define FAIL_H

#include <stdint.h>
#include "watchdog.h"

#define CAN_UNPACK_ERROR_INIT 1
#define RTCAN_SUBSCRIBE_ERROR_INIT 2
#define RTCAN_START_ERROR 3
#define STATS_MUTEX_ERROR 4
#define CAN_RX_QUEUE_ERROR 5

void critical_error(uint32_t error_code, watchdog_context_t* watchdog);

#endif /* FAIL_H */
