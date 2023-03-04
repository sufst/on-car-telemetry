#ifndef CAN_UNPACK_H
#define CAN_UNPACK_H
#include "tx_api.h"
#include "telemetry_protocol.h"
#include "rtcan.h"
#include <stdint.h>
#include "watchdog.h"

#define CAN_PUBLISHER_RX_QUEUE_SIZE 10 //TODO: add config.h storing such values to avoid redefinition.

typedef struct {

  TX_TIMER bps_timer; /* Timer definition */
  TX_MUTEX stats_mutex; /* Mutex definition */
  int32_t rx_can_count; /* Received can frames */
  int32_t rx_can_bps; /* Received bits per second */
  int32_t rx_bytes;
  int32_t tx_pdu_count; /* Transmitted pdu frames */
  int32_t tx_pdu_bps; /* Transmitted bits per second */
  int32_t tx_bytes;
} unpack_stats_t;

typedef struct {

  TX_THREAD thread;

  TX_QUEUE rx_queue;

  rtcan_msg_t* rx_queue_mem[CAN_PUBLISHER_RX_QUEUE_SIZE];

  rtcan_handle_t* rtcan;

  unpack_stats_t stats;

  watchdog_context_t* watchdog;

} unpack_context_t;

UINT unpack_init(unpack_context_t* unpack_ptr, watchdog_context_t* watchdog_context, TX_BYTE_POOL* stack_pool_ptr, rtcan_handle_t* rtcan);

#endif /* CAN_UNPACK_H */
