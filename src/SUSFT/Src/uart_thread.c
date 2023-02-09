#include "usart.h"
#include "tx_api.h"
#include "stdint.h"
#include "xbee_platform.h"
#include <stdio.h>
#include "_atinter.h"
/**
 * @brief Blink thread instance
 */
static TX_THREAD uart_thread;

#define UART_THREAD_PRIORITY             10
#define UART_THREAD_STACK_SIZE           512
#define UART_THREAD_PREEMPTION_THRESHOLD 10
#define UART_THREAD_NAME                 "Blink Thread"

/*
 * function prototypes
 */
static void uart_thread_entry(ULONG thread_input);
xbee_dev_t my_xbee;
xbee_serial_t *serport;
char cmdstr[80];
/**
 * @brief 		Creates the uart thread
 *
 * @param[in]	stack_pool_ptr	Pointer to start of application stack area
 *
 * @return		See ThreadX return codes
 */
UINT uart_thread_create(TX_BYTE_POOL* stack_pool_ptr)
{
    VOID* thread_stack_ptr;

    UINT ret = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                UART_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    if (ret == TX_SUCCESS)
    {
        ret = tx_thread_create(&uart_thread,
                               UART_THREAD_NAME,
                               uart_thread_entry,
                               0,
                               thread_stack_ptr,
                               UART_THREAD_STACK_SIZE,
                               UART_THREAD_PRIORITY,
                               UART_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    return ret;
}


void setup_xbee(){
  int status;
    xbee_cmd_init_device( &my_xbee);
    printf( "Waiting for driver to query the XBee device...\n");
    do {
        xbee_dev_tick( &my_xbee);
        status = xbee_cmd_query_status( &my_xbee);
    } while (status == -EBUSY);
    if (status)
    {
        printf( "Error %d waiting for query to complete.\n", status);
    }

    // report on the settings
    xbee_dev_dump_settings( &my_xbee, XBEE_DEV_DUMP_FLAG_DEFAULT);

    printATCmds( &my_xbee);

}

void try_commands(){

        while (xbee_readline( cmdstr, sizeof cmdstr) == -EAGAIN)
        {
            xbee_dev_tick( &my_xbee);
        }

        if (! strncmpi( cmdstr, "menu", 4))
        {
            printATCmds( &my_xbee);
        }
        else
        {
            process_command( &my_xbee, cmdstr);
        }

}
/**
 * @brief       Blink thread entry function
 *
 * @details     This begins running as soon as the RTOS kernel is entered.
 *				Function testing RTOS by uarting LED.
 *
 * @param[in]	thread_input	Unused input
 */
void uart_thread_entry(ULONG thread_input)
{
  setup_xbee();
(void) thread_input;
uint8_t UART1_rxBuffer[12] = {0};
uint8_t UART1_OK[2] = "OK";
int a;
HAL_StatusTypeDef ret;
serport->huart = &huart4;
  /* Infinite loop */
  while(1)
  {
  //char ch = 'a';
  //ret = HAL_UART_Receive(&huart4, UART1_rxBuffer, 1, 0);
  //ch = xbee_ser_getchar(serport); 
  //xbee_ser_putchar(serport, ch);

  try_commands();
  //while (xbee_readline( cmdstr, sizeof cmdstr) == -EAGAIN){}
  
  //xbee_ser_write(&huart4,UART1_OK,2);

  //a = xbee_ser_read(serport, &UART1_rxBuffer, 5);
  
  //if(a == 5){
  //  xbee_ser_write(serport, &UART1_rxBuffer, 5);
  //  }

    /* Delay for 1000ms */
// tx_thread_sleep(100);
  }

}

#include "xbee_atcmd.h"         // for XBEE_FRAME_HANDLE_LOCAL_AT
#include "xbee_device.h"
const xbee_dispatch_table_entry_t xbee_frame_handlers[] =
{
    XBEE_FRAME_HANDLE_LOCAL_AT,
    XBEE_FRAME_MODEM_STATUS_DEBUG,
    XBEE_FRAME_TABLE_END
};