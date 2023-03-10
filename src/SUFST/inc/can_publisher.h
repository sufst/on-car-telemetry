#ifndef CAN_PUBLISHER_H
#define CAN_PUBLISHER_H
#include <stdint.h>
#include <tx_api.h>
#include "telemetry_protocol.h"
#include "can_unpack.h"

#define CAN_PUBLISHER_TX_QUEUE_SIZE 10

UINT can_publisher_init(unpack_context_t* publisher_ptr, TX_BYTE_POOL* stack_pool_ptr);
TX_QUEUE * can_publisher_get_queue_ptr(unpack_context_t* pub_context);

#endif /* CAN_PUBLISHER_H */