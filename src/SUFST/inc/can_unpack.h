#ifndef CAN_UNPACK_H
#define CAN_UNPACK_H
#include "tx_api.h"
#include "telemetry_protocol.h"


#define TX_QUEUE_SIZE 10
#define RX_QUEUE_SIZE 10

typedef struct {

  TX_THREAD thread;

  TX_QUEUE tx_queue;

  pdu_t tx_queue_mem[TX_QUEUE_SIZE];

  TX_QUEUE * rx_queue;

  rtcan_msg_t* rx_queue_mem[RX_QUEUE_SIZE];

} unpack_context_t;

UINT unpack_init(unpack_context_t* unpack_ptr, TX_BYTE_POOL* stack_pool_ptr, TX_QUEUE * can_queue);
void queue_receive_thread_entry(ULONG input);


#endif /* CAN_UNPACK_H */
