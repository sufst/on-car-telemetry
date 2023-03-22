/**
    @defgroup STM32F4 HAL: STMF4x with ThreadX

    @ingroup hal
    @{
    @file ports/stm32/platform_config.h

    This file is automatically included by xbee/platform.h.
*/
#ifndef __XBEE_PLATFORM_STM32
#define __XBEE_PLATFORM_STM32
      
    #define strcmpi         strcasecmp
    #define strncmpi        strncasecmp
    
    #define _f_memcpy        memcpy
    #define _f_memset        memset

    // stdint.h for int8_t, uint8_t, int16_t, etc. types
    #include <stdint.h>
    /* Include STM32 HAL Lib */
    #include "main.h"

    typedef int bool_t;

    // the "FAR" modifier is not used
    #define FAR
    #define BYTE_ORDER      BIG_ENDIAN

    typedef struct xbee_serial_t
    {
        uint32_t                    baudrate;
        UART_HandleTypeDef         *huart;
    } xbee_serial_t;

    // We'll use 1/1/2000 as the epoch, to match ZigBee.
    #define ZCL_TIME_EPOCH_DELTA    0

    #define XBEE_MS_TIMER_RESOLUTION 1

#endif //__XBEE_PLATFORM_STM32