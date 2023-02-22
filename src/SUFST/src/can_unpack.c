#include <tx_api.h>

#include "can_handlers.h"
#include "can_unpack.h"
#include "usart.h"
#include "rtcan.h"
#include "can.h"

#define RTCAN_THREAD_PRIORITY   3
#define QUEUE_RX_THREAD_PRIORITY             10
#define QUEUE_RX_THREAD_STACK_SIZE           1024
#define QUEUE_RX_THREAD_PREEMPTION_THRESHOLD 10

void queue_receive_thread_entry(ULONG input);

UINT unpack_init(unpack_context_t* unpack_ptr, TX_BYTE_POOL* stack_pool_ptr){

VOID* thread_stack_ptr = NULL;

    // initialise RTCAN instance
    UINT tx_status = rtcan_init(&unpack_ptr->rtcan, 
               &hcan1, 
               RTCAN_THREAD_PRIORITY, 
               stack_pool_ptr);

    if(tx_status == RTCAN_OK)
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
                               (ULONG) unpack_ptr,
                               thread_stack_ptr,
                               QUEUE_RX_THREAD_STACK_SIZE,
                               QUEUE_RX_THREAD_PRIORITY,
                               QUEUE_RX_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }
    if (tx_status == TX_SUCCESS)
    {
        tx_status = tx_queue_create(&unpack_ptr->rx_queue,
                    "My Rx Queue",
                    TX_1_ULONG,
                    unpack_ptr->rx_queue_mem,
                    sizeof(unpack_ptr->rx_queue));        
    }
    
    // subscribe to a message
    if (tx_status == TX_SUCCESS)
    {
        for(int i = 0; i < TABLE_SIZE; i++)
        {
            tx_status = rtcan_subscribe(&unpack_ptr->rtcan, can_handler_get(i)->identifier , &unpack_ptr->rx_queue);
            if(tx_status != RTCAN_OK)
            {
                break;
            }
        }
    }

    if(tx_status == RTCAN_OK)
    {
        tx_status = rtcan_start(&unpack_ptr->rtcan);
    }

    // start the RTCAN service
    return tx_status;
}

void queue_receive_thread_entry(ULONG input)
{
    unpack_context_t* unpack_ptr = (unpack_context_t *) input;

    can_handler_t* handlerunpack = NULL;
    rtcan_msg_t* rx_msg_ptr;
    rtcan_msg_t rx_msg;
    pdu_t pdu_struct;
    uint32_t l_timestamp, c_timestamp;
    
    while (1)
    {
        int ret;
        
        /* Receive data from the queue. */
        ret = tx_queue_receive(&unpack_ptr->rx_queue,
                                    (void *) &rx_msg_ptr,
                                    TX_WAIT_FOREVER);
        if (ret != TX_SUCCESS)
        {
            return;
        }
        memcpy((void*) rx_msg.data, (void*) rx_msg_ptr->data, rx_msg_ptr->length);

        // mark the original received message as consumed
        rtcan_msg_consumed(&unpack_ptr->rtcan, rx_msg_ptr);

        /* Find the can handler of matching identifier */
        int id = 0;
        for(; id<=TABLE_SIZE; id++)
        {
            handlerunpack = (can_handler_t *) can_handler_get(i);
            
            if(rx_msg.identifier == handlerunpack->identifier)
            {
              break;
            }
            /* Couldn't find matching identifier - deassign pointer. */
            if(id == TABLE_SIZE)handlerunpack = NULL;
        }
        /* Skip frame if couldn't find matching identifier. */
        if(handlerunpack == NULL)
        {
          continue;
        }
        /* Check latest timestamp in ts_table, skip frame if not enough time has elapsed. Update ts_table. */
        /* This part will not be needed when this feature will be implemented in rtcan */
        l_timestamp = ts_table[id];
        c_timestamp = tx_time_get();
        if (c_timestamp - l_timestamp < 50)
        {
          continue;
        }
        ts_table[id] = c_timestamp;

        /* Fill pdu_struct data buffer */
        handlerunpack->unpack_func((uint8_t *) &pdu_struct.data, rx_msg.data, rx_msg.length);

        pdu_struct.header.epoch = c_timestamp; /* Assign timestamp */
        pdu_struct.start_byte = 1; /* Assign start byte */
        pdu_struct.ID = id; /* Assign PDU ID */
        pdu_struct.header.valid_bitfield = 1; /* Assign Valid_bitfield */

        /* Send pdu packet through UART */
        HAL_StatusTypeDef status = HAL_UART_Transmit(&huart4, (uint8_t *) &pdu_struct, sizeof(pdu_t), 10);

        if(status != HAL_OK){
            return;
        }
    }
}
