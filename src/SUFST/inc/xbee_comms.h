
#ifndef XBEE_COMMS_H
#define XBEE_COMMS_H
#include <tx_api.h>
#include "xbee/xbee_device.h"

typedef struct {

  TX_THREAD thread;

} xbee_comms_context_t;

UINT xbee_comms_init(xbee_comms_context_t* xbee_comms_ptr, TX_BYTE_POOL* stack_pool_ptr);

void transmit_status_handler( xbee_dev_t *xbee,
   const void FAR *frame, uint16_t length, void FAR *context);


const xbee_dispatch_table_entry_t xbee_frame_handlers[
  { XBEE_FRAME_TRANSMIT_STATUS, 0, transmit_status_handler(), NULL}, //Fill in 0x8B Handler
  XBEE_FRAME_TABLE_END,
];


#endif /* XBEE_COMMS_H */

