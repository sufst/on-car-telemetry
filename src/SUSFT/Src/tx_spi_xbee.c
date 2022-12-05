#include "spi.h"
#include "tx_api.h"

/**
 * @brief Tx_Spi thread instance
 */
static TX_THREAD Tx_Spi_thread;

#define TX_SPI_THREAD_PRIORITY             10
#define TX_SPI_THREAD_STACK_SIZE           512
#define TX_SPI_THREAD_PREEMPTION_THRESHOLD 10
#define TX_SPI_THREAD_NAME                 "Tx_Spi Thread"

/*
 * function prototypes
 */
static void Tx_Spi_thread_entry(telemetry_mess message_in);

/**
 * @brief 		Creates the Tx_Spi thread
 *
 * @param[in]	stack_pool_ptr	Pointer to start of application stack area
 *
 * @return		See ThreadX return codes
 */
UINT Tx_Spi_thread_create(TX_BYTE_POOL* stack_pool_ptr)
{
    VOID* thread_stack_ptr;

    UINT ret = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                TX_SPI_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    if (ret == TX_SUCCESS)
    {
        ret = tx_thread_create(&Tx_Spi_thread,
                               TX_SPI_THREAD_NAME,
                               Tx_Spi_thread_entry,
                               0,
                               thread_stack_ptr,
                               TX_SPI_THREAD_STACK_SIZE,
                               TX_SPI_THREAD_PRIORITY,
                               TX_SPI_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    return ret;
}


/**
 * @brief       Tx_Spi thread entry function
 *
 * @details     This thread get an input from rx_process_thread
 *              and sends data to XBee using SPI HAL.
 *
 * @param[in]	message_in	Tx_message format processed in rx_process_thread
 */
void Tx_Spi_thread_entry(telemetry_mess message_in)
{

    /*
    Define telemetry_mess.
    telemetry_mess defined in rx_process_thread 
    and based on telemetry server requirements and SPI->XBEE communication.
    */

  /* Infinite loop */
  while(1)
  {	
/*
    if(TX_MESS()){

    
    //Fetch message from rx_process and convert to SPI message ?.
    

        if(Fetch_success){
        
        HAL_SPI_Send(message_in);
        //Send data using SPI HAL
        

        }
    }
*/

  tx_thread_sleep(100);
  }

}