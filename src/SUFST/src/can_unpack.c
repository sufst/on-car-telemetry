#ifndef CAN_UNPACK_C
#define CAN_UNPACK_C
#include <tx_api.h>

#include "can_publisher.h"
#include "can_database.h"
#include "can_handlers.h"
//#include "telemetry_protocol.h"
#include "can_unpack.h"
TX_QUEUE queue_spi;
/**
 * @brief Queue_Rx thread instance
 */
static TX_THREAD  Queue_Rx_thread;


#define QUEUE_RX_THREAD_PRIORITY             10
#define QUEUE_RX_THREAD_STACK_SIZE           512
#define QUEUE_RX_THREAD_PREEMPTION_THRESHOLD 10
#define QUEUE_RX_THREAD_NAME                 "Queue_Rx Thread"

#define QUEUE_SIZE 100
static ULONG queue_memory_area[QUEUE_SIZE * sizeof(pdu_t)];

// Simulated CAN message
rtcan_msg_t queue_data;

/**
 * @brief 		Creates the Unpack thread
 *
 * @param[in]	stack_pool_ptr	Pointer to start of application stack area
 *
 * @return		See ThreadX return codes
 */
UINT queue_receive_thread_create(TX_BYTE_POOL* stack_pool_ptr)
{
    VOID* thread_stack_ptr;

    UINT ret = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                QUEUE_RX_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    if (ret == TX_SUCCESS)
    {
        ret = tx_thread_create(&Queue_Rx_thread,
                               QUEUE_RX_THREAD_NAME,
                               queue_receive_thread_entry,
                               0,
                               thread_stack_ptr,
                               QUEUE_RX_THREAD_STACK_SIZE,
                               QUEUE_RX_THREAD_PRIORITY,
                               QUEUE_RX_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    return ret;
}

void queue_receive_thread_entry(ULONG input)
{
can_handler_t* handlerunpack;
pdu_t pdu_struct;
uint32_t l_timestamp, c_timestamp;
  while(1){

  // Receive data from the queue.
  tx_queue_receive(&queue, &queue_data, TX_WAIT_FOREVER);
  
  // Find the can handler of matching identifier
  int i = 0;
    do{
        handlerunpack = can_handler_get(i);
        i++;
    }
    while(queue_data.identifier != handlerunpack->identifier && i < 20);

  // Check latest timestamp in ts_table, skip frame if not enough time has elapsed. Update ts_table.
  l_timestamp = ts_table[i-1];
  c_timestamp = tx_time_get(); 
  if(c_timestamp - l_timestamp < 500){continue;}

  ts_table[i-1] = c_timestamp;
  
  // Fill pdu_struct data buffer
  handlerunpack->unpack_func(&pdu_struct.data, queue_data.data, queue_data.length);

  pdu_struct.header.epoch = c_timestamp; // Assign timestamp
  pdu_struct.start_byte = 1; // Assign start byte
  pdu_struct.ID = 0; // Assign PDU ID
  pdu_struct.header.valid_bitfield = 1; // Assign Valid_bitfield

  //Ready bitstream to be sent by SPI.
  tx_queue_send(&queue_spi, &pdu_struct, TX_WAIT_FOREVER);


  }

}


#endif /* CAN_UNPACK_C */
