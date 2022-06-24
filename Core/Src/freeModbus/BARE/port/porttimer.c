/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "tim.h"
/* ----------------------- static functions ---------------------------------*/
void prvvTIMERExpiredISR( void );

extern TIM_HandleTypeDef htim4;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	HAL_TIM_Base_DeInit(&htim4);
	
	TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 4500 - 1; // 90MHz / 4500 = 20KHz, 50us·ÖÆµ
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;

  htim4.Init.Period = usTim1Timerout50us - 1;//modbus 3.5¸ö×Ö·û
	
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
     return FALSE;
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
     return FALSE;
  }
	 return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	__HAL_TIM_CLEAR_IT(&htim4,TIM_IT_UPDATE);
	__HAL_TIM_SetCounter(&htim4,0);
	HAL_TIM_Base_Start_IT(&htim4);
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
	HAL_TIM_Base_Stop_IT(&htim4);
	
	__HAL_TIM_SetCounter(&htim4,0);
	__HAL_TIM_CLEAR_IT(&htim4,TIM_IT_UPDATE);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

