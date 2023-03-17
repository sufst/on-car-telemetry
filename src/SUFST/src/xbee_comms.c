#include <tx_api.h>
#include "rtcan.h"
#include "xbee_comms.h"
#include "can_database.h"
#include "Debug/testbench_can_data.h"
#include "error_handler.h"

/* Include transmit frame API layer */
#include "xbee/xbee_wpan.h"
#include "xbee/device.h"

#define QUEUE_SEND_THREAD_PRIORITY             10
#define QUEUE_SEND_THREAD_STACK_SIZE           1024
#define QUEUE_SEND_THREAD_PREEMPTION_THRESHOLD 10                

void xbee_comms_entry(ULONG input);

/**
 * @brief 		Creates the Xbee Comms thread
 *
 * @param[in]	stack_pool_ptr	Pointer to start of application stack area
 *
 * @return		See ThreadX return codes
 */
UINT xbee_comms_init(xbee_comms_context_t* xbee_comms_ptr, TX_BYTE_POOL* stack_pool_ptr)
{

    VOID* thread_stack_ptr = NULL;
    //@todo Implement queue
    UINT tx_status = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                QUEUE_SEND_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    if (tx_status == TX_SUCCESS)
    {
        tx_status = tx_thread_create(&xbee_comms_ptr->thread,
                               "Xbee Communication Thread",
                               xbee_comms_entry,
                               xbee_comms_ptr,
                               thread_stack_ptr,
                               QUEUE_SEND_THREAD_STACK_SIZE,
                               QUEUE_SEND_THREAD_PRIORITY,
                               QUEUE_SEND_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    xbee_dev_init (xbee_dev_t *xbee, const xbee_serial_t *serport, xbee_is_awake_fn is_awake, xbee_reset_fn reset); //@todo Implement

    xbee_wpan_init( xbee_dev_t *xbee, const wpan_endpoint_table_entry_t *ep_table); //@todo Implement

    return tx_status;
}

void xbee_comms_entry(ULONG input)
{
    xbee_comms_context_t* publisher_ptr = (xbee_comms_context_t*) input;

    
    xbee_header_transmit_explicit_t transmit_frame;

    while(1){
    
        //@Todo Payload data queue receive

        //@Todo Create Frame (Transmit Request - 0x10 or 0x11)
        
        //Write Frame (Transmit Request - 0x10 or 0x11)  
        int xbee_frame_write(xbee_dev_t * xbee,
                            const void FAR * 	header,
                            uint16_t 	headerlen,
                            const void FAR * 	data,
                            uint16_t 	datalen,
                            uint16_t 	flags
        );

        //Check for newly received frames (Transmit Status)
        xbee_dev_tick (xbee_dev_t *xbee); // @todo Currently based on blocking UART function (see xbee_serial_stm32). Should it be non-blocking interrupt based?


        // Introduce 500ms delay
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 2);
    }

    return;
}

/* Function for handling Received Transmit Status Frame */

void transmit_status_handler( xbee_dev_t *xbee,
   const void FAR *frame, uint16_t length, void FAR *context){

    const xbee_frame_transmit_status_t FAR *payload = frame;

    XBEE_UNUSED_PARAMETER(xbee);
    XBEE_UNUSED_PARAMETER(length);
    XBEE_UNUSED_PARAMETER(context);

    /* @todo If success - implement rx/tx stats counter */

    return;
}