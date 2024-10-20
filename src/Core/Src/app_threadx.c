/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can_unpack.h"
#include "error_handler.h"
#include "xbee_comms.h"
#include "can.h"
#include "config.h"
#if CAN_DEBUG_MODE == 1
  #include "can_publisher.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static unpack_context_t unpack_context;
static error_handler_context_t error_handler_context;
static rtcan_handle_t rtcan;
static xbee_comms_context_t xbee_comms_context;
#if CAN_DEBUG_MODE == 1
  static publisher_context_t publisher_context;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
    (void)byte_pool;

    ret = error_handler_init(&error_handler_context, byte_pool);

    if(ret == TX_SUCCESS)
    {
        ret = unpack_init(&unpack_context, &error_handler_context, byte_pool, &rtcan);
    }

    /* if debug mode is on, start can_publisher thread here */
    #if CAN_DEBUG_MODE == 1
      if(ret == TX_SUCCESS)
      {
        ret = can_publisher_init(&publisher_context, can_unpack_get_rx_queue_ptr(&unpack_context), byte_pool);
      }
    #endif

    if(ret == TX_SUCCESS)
    {
        ret = xbee_comms_init(&xbee_comms_context, &error_handler_context, can_unpack_get_tx_queue_ptr(&unpack_context), byte_pool);
    }

  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* can_h)
{
    rtcan_handle_tx_mailbox_callback(&rtcan, can_h);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* can_h)
{
    rtcan_handle_tx_mailbox_callback(&rtcan, can_h);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* can_h)
{
    rtcan_handle_tx_mailbox_callback(&rtcan, can_h);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* can_h)
{
    rtcan_handle_rx_it(&rtcan, can_h, 0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* can_h)
{
    rtcan_handle_rx_it(&rtcan, can_h, 1);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* can_h)
{
    rtcan_handle_hal_error(&rtcan, can_h);
}
/* USER CODE END 1 */
