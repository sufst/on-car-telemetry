#include <stdint.h>
#include <can_database.h>

/**
 * @brief  Frame Header
 * @note 
 * @retval None  
 */

typedef struct header_s
{
    uint16_t valid_bitfield;
    unsigned int    epoch;
} header_t;

/**
 * @brief   
 *
 * @note    
 *          
 */
typedef struct
{
    /**
     * @brief   
     */
    unsigned char start_byte;

    /**
     * @brief   
     */
    uint8_t ID;

    /**
     * @brief   Message data header
     */
    header_t header;

    /**
     * @brief   Message data buffer
     */
    uint8_t data[8];

} pdu_t;

typedef struct {

    uint32_t identifier;
    uint32_t timestamp;

}timestamp_table_t;

/**
 * @brief   Table of CAN message IDs and associated unpacking functions
 */
static timestamp_table_t ts_table[] = {
    {CAN_DATABASE_M173_MODULATION_AND_FLUX_INFO_FRAME_ID, 0},
    {CAN_DATABASE_M172_TORQUE_AND_TIMER_INFO_FRAME_ID, 0},
    {CAN_DATABASE_M194_READ_WRITE_PARAM_RESPONSE_FRAME_ID, 0},
    {CAN_DATABASE_M193_READ_WRITE_PARAM_COMMAND_FRAME_ID, 0},
    {CAN_DATABASE_M192_COMMAND_MESSAGE_FRAME_ID, 0},
    {CAN_DATABASE_M171_FAULT_CODES_FRAME_ID, 0},
    {CAN_DATABASE_M170_INTERNAL_STATES_FRAME_ID, 0},
    {CAN_DATABASE_M169_INTERNAL_VOLTAGES_FRAME_ID, 0},
    {CAN_DATABASE_M168_FLUX_ID_IQ_INFO_FRAME_ID, 0},
    {CAN_DATABASE_M167_VOLTAGE_INFO_FRAME_ID,0},
    {CAN_DATABASE_M166_CURRENT_INFO_FRAME_ID, 0},
    {CAN_DATABASE_M165_MOTOR_POSITION_INFO_FRAME_ID, 0},
    {CAN_DATABASE_M164_DIGITAL_INPUT_STATUS_FRAME_ID, 0},
    {CAN_DATABASE_M163_ANALOG_INPUT_VOLTAGES_FRAME_ID, 0},
    {CAN_DATABASE_M162_TEMPERATURE_SET_3_FRAME_ID, 0},
    {CAN_DATABASE_M161_TEMPERATURE_SET_2_FRAME_ID, 0},
    {CAN_DATABASE_M160_TEMPERATURE_SET_1_FRAME_ID, 0},
    {CAN_DATABASE_M174_FIRMWARE_INFO_FRAME_ID, 0},
    {CAN_DATABASE_M175_DIAG_DATA_FRAME_ID, 0},
    {CAN_DATABASE_BMS_CURRENT_LIMIT_FRAME_ID,  0},
    {CAN_DATABASE_M176_FAST_INFO_FRAME_ID,0}
};