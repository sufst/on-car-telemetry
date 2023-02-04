#ifndef CAN_PUBLISHER_H
#define CAN_PUBLISHER_H
#include <stdint.h>
#include <tx_api.h>

extern TX_QUEUE queue;
UINT queue_send_thread_create(TX_BYTE_POOL* stack_pool_ptr);
void queue_send_thread_entry(ULONG input);


#endif /* CAN_PUBLISHER_H */
