/*
 * BspUartIt.cpp
 *
 *  Created on: Oct 7, 2023
 *      Author: Vladislav
 */

#include "main.h"

#include "SetupController.h"
#include "tx_api.h"

extern bool StateIdle;
extern TX_EVENT_FLAGS_GROUP FullDataReceived;
extern LineBuffer_c<uint8_t, 256> RxRingBuffer;

void USART2_IRQHandler(void)
{
	/* обработка прерывания по задержке в приеме данных размером в один символ */
	if(LL_USART_IsActiveFlag_IDLE(USART2))
	{
	    LL_USART_ClearFlag_IDLE(USART2);
	    tx_event_flags_set(&FullDataReceived, (ULONG)Flags_e::FULL_DATA_RECEIVED, TX_OR);
	    RxRingBuffer.WriteIndex = RxRingBuffer.GetVolume() - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);
	}
}




void DMA1_Channel6_IRQHandler(void)
{
	/* USART2 прием данных */

	if(LL_DMA_IsActiveFlag_HT6(DMA1))
	{// прерывание по приему половины буфера
		LL_DMA_ClearFlag_HT6(DMA1);
		RxRingBuffer.WriteIndex = RxRingBuffer.GetVolume() - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);
	}
	else if(LL_DMA_IsActiveFlag_TC6(DMA1))
	{// прерывание по приему всего буфера
		LL_DMA_ClearFlag_TC6(DMA1);
		RxRingBuffer.WriteIndex = RxRingBuffer.GetVolume() - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);
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


