#ifndef GPS_COMMS_H
#define GPS_COMMS_H
#include "tx_api.h"
#include "telemetry_protocol.h"
#include <stdint.h>
#include "error_handler.h"
#include "rtcan.h"

typedef struct {

  TX_THREAD thread;

  rtcan_handle_t* rtcan;

  error_handler_context_t* error_handler;

} gps_comms_context_t;

UINT gps_comms_init(gps_comms_context_t* gps_comms_ptr, error_handler_context_t* error_handler_context, TX_BYTE_POOL* stack_pool_ptr, rtcan_handle_t* rtcan);

#endif /* GPS_COMMS_H */