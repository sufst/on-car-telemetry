#include "gpio.h"
#include "tx_api.h"
/**
 * @brief Blink thread instance
 */
static TX_THREAD blink_thread;

#define BLINK_THREAD_PRIORITY             10
#define BLINK_THREAD_STACK_SIZE           512
#define BLINK_THREAD_PREEMPTION_THRESHOLD 10
#define BLINK_THREAD_NAME                 "Blink Thread"

/*
 * function prototypes
 */
static void blink_thread_entry(ULONG thread_input);

/**
 * @brief 		Creates the blink thread
 *
 * @param[in]	stack_pool_ptr	Pointer to start of application stack area
 *
 * @return		See ThreadX return codes
 */
UINT blink_thread_create(TX_BYTE_POOL* stack_pool_ptr)
{
    VOID* thread_stack_ptr;

    UINT ret = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                BLINK_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    if (ret == TX_SUCCESS)
    {
        ret = tx_thread_create(&blink_thread,
                               BLINK_THREAD_NAME,
                               blink_thread_entry,
                               0,
                               thread_stack_ptr,
                               BLINK_THREAD_STACK_SIZE,
                               BLINK_THREAD_PRIORITY,
                               BLINK_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    return ret;
}


/**
 * @brief       Blink thread entry function
 *
 * @details     This begins running as soon as the RTOS kernel is entered.
 *				Function testing RTOS by blinking LED.
 *
 * @param[in]	thread_input	Unused input
 */
void blink_thread_entry(ULONG thread_input)
{

(void) thread_input;

  /* Infinite loop */
  while(1)
  {	
	HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
    /* Delay for 1000ms */
  tx_thread_sleep(100);
  }

}