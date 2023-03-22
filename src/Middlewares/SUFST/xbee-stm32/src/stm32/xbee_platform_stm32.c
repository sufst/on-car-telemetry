/**
    @addtogroup hal_kl25
    @{
    @file
    Platform support for Freescale KL25 using mbed compiler.
*/

#include "xbee_platform.h"

/* Inlude ThreadX API Header */
#include <tx_api.h>

uint32_t xbee_seconds_timer( void)
{
    return tx_time_get()/TX_TIMER_TICKS_PER_SECOND;
}

uint32_t xbee_millisecond_timer( void)
{
    return tx_time_get()/(TX_TIMER_TICKS_PER_SECOND/1000);
}