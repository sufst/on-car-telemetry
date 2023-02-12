#include <tx_api.h>

#include "can_handlers.h"
#include "can_unpack.h"

#define QUEUE_RX_THREAD_PRIORITY             10
#define QUEUE_RX_THREAD_STACK_SIZE           1024
#define QUEUE_RX_THREAD_PREEMPTION_THRESHOLD 10

UINT unpack_init(unpack_context_t* unpack_ptr, TX_BYTE_POOL* stack_pool_ptr){

VOID* thread_stack_ptr = NULL;
/* Setup transmit queue */
    UINT tx_status = tx_queue_create(&unpack_ptr->tx_queue,
                                     "SPI Data Receive Queue",
                                     sizeof(pdu_t)/sizeof(ULONG),
                                     &unpack_ptr->tx_queue_mem,
                                     TX_QUEUE_SIZE * sizeof(pdu_t));

/* Setup receive queue */
    if(tx_status == TX_SUCCESS)
    {
        tx_status = tx_queue_create(&unpack_ptr->rx_queue,
                                     "Unpack CAN Receive Queue",
                                     sizeof(rtcan_msg_t*)/sizeof(ULONG),
                                     &unpack_ptr->rx_queue_mem,
                                     TX_QUEUE_SIZE * sizeof(rtcan_msg_t));
    }

    if(tx_status == TX_SUCCESS)
    {
        tx_status = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                QUEUE_RX_THREAD_STACK_SIZE,
                                TX_NO_WAIT);
    }

    if (tx_status == TX_SUCCESS)
    {
        tx_status = tx_thread_create(&unpack_ptr->thread,
                               "Queue_Rx Thread",
                               queue_receive_thread_entry,
                               unpack_ptr,
                               thread_stack_ptr,
                               QUEUE_RX_THREAD_STACK_SIZE,
                               QUEUE_RX_THREAD_PRIORITY,
                               QUEUE_RX_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    return tx_status;
}

void queue_receive_thread_entry(ULONG input)
{
    unpack_context_t* unpack_ptr = (unpack_context_t*) input;

    can_handler_t* handlerunpack = NULL;
    pdu_t pdu_struct;
    uint32_t l_timestamp, c_timestamp;
    
    while (1)
    {
        int ret;
        rtcan_msg_t* rx_msg;

        /* Receive data from the queue. */
        ret = tx_queue_receive(&unpack_ptr->rx_queue, &rx_msg, TX_WAIT_FOREVER);
        if (ret != TX_SUCCESS)
        {
            return ret;
        }
        /* Find the can handler of matching identifier */
        int i = 0;
        for(i; i<=TABLE_SIZE; i++)
        {
            handlerunpack = can_handler_get(i);
            
            if(&rx_msg->identifier == handlerunpack->identifier)
            {
              break;
            }
            /* Couldn't find matching identifier - deassign pointer. */
            if(i == TABLE_SIZE)handlerunpack = NULL;
        }
        /* Skip frame if couldn't find matching identifier. */
        if(handlerunpack == NULL)
        {
          continue;
        }
        /* Check latest timestamp in ts_table, skip frame if not enough time has elapsed. Update ts_table. */
        l_timestamp = ts_table[i];
        c_timestamp = tx_time_get();
        if (c_timestamp - l_timestamp < 500)
        {
          continue;
        }
        ts_table[i] = c_timestamp;

        /* Fill pdu_struct data buffer */
        handlerunpack->unpack_func(&pdu_struct.data, &rx_msg->data, &rx_msg->length);

        pdu_struct.header.epoch = c_timestamp; /* Assign timestamp */
        pdu_struct.start_byte = 1; /* Assign start byte */
        pdu_struct.ID = 0; /* Assign PDU ID */
        pdu_struct.header.valid_bitfield = 1; /* Assign Valid_bitfield */

        /* Ready bitstream to be sent by SPI. */
        ret = tx_queue_send(&unpack_ptr->tx_queue, &pdu_struct, TX_WAIT_FOREVER);
        if (ret != TX_SUCCESS)
        {
            return ret;
        }
    }
}
