#include <tx_api.h>
#include "error_handler.h"
#include "can_handlers.h"
#include "gps_comms.h"
#include "config.h"

#define GPS_COMMS_THREAD_PRIORITY             10
#define GPS_COMMS_THREAD_STACK_SIZE           1024
#define GPS_COMMS_THREAD_PREEMPTION_THRESHOLD 10

static void gps_comms_entry(ULONG input);

UINT gps_comms_init(gps_comms_context_t* gps_comms_ptr, error_handler_context_t* error_handler_context, TX_BYTE_POOL* stack_pool_ptr, rtcan_handle_t* rtcan){

    VOID* thread_stack_ptr = NULL;

    UINT tx_status;

    gps_comms_ptr->error_handler = error_handler_context;

    gps_comms_ptr->rtcan = rtcan;

    tx_status = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                GPS_COMMS_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    /* Init thread */
    if (tx_status == TX_SUCCESS)
    {
        tx_status = tx_thread_create(&gps_comms_ptr->thread,
                               "GPS Comms Thread",
                               gps_comms_entry,
                               (ULONG) gps_comms_ptr,
                               thread_stack_ptr,
                               GPS_COMMS_THREAD_STACK_SIZE,
                               GPS_COMMS_THREAD_PRIORITY,
                               GPS_COMMS_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    /* Error handling - GPS Comms Thread Initialisation failed */
    if(tx_status != TX_SUCCESS)
    {
        critical_error(&gps_comms_ptr->thread, GPS_COMMS_INIT_ERROR, gps_comms_ptr->error_handler);
        return tx_status;
    }

    return tx_status;
}

void gps_comms_entry(ULONG input)
{
    gps_comms_context_t* gps_comms_ptr = (gps_comms_context_t *) input;

    while (1)
    {
        /* Main GPS Loop */


    }
}