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
    Platform support for Freescale KL25 using mbed compiler.
*/

#include "xbee_platform.h"
#include <tx_api.h>
#include "usart.h"

xbee_serial_t Serial;

uint32_t xbee_seconds_timer( void)
{
return tx_time_get()/TX_TIMER_TICKS_PER_SECOND;
}

uint32_t xbee_millisecond_timer( void)
{
return tx_time_get()/(TX_TIMER_TICKS_PER_SECOND/1000);
}

#include <ctype.h>

#define XBEE_READLINE_STATE_INIT            0
#define XBEE_READLINE_STATE_START_LINE      1
#define XBEE_READLINE_STATE_CONTINUE_LINE   2
//#define FPUTCHAR_PROTOTYPE int fputc(int ch)
/*
FPUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
*/

int xbee_readline( char *buffer, int length)
{
    static int state = XBEE_READLINE_STATE_INIT;
    int c = 0;
    static char *cursor;
    HAL_StatusTypeDef ret;
    if (buffer == NULL || length < 1)
    {
        return -EINVAL;
    }

    switch (state)
    {
        default:
        case XBEE_READLINE_STATE_INIT:


        case XBEE_READLINE_STATE_START_LINE:            // start of new line
            cursor = buffer;
            *buffer = '\0'; // reset string
            state = XBEE_READLINE_STATE_CONTINUE_LINE;
            // fall through to continued input

        case XBEE_READLINE_STATE_CONTINUE_LINE:     // continued input
            __HAL_UART_CLEAR_OREFLAG(&huart2);
            //ret = getc();
            ret = HAL_UART_Receive(&huart2, &c, 1, HAL_MAX_DELAY);
//            fputc(ret);
            if(ret != HAL_OK){
                return -EAGAIN;
            }
            switch (c)
            {
                case 0x7F:              // backspace (Win32)
                case '\b':              // supposedly backspace...
                    if (buffer != cursor)
                    {
                        fputc('\b');
                        fputc(' ');
                        fputc('\b'); // back up, erase last character
                        cursor--;
                    }
                    break;

                case '\n':
                case '\r':
//                    fputc('\r'); //for WINDOWS Terminal
                    fputc('\n');
                    state = XBEE_READLINE_STATE_START_LINE;
                    return cursor - buffer;

                default:
                    if (isprint( c) && (cursor - buffer < length - 1))
                    {
                        *cursor++= c;
                        fputc(c);
                    }
               else
               {
                  fputc( '\x07');     // error beep -- bad char
               }
                    break;
            }
            *cursor = 0;        // keep string null-terminated
    }

    return -EAGAIN;
}

int xbee_platform_init( void)
{
    static bool_t init = TRUE;
    if (init)
    {
        init = FALSE;
        Serial.huart = &huart4;
    }
    return 0;
}

uint16_t _xbee_get_unaligned16( const void FAR *p)
{
    uint16_t retval;
    _f_memcpy( &retval, p, 2);

    return retval;
}

uint32_t _xbee_get_unaligned32( const void FAR *p)
{
    uint32_t retval;
    _f_memcpy( &retval, p, 4);

    return retval;
}

void _xbee_set_unaligned16( void FAR *p, uint16_t value)
{
    _f_memcpy( p, &value, 2);
}

void _xbee_set_unaligned32( void FAR *p, uint32_t value)
{
    _f_memcpy( p, &value, 4);
}