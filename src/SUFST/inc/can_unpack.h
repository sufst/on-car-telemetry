#ifndef CAN_UNPACK_H
#define CAN_UNPACK_H
#include "tx_api.h"
#include "telemetry_protocol.h"

extern TX_QUEUE queue_spi;

UINT queue_receive_thread_create(TX_BYTE_POOL* stack_pool_ptr);
void queue_receive_thread_entry(ULONG input);


#endif /* CAN_UNPACK_H */
