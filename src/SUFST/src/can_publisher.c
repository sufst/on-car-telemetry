#include <tx_api.h>
#include "rtcan.h"
#include "can_publisher.h"
#include "can_database.h"
#include "Debug/testbench_can_data.h"
#include "error_handler.h"

#define QUEUE_SEND_THREAD_PRIORITY             10
#define QUEUE_SEND_THREAD_STACK_SIZE           1024
#define QUEUE_SEND_THREAD_PREEMPTION_THRESHOLD 10                

void queue_send_thread_entry(ULONG input);

/**
 * @brief 		Creates the Unpack thread
 *
 * @param[in]	stack_pool_ptr	Pointer to start of application stack area
 *
 * @return		See ThreadX return codes
 */
UINT can_publisher_init(publisher_context_t* publisher_ptr, TX_QUEUE * rx_queue, TX_BYTE_POOL* stack_pool_ptr)
{

    VOID* thread_stack_ptr = NULL;

    publisher_ptr->tx_queue = rx_queue;

    UINT tx_status = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                QUEUE_SEND_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    if (tx_status == TX_SUCCESS)
    {
        tx_status = tx_thread_create(&publisher_ptr->thread,
                               "CAN Publisher Thread",
                               queue_send_thread_entry,
                               publisher_ptr,
                               thread_stack_ptr,
                               QUEUE_SEND_THREAD_STACK_SIZE,
                               QUEUE_SEND_THREAD_PRIORITY,
                               QUEUE_SEND_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    return tx_status;
}

void queue_send_thread_entry(ULONG input)
{
    publisher_context_t* publisher_ptr = (publisher_context_t*) input;

    // Simulated CAN message
    rtcan_msg_t queue_data;
    uint16_t debug_index = 0;

    while(1){
    
    queue_data.identifier = CAN_DATABASE_PM100_VOLTAGE_INFO_FRAME_ID;
    queue_data.length = CAN_DATABASE_PM100_VOLTAGE_INFO_LENGTH;

    // Parse lookup table of dummy data here and pack it in queue_data
    for(int i = 0; i < 8; i++)
    {
        queue_data.data[i] = debug_lookup[debug_index%16 + i];
    }
    debug_index++;

    // Send the data to the queue.
    UINT ret = tx_queue_send(publisher_ptr->tx_queue, (rtcan_msg_t*) &queue_data, TX_WAIT_FOREVER);
    if(ret != TX_SUCCESS){
        return;
    }
    // Introduce 500ms delay
    tx_thread_sleep(50);
    }

}