#include "usart.h"
#include "tx_api.h"
#include "stdint.h"
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

(void) thread_input;
uint8_t data = 'A';
  /* Infinite loop */
  while(1)
  {

  HAL_UART_Transmit(&huart4, &data, 1, 100);
  data++;
    /* Delay for 1000ms */
  tx_thread_sleep(100);
  }

}