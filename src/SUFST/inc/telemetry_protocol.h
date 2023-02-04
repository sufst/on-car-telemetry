#include <stdint.h>
#include <can_database.h>

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
    {CAN_DATABASE_PM100_MODULATION_AND_FLUX_INFO_FRAME_ID, 0},
    {CAN_DATABASE_PM100_TORQUE_AND_TIMER_INFO_FRAME_ID, 0},
    {CAN_DATABASE_PM100_READ_WRITE_PARAM_RESPONSE_FRAME_ID, 0},
    {CAN_DATABASE_PM100_READ_WRITE_PARAM_COMMAND_FRAME_ID, 0},
    {CAN_DATABASE_PM100_COMMAND_MESSAGE_FRAME_ID, 0},
    {CAN_DATABASE_PM100_FAULT_CODES_FRAME_ID, 0},
    {CAN_DATABASE_PM100_INTERNAL_STATES_FRAME_ID, 0},
    {CAN_DATABASE_PM100_INTERNAL_VOLTAGES_FRAME_ID, 0},
    {CAN_DATABASE_PM100_FLUX_ID_IQ_INFO_FRAME_ID, 0},
    {CAN_DATABASE_PM100_VOLTAGE_INFO_FRAME_ID, 0},
    {CAN_DATABASE_PM100_CURRENT_INFO_FRAME_ID, 0},
    {CAN_DATABASE_PM100_MOTOR_POSITION_INFO_FRAME_ID, 0},
    {CAN_DATABASE_PM100_DIGITAL_INPUT_STATUS_FRAME_ID, 0},
    {CAN_DATABASE_PM100_ANALOG_INPUT_VOLTAGES_FRAME_ID, 0},
    {CAN_DATABASE_PM100_TEMPERATURE_SET_3_FRAME_ID, 0},
    {CAN_DATABASE_PM100_TEMPERATURE_SET_2_FRAME_ID, 0},
    {CAN_DATABASE_PM100_TEMPERATURE_SET_1_FRAME_ID, 0},
    {CAN_DATABASE_PM100_FIRMWARE_INFO_FRAME_ID, 0},
    {CAN_DATABASE_PM100_DIAG_DATA_FRAME_ID, 0},
    {CAN_DATABASE_PM100_U2_C_COMMAND_TXD_FRAME_ID, 0},
    {CAN_DATABASE_PM100_U2_C_MESSAGE_RXD_FRAME_ID, 0},
    {CAN_DATABASE_BMS_CURRENT_LIMIT_FRAME_ID, 0},
    {CAN_DATABASE_PM100_FAST_INFO_FRAME_ID, 0}
};