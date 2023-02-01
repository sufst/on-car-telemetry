/**
 * @file can-sim.c
 * @author Piotr Mikulowski (pm4u19@soton.ac.uk)
 * @brief Can-bus simulator for on-car telemetry software testing
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include "can_database.c"
#include "can_handlers.c"


/**
 * @brief can_database_m173_modulation_and_flux_info_s struct
 * 
    int16_t inv_modulation_index;
    int16_t inv_flux_weakening_output;
    int16_t inv_id_command;
    int16_t inv_iq_command;
 * 
 */
struct can_database_m173_modulation_and_flux_info_t can_database_m173_modulation_and_flux_info_s = {43 , -1230 , -8 , 647};
struct can_database_m173_modulation_and_flux_info_t can_database_m173_modulation_and_flux_info_target = {0 , 0 , 0 , 0};

/**
 * @brief   RTCAN message (normally defined in rtcan.h)
 *
 * @note    This must have a size that is a multiple of sizeof(ULONG) for use
 *          with TX_QUEUE
 */
typedef struct
{
    /**
     * @brief   CAN standard identifier for message
     */
    uint32_t identifier;

    /**
     * @brief   Message data buffer
     */
    uint8_t data[8];

    /**
     * @brief   Length of message data in bytes
     */
    uint32_t length;

} rtcan_msg_t;

//rtcan_msg_t message;
/*
1.Encode data
2.Pack data
3.Send to other thread.
*/

int main(){


printf("This struct has members: %d %d %d %d\n",    can_database_m173_modulation_and_flux_info_s.inv_modulation_index, \
                                    can_database_m173_modulation_and_flux_info_s.inv_flux_weakening_output, \
                                    can_database_m173_modulation_and_flux_info_s.inv_id_command, \
                                    can_database_m173_modulation_and_flux_info_s.inv_iq_command);

rtcan_msg_t message;
//Pack into memory bitstream and to message struct

const can_handler_t* handlerpack = can_handler_get(0);
handlerpack->pack_func(message.data, &can_database_m173_modulation_and_flux_info_s, CAN_DATABASE_M173_MODULATION_AND_FLUX_INFO_LENGTH);

message.identifier = CAN_DATABASE_M173_MODULATION_AND_FLUX_INFO_FRAME_ID;
message.length = CAN_DATABASE_M173_MODULATION_AND_FLUX_INFO_LENGTH;

//Send struct message to queue (other thread)
//
//Send_queue(message)
//

//Repeat with other data input (see dictionary)



//Receiver part:
//Receive_queue(message)
//Find the entry with matching id with message.identifier
//const can_handler_t* handler = can_handler_get(0); //Find the entry of matching id

const can_handler_t* handlerunpack = can_handler_get(0); //Find the entry of matching id
handlerunpack->unpack_func(&can_database_m173_modulation_and_flux_info_target, message.data, message.length);

printf("Another struct has members: %d %d %d %d\n",    can_database_m173_modulation_and_flux_info_target.inv_modulation_index, \
                                    can_database_m173_modulation_and_flux_info_target.inv_flux_weakening_output, \
                                    can_database_m173_modulation_and_flux_info_target.inv_id_command, \
                                    can_database_m173_modulation_and_flux_info_target.inv_iq_command);

return 0;
}

/*
void simulation_thread(){
rtcan_msg_t message;
int index = 0;

while(1){
    int ret = can_database_m173_modulation_and_flux_info_pack(message.data, &can_database_m173_modulation_and_flux_info_s, CAN_DATABASE_M173_MODULATION_AND_FLUX_INFO_LENGTH);
    if(ret < 0 )return ret;

    message.identifier = CAN_DATABASE_M173_MODULATION_AND_FLUX_INFO_FRAME_ID;
    message.length = CAN_DATABASE_M173_MODULATION_AND_FLUX_INFO_LENGTH;


    tx_thread_sleep(1000); 
}
}
*/
