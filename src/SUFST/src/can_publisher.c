#ifndef CAN_PUBLISHER_C
#define CAN_PUBLISHER_C
#include <tx_api.h>
#include "can_publisher.h"
#include "can_database.h"
#include "telemetry_protocol.h"

#define QUEUE_SIZE 100
/**
 * @brief Queue_Send thread instance
 */

TX_QUEUE queue;
static TX_THREAD  Queue_Send_thread;
static ULONG queue_memory_area[QUEUE_SIZE * sizeof(rtcan_msg_t)];

#define QUEUE_SEND_THREAD_PRIORITY             10
#define QUEUE_SEND_THREAD_STACK_SIZE           512
#define QUEUE_SEND_THREAD_PREEMPTION_THRESHOLD 10
#define QUEUE_SEND_THREAD_NAME                 "Queue_Send Thread"

/**
 * @brief 		Creates the Unpack thread
 *
 * @param[in]	stack_pool_ptr	Pointer to start of application stack area
 *
 * @return		See ThreadX return codes
 */
UINT queue_send_thread_create(TX_BYTE_POOL* stack_pool_ptr)
{
    tx_queue_create(&queue, "CANQueue", QUEUE_SIZE, queue_memory_area, sizeof(ULONG));

    VOID* thread_stack_ptr;

    UINT ret = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                QUEUE_SEND_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    if (ret == TX_SUCCESS)
    {
        ret = tx_thread_create(&Queue_Send_thread,
                               QUEUE_SEND_THREAD_NAME,
                               queue_send_thread_entry,
                               0,
                               thread_stack_ptr,
                               QUEUE_SEND_THREAD_STACK_SIZE,
                               QUEUE_SEND_THREAD_PRIORITY,
                               QUEUE_SEND_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    return ret;
}

void queue_send_thread_entry(ULONG input)
{
    // Simulated CAN message
    rtcan_msg_t queue_data;

    while(1){
    
    // Simulate CAN Message here. TODO: Get input from file.

    queue_data.identifier = CAN_DATABASE_PM100_VOLTAGE_INFO_FRAME_ID;
    queue_data.length = CAN_DATABASE_PM100_VOLTAGE_INFO_LENGTH;

    for(int i = 0; i<8; i++){

    queue_data.data[i] = i;

    }

    // Send the data to the queue.
    tx_queue_send(&queue, &queue_data, TX_WAIT_FOREVER);

    // Introduce 500ms delay
    tx_thread_sleep(500);
    }

}


#endif /* CAN_PUBLISHER_C */
