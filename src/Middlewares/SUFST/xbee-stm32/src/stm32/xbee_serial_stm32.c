/*
 * Copyright (c) 2010-2012 Digi International Inc.,
 * All rights not expressly granted are reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 * =======================================================================
 */
/**
    @addtogroup hal_kl25
    @{
    @file
    Serial Interface for Freescale KL25 using mbed compiler.
*/

// NOTE: Documentation for these functions can be found in xbee/serial.h.

#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include "xbee_platform.h"
#include "xbee_serial.h"
#include "xbee_cbuf.h"
#include "usart.h"

bool_t xbee_ser_invalid( xbee_serial_t *serial)
{
    return 0;
}


const char *xbee_ser_portname( xbee_serial_t *serial)
{
    return "UART4";
}


int xbee_ser_write( xbee_serial_t *serial, const void FAR *buffer,
    int length)
{
HAL_StatusTypeDef ret;

ret = HAL_UART_Transmit(&huart4, buffer, length, 200); // @todo Create xbee_serial_t with huart handler defined in user app.
if(ret != HAL_OK)
{
    return -EIO;
}
else
{
    return length;
}

}

int xbee_ser_read( xbee_serial_t *serial, void FAR *buffer, int bufsize)
{
    HAL_StatusTypeDef ret;

	if (! buffer || bufsize < 0)
	{
		return -EINVAL;
	}
    
    ret = HAL_UART_Receive(&huart4, buffer, bufsize, 100*bufsize); // @todo Create xbee_serial_t with huart handler defined in user app.
    if(ret != HAL_OK)
    {
        return -EIO;
    }
    else
    {
        return bufsize;
    }
}


int xbee_ser_putchar( xbee_serial_t *serial, uint8_t ch)
{
    HAL_StatusTypeDef ret;
    ret = HAL_UART_Transmit(&huart4, &ch, (uint8_t)1, 10); // @todo Create xbee_serial_t with huart handler defined in user app.
    if(ret != HAL_OK)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


int xbee_ser_getchar( xbee_serial_t *serial)
{
    HAL_StatusTypeDef ret;
    uint8_t ch;
    ret = HAL_UART_Receive(&huart4, &ch, 1, 10); // @todo Create xbee_serial_t with huart handler defined in user app.
    if(ret != HAL_OK)
    {
        return 0;
    }
    else
    {
        return ch;
    }
}


// Since we're using blocking transmit, there isn't a transmit buffer.
// Therefore, have xbee_ser_tx_free() and xbee_ser_tx_used() imply that
// we have an empty buffer that can hold an unlimited amount of data.
#define MAX_INT 127

int xbee_ser_tx_free( xbee_serial_t *serial)
{
    return MAX_INT;
}
int xbee_ser_tx_used( xbee_serial_t *serial)
{
    return 0;
}


int xbee_ser_tx_flush( xbee_serial_t *serial)
{
    return 0;
}


int xbee_ser_rx_free( xbee_serial_t *serial)
{
    return MAX_INT;
}


int xbee_ser_rx_used( xbee_serial_t *serial)
{
    return 0;
}


int xbee_ser_rx_flush( xbee_serial_t *serial)
{
    return 0;
}


int xbee_ser_baudrate( xbee_serial_t *serial, uint32_t baudrate)
{
    return 0;
}
/*
void xbee_ser_rx_isr( void)
{

}
*/
int xbee_ser_open( xbee_serial_t *serial, uint32_t baudrate)
{
    return 0;
}


int xbee_ser_close( xbee_serial_t *serial)
{
    return 0;
}


int xbee_ser_break( xbee_serial_t *serial, bool_t enabled)
{
    return 0;
}


int xbee_ser_flowcontrol( xbee_serial_t *serial, bool_t enabled)
{
    return 0;
}

int xbee_ser_set_rts( xbee_serial_t *serial, bool_t asserted)
{
    return 0;
}


int xbee_ser_get_cts( xbee_serial_t *serial)
{
    /* Always clear to send ( don't use CTS pin )*/
    return 1;
}

