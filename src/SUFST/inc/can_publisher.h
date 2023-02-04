#include <stdint.h>
#include <tx_api.h>
#define QUEUE_SIZE 100

TX_QUEUE queue;
static ULONG queue_memory_area[QUEUE_SIZE * sizeof(rtcan_msg_t)];

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