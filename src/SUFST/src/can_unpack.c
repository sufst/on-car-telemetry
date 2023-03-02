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
#define STATS_TIMER_SECONDS                  1
#define STATS_TIMER_TICKS                    STATS_TIMER_SECONDS * TX_TIMER_TICKS_PER_SECOND
const uint32_t BITS_PER_BYTE = 8;

static void queue_receive_thread_entry(ULONG input);
static void stats_init(unpack_stats_t* stats);
static void stats_timer_callback(unpack_stats_t* stats);

UINT unpack_init(unpack_context_t* unpack_ptr, TX_BYTE_POOL* stack_pool_ptr){

VOID* thread_stack_ptr = NULL;
rtcan_status_t can_status;

    /* Initialise RTCAN instance */
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
    /* Init thread */
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
    /* Init queue */
    if (tx_status == TX_SUCCESS)
    {
        tx_status = tx_queue_create(&unpack_ptr->rx_queue,
                    "Unpack Rx Queue",
                    TX_1_ULONG,
                    unpack_ptr->rx_queue_mem,
                    sizeof(unpack_ptr->rx_queue));        
    }
    /* Init timer */
    if(tx_status == TX_SUCCESS)
    {
        tx_status =  tx_timer_create(&unpack_ptr->stats.bps_timer,
              "Data statistic timer",
              stats_timer_callback,
              &unpack_ptr->stats,
              STATS_TIMER_TICKS,
              STATS_TIMER_TICKS,
              TX_AUTO_ACTIVATE);
    }
    /* Init mutex */
    if(tx_status == TX_SUCCESS)
    {
        tx_status =  tx_mutex_create(&unpack_ptr->stats.stats_mutex,
              "Stats mutex",
              TX_INHERIT);
    }

    /* Subscribe to can messages*/
    if (tx_status == TX_SUCCESS)
    {
        for(int i = 0; i < TABLE_SIZE; i++)
        {
            can_status = rtcan_subscribe(&unpack_ptr->rtcan, can_handler_get(i)->identifier , &unpack_ptr->rx_queue);
            if(can_status != RTCAN_OK)
            {
                break; /* TODO: Add error handling */
            }
        }
    }
    /* Start RTCAN service */
    if(can_status == RTCAN_OK)
    {
        can_status = rtcan_start(&unpack_ptr->rtcan);
    }
    /* Initialise stats structure */
    if(can_status == RTCAN_OK)
    {
        stats_init(&unpack_ptr->stats);
    }

    return can_status;
}

void queue_receive_thread_entry(ULONG input)
{
    unpack_context_t* unpack_ptr = (unpack_context_t *) input;

    can_handler_t* handlerunpack = NULL;
    rtcan_msg_t* rx_msg_ptr;
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
        /* For statistic */
        tx_mutex_get(&unpack_ptr->stats.stats_mutex,TX_WAIT_FOREVER);
        unpack_ptr->stats.rx_can_count++;
        unpack_ptr->stats.rx_bytes += rx_msg_ptr->length;
        tx_mutex_put(&unpack_ptr->stats.stats_mutex);
        /* Find the can handler of matching identifier */
        int id = 0;
        for(; id<=TABLE_SIZE; id++)
        {
            handlerunpack = (can_handler_t *) can_handler_get(id);
            
            if(rx_msg_ptr->identifier == handlerunpack->identifier)
            {
              break;
            }
            /* Couldn't find matching identifier - deassign pointer. */
            if(id == TABLE_SIZE)
            {
              handlerunpack = NULL;
            }
        }
        /* Skip frame if couldn't find matching identifier. */
        if(handlerunpack == NULL)
        {
          // mark the original received message as consumed
          rtcan_msg_consumed(&unpack_ptr->rtcan, rx_msg_ptr);
          continue;
        }
        /* Check latest timestamp in ts_table, skip frame if not enough time has elapsed. Update ts_table. */
        /* This part will not be needed when this feature will be implemented in rtcan */
        l_timestamp = ts_table[id];
        c_timestamp = tx_time_get();
        if (c_timestamp - l_timestamp < 50)
        {
          // mark the original received message as consumed
          rtcan_msg_consumed(&unpack_ptr->rtcan, rx_msg_ptr);
          continue;
        }
        ts_table[id] = c_timestamp;
        
        /* Fill pdu_struct data buffer */
        handlerunpack->unpack_func((uint8_t *) &pdu_struct.data, rx_msg_ptr->data, rx_msg_ptr->length);

        // mark the original received message as consumed
        rtcan_msg_consumed(&unpack_ptr->rtcan, rx_msg_ptr);

        pdu_struct.header.epoch = c_timestamp; /* Assign timestamp */
        pdu_struct.start_byte = 1; /* Assign start byte */
        pdu_struct.ID = id; /* Assign PDU ID */
        pdu_struct.header.valid_bitfield = 1; /* Assign Valid_bitfield */

        /* Send pdu packet through UART */
        HAL_StatusTypeDef status = HAL_UART_Transmit(&huart4, (uint8_t *) &pdu_struct, sizeof(pdu_t), 10);

        if(status != HAL_OK)
        {
          return;
        }
        /* For statistic */
        tx_mutex_get(&unpack_ptr->stats.stats_mutex,TX_WAIT_FOREVER);
        unpack_ptr->stats.tx_pdu_count++;
        unpack_ptr->stats.tx_bytes += sizeof(pdu_struct);
        tx_mutex_put(&unpack_ptr->stats.stats_mutex);    
    }
}

void stats_init(unpack_stats_t* stats)
{
    stats->rx_can_bps = 0;
    stats->rx_can_count = 0;
    stats->tx_pdu_bps = 0;
    stats->tx_pdu_count = 0;
    stats->rx_bytes = 0;
    stats->tx_bytes = 0;
}

void stats_timer_callback(unpack_stats_t* stats)
{
    tx_mutex_get(&stats->stats_mutex,TX_WAIT_FOREVER);
    stats->rx_can_bps = (stats->rx_bytes * BITS_PER_BYTE) / STATS_TIMER_SECONDS; /* Amount of bits received per second */
    stats->tx_pdu_bps = (stats->tx_bytes * BITS_PER_BYTE) / STATS_TIMER_SECONDS; /* Amount of bits sent per second */

    /* Set counter to zeroes */
    stats->rx_bytes = 0;
    stats->tx_bytes = 0;
    tx_mutex_put(&stats->stats_mutex);
}