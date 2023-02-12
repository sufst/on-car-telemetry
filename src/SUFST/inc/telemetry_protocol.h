#ifndef CAN_TELEMETRY_H
#define CAN_TELEMETRY_H

#include <stdint.h>
#include <can_database.h>

#define TABLE_SIZE 23
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

/**
 * @brief   Table of CAN message IDs and associated unpacking functions
 */
static uint32_t ts_table[TABLE_SIZE] = {0};

#endif /* CAN_TELEMETRY_H */