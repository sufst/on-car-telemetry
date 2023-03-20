
#ifndef XBEE_COMMS_H
#define XBEE_COMMS_H
#include <tx_api.h>
#include "xbee_device.h"
#include "xbee_wpan.h"
#include "telemetry_protocol.h"
#include "xbee_discovery.h"
#include "zigbee_zdo.h"
#include "xbee_transparent_serial.h"

typedef struct {

  TX_THREAD thread;

  TX_QUEUE* rx_queue;

} xbee_comms_context_t;

UINT xbee_comms_init(xbee_comms_context_t* xbee_comms_ptr, TX_QUEUE* rx_queue, TX_BYTE_POOL* stack_pool_ptr);

#endif /* XBEE_COMMS_H */

