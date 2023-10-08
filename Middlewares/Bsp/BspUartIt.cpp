/*
 * BspUartIt.cpp
 *
 *  Created on: Oct 7, 2023
 *      Author: Vladislav
 */

#include "main.h"

#include "BspUart.h"

extern bool StateIdle;

void USART2_IRQHandler(void)
{
	/* обработка прерывания по задержке в приеме данных размером в один символ */
	if(LL_USART_IsActiveFlag_IDLE(USART2))
	{
	    LL_USART_ClearFlag_IDLE(USART2);
	    StateIdle = true;

	    uint32_t length = RX_BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);

	    if (length < RX_BUFFER_SIZE)
	      RxBuffer[length] = 0; //Добавляем '\0' в конец строки
	    else
	      //Overflow rx data

	    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);
	    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, RX_BUFFER_SIZE);
	    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6);
	}
}




void DMA1_Channel6_IRQHandler(void)
{
	/* USART2 прием данных */

	if(LL_DMA_IsActiveFlag_HT6(DMA1))
	{// прерывание по приему половины буфера
		LL_DMA_ClearFlag_HT6(DMA1);
	}
	else if(LL_DMA_IsActiveFlag_TC6(DMA1))
	{// прерывание по приему всего буфера
		LL_DMA_ClearFlag_TC6(DMA1);
	}
	else if(LL_DMA_IsActiveFlag_TE6(DMA1))
	{// прерывание по ошибке приема
		LL_DMA_ClearFlag_TE6(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
	}
}


void DMA1_Channel7_IRQHandler(void)
{
	/* USART2 передача данных */

	if(LL_DMA_IsActiveFlag_TC7(DMA1))
	{// ДМА передача в УАРТ завершена
		LL_DMA_ClearFlag_TC7(DMA1);
	}
	else if(LL_DMA_IsActiveFlag_TE7(DMA1))
	{// ошибка ДМА передачи в УАРТ
		LL_DMA_ClearFlag_TE7(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
	}
}


