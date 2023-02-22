#ifndef CAN_UNPACK_H
#define CAN_UNPACK_H
#include "tx_api.h"
#include "telemetry_protocol.h"
#include "rtcan.h"

#define CAN_PUBLISHER_RX_QUEUE_SIZE 10 //TODO: add config.h storing such values to avoid redefinition.

typedef struct {

  TX_THREAD thread;

  TX_QUEUE rx_queue;

  rtcan_msg_t* rx_queue_mem[CAN_PUBLISHER_RX_QUEUE_SIZE];

  rtcan_handle_t rtcan;

} unpack_context_t;

UINT unpack_init(unpack_context_t* unpack_ptr, TX_BYTE_POOL* stack_pool_ptr);


#endif /* CAN_UNPACK_H */
