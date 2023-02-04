#include <tx_api.h>
#include "can_publisher.h"
#include "can_database.h"

#define QUEUE_SIZE 100

// Simulated CAN message
rtcan_msg_t queue_data;

void queue_send_thread_entry(ULONG input)
{

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