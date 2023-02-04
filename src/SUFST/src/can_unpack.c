#include <tx_api.h>

#include "can_publisher.h"
#include "can_database.h"
#include "can_handlers.h"
#include "telemetry_protocol.h"
#include "can_unpack.h"

#define QUEUE_SIZE 100

// Simulated CAN message
rtcan_msg_t queue_data;

void queue_receive_thread_entry(ULONG input)
{
can_handler_t* handlerunpack;
pdu_t pdu_struct;
int l_timestamp, c_timestamp;
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
  l_timestamp = ts_table[i-1].timestamp;
  c_timestamp = tx_time_get(); 
  if(c_timestamp - l_timestamp < 500){continue;}

  ts_table[i-1].timestamp = c_timestamp;
  
  // Fill pdu_struct data buffer
  handlerunpack->unpack_func(&pdu_struct.data, queue_data.data, queue_data.length);

  pdu_struct.header.epoch = c_timestamp; // Assign timestamp
  pdu_struct.start_byte = 1; // Assign start byte
  pdu_struct.ID = 0; // Assign PDU ID
  pdu_struct.header.valid_bitfield = 1; // Assign Valid_bitfield

  //Ready bitstream to be sent by SPI.
  tx_queue_send(&queue_spi, &pdu_struct, TX_WAIT_FOREVER);

  // Introduce delay 500ms
  tx_thread_sleep(500);

  }

}