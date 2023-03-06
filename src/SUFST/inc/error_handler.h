
#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <stdint.h>
#include <tx_api.h>
#include <stdatomic.h>

#define ERROR_HANDLER_TX_QUEUE_SIZE 10

#define CAN_UNPACK_ERROR_INIT 1
#define RTCAN_SUBSCRIBE_ERROR_INIT 2
#define RTCAN_START_ERROR 3
#define STATS_MUTEX_ERROR 4
#define CAN_RX_QUEUE_ERROR 5

typedef struct {

  TX_THREAD thread;

  TX_QUEUE tx_queue;

  atomic_uint error_code;

  TX_SEMAPHORE fault_semaphore;

} error_handler_context_t;

UINT error_handler_init(error_handler_context_t* error_handler_ptr, TX_BYTE_POOL* stack_pool_ptr);
void critical_error(TX_THREAD *thread_ptr, uint32_t error_code, error_handler_context_t* error_handler);

#endif /* ERROR_HANDLER_H */
