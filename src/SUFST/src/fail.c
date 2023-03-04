#include <stdint.h>
#include "fail.h"
#include "watchdog.h"

void critical_error(uint32_t error_code, watchdog_context_t* watchdog)
{

watchdog->error_code = error_code;
tx_semaphore_put(&watchdog->fault_semaphore);

}