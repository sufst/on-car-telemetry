#include <tx_api.h>
#include "watchdog.h"

#define WATCHDOG_THREAD_PRIORITY             10
#define WATCHDOG_THREAD_STACK_SIZE           1024
#define WATCHDOG_THREAD_PREEMPTION_THRESHOLD 10                
#define WATCHDOG_THREAD_WAKE_TIMEOUT         10

static void watchdog_thread_entry(ULONG input);

/**
 * @brief 		Creates the watchdog thread
 *
 * @param[in]	stack_pool_ptr	Pointer to start of application stack area
 *
 * @return		See ThreadX return codes
 */
UINT watchdog_init(watchdog_context_t* watchdog_ptr, TX_BYTE_POOL* stack_pool_ptr)
{

    VOID* thread_stack_ptr = NULL;

    UINT tx_status = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                WATCHDOG_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    if (tx_status == TX_SUCCESS)
    {
        tx_status = tx_thread_create(&watchdog_ptr->thread,
                               "Watchdog Thread",
                               watchdog_thread_entry,
                               watchdog_ptr,
                               thread_stack_ptr,
                               WATCHDOG_THREAD_STACK_SIZE,
                               WATCHDOG_THREAD_PRIORITY,
                               WATCHDOG_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    return tx_status;
}

static void watchdog_thread_entry(ULONG input)
{
    watchdog_context_t* watchdog_ptr = (watchdog_context_t*) input;

    /* Turn on LED by default */

    /* Suspend the thread on semaphore, when fault happen it will give back the semaphore */
    const UINT status = tx_semaphore_get(&watchdog_ptr->fault_semaphore, WATCHDOG_THREAD_WAKE_TIMEOUT);

    while(1)
    {


        switch (watchdog_ptr->error_code)
        {
        case 0:
            /* code */
            break;
        
        default:
            break;
        }

        // Introduce 500ms delay
        tx_thread_sleep(50);
    }

}