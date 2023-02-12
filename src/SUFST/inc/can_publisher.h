#ifndef CAN_PUBLISHER_H
#define CAN_PUBLISHER_H
#include <stdint.h>
#include <tx_api.h>
#include "telemetry_protocol.h"

#define TX_QUEUE_SIZE 10

typedef struct {

  TX_THREAD thread;

  TX_QUEUE tx_queue;

  rtcan_msg_t* tx_queue_mem[TX_QUEUE_SIZE];

} publisher_context_t;

UINT can_publisher_init(publisher_context_t* publisher_ptr, TX_BYTE_POOL* stack_pool_ptr);
void queue_send_thread_entry(ULONG input);

#endif /* CAN_PUBLISHER_H */
