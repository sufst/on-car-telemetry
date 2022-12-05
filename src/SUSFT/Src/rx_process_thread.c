#include "gpio.h"
#include "tx_api.h"
#include "telemetry_protocol.h"

/**
 * @brief Rx_Process thread instance
 */
static TX_THREAD Rx_Process_thread;

#define RX_PROCESS_THREAD_PRIORITY             10
#define RX_PROCESS_THREAD_STACK_SIZE           512
#define RX_PROCESS_THREAD_PREEMPTION_THRESHOLD 10
#define RX_PROCESS_THREAD_NAME                 "Rx_Process Thread"

/*
 * function prototypes
 */
static void Rx_Process_thread_entry(can_mess message_in);

/**
 * @brief 		Creates the Rx_Process thread
 *
 * @param[in]	stack_pool_ptr	Pointer to start of application stack area
 *
 * @return		See ThreadX return codes
 */
UINT Rx_Process_thread_create(TX_BYTE_POOL* stack_pool_ptr)
{
    VOID* thread_stack_ptr;

    UINT ret = tx_byte_allocate(stack_pool_ptr,
                                &thread_stack_ptr,
                                Rx_Process_THREAD_STACK_SIZE,
                                TX_NO_WAIT);

    if (ret == TX_SUCCESS)
    {
        ret = tx_thread_create(&Rx_Process_thread,
                               RX_PROCESS_THREAD_NAME,
                               RX_PROCESS_thread_entry,
                               0,
                               thread_stack_ptr,
                               RX_PROCESS_THREAD_STACK_SIZE,
                               RX_PROCESS_THREAD_PRIORITY,
                               RX_PROCESS_THREAD_PREEMPTION_THRESHOLD,
                               TX_NO_TIME_SLICE,
                               TX_AUTO_START);
    }

    return ret;
}


/**
 * @brief       Rx_Process thread entry function
 *
 * @details     This unpacks CAN message received from CAN LIB's broadcaster
 *              Then, data is converted into desired format for telemetry.
 *              Ouput message of this thread is an input for tx_spi_xbee thread.
 *
 * @param[in]	message_in	CAN message
 */
void Rx_Process_thread_entry(can_mess message_in)
{

    /*
    Define telemetry_mess.
    can_mess to be provided and defined in CAN LIB
    telemetry_mess based on telemetry server requirements and SPI->XBEE communication.
    */
   
telemetry_mess message_out;

  /* Infinite loop */
  while(1)
  {	
/*
    if(CAN_OK()){

    
    //Fetch message from CAN LIB
    

        if(Fetch_success){
        
        
        //Convert format into telemetry_mess
        

        }
    }
*/

  tx_thread_sleep(100);
  }

}