
#ifndef XBEE_COMMS_H
#define XBEE_COMMS_H
#include <tx_api.h>
#include "xbee/device.h""
#include "xbee/wpan.h"
#include "telemetry_protocol.h"
#include "xbee/discovery.h"
#include "zigbee/zdo.h"
#include "xbee/transparent_serial.h"

typedef struct {

  TX_THREAD thread;

  TX_QUEUE* rx_queue;

  xbee_serial_t xbee_serial;

  xbee_dev_t xbee_dev;

} xbee_comms_context_t;

UINT xbee_comms_init(xbee_comms_context_t* xbee_comms_ptr, TX_QUEUE* rx_queue, TX_BYTE_POOL* stack_pool_ptr);

#endif /* XBEE_COMMS_H */

