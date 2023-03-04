
#ifndef WATCHDOG_H
#define WATCHDOG_H
#include <stdint.h>
#include <tx_api.h>

#define WATCHDOG_TX_QUEUE_SIZE 10

typedef struct {

  TX_THREAD thread;

  TX_QUEUE tx_queue;

  uint32_t error_code;

  TX_SEMAPHORE fault_semaphore;

} watchdog_context_t;

UINT watchdog_init(watchdog_context_t* watchdog_ptr, TX_BYTE_POOL* stack_pool_ptr);

#endif /* WATCHDOG_H */
