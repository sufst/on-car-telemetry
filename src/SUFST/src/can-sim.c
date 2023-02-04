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
#include "telemetry_protocol.h"



int main(){

/*
printf("This struct has members: %d %d %d %d\n",    can_database_m173_modulation_and_flux_info_s.inv_modulation_index, \
                                    can_database_m173_modulation_and_flux_info_s.inv_flux_weakening_output, \
                                    can_database_m173_modulation_and_flux_info_s.inv_id_command, \
                                    can_database_m173_modulation_and_flux_info_s.inv_iq_command);
*/
rtcan_msg_t message;
pdu_t pdu_struct;
can_handler_t* handlerunpack;
timestamp_table_t* ts;

//Pack into memory bitstream and to message struct

for(int i = 0; i < 7; i++){
message.data[i] = 0x00 + i; //Input data from CAN / Simulator
}
message.identifier = CAN_DATABASE_M160_TEMPERATURE_SET_1_FRAME_ID;
message.length = CAN_DATABASE_M160_TEMPERATURE_SET_1_LENGTH;

int j = 0;
do{
    ts = &ts_table[j];
    j++;
}
while(message.identifier != ts->identifier && j < 20);

if(tx_time_get() - ts->timestamp <= 500){return 0; } //Skip if too often 500ms
ts->timestamp = tx_time_get(); //tx_time_get()

//Send struct message to queue (other thread)
/*Send_queue(message)*/

//- - - - - ->

//Receiver part:
//Receive_queue(message)

//Find the entry with matching id with message.identifier
int i = 0;
do{
    handlerunpack = can_handler_get(i); //Find the entry of matching id
    i++;
}
while(message.identifier != handlerunpack->identifier && i < 20);


handlerunpack->unpack_func(&pdu_struct.data, message.data, message.length);

pdu_struct.start_byte = 1; //Assign start byte
pdu_struct.ID = 0; //Assign PDU ID
pdu_struct.header.epoch = handlerunpack->l_timestamp;
//pdu_struct.header.epoch = 567; //Assign Timestamp - use tx_time_get()
pdu_struct.header.valid_bitfield = 0;



printf("\nAnother struct has members:");
for(int i = 0; i < 7; i++){

printf("%x", pdu_struct.data[i]);

}
printf("This is epoch: %d\n", pdu_struct.header.epoch);

//Ready bitstream to be sent by SPI.
//Send_queue(pdu_struct)

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
