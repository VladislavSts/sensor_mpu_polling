/*
 * BspUartIt.cpp
 *
 *  Created on: Oct 7, 2023
 *      Author: Vladislav
 */

#include "main.h"
#include "SetupController.h"
#include "Buffer.h"

#include "stm32f1xx_it.h" // TODO для прерываний

extern TX_EVENT_FLAGS_GROUP MyEventGroup;
extern LineBuffer_c<char, 256> RxBufferUart2;

extern TX_QUEUE TxUsartReceiveQueue;

void USART2_IRQHandler(void)
{
	/* обработка прерывания по таймауту на линии */
	if(LL_USART_IsActiveFlag_IDLE(USART2))
	{
	    LL_USART_ClearFlag_IDLE(USART2);

	    Command_e Command = Command_e::FULL_DATA_RECEIVED;
	    tx_queue_send(&TxUsartReceiveQueue, &Command, TX_NO_WAIT);
	    RxBufferUart2.WriteIndex = RxBufferUart2.GetVolume() - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);

	    /* Буфер приема пишет всегда с начала, т.к. с адреса Buffer (GetAddressBuffer()) */
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, (uint32_t)RxBufferUart2.GetVolume());
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6);
	}
}


void DMA1_Channel6_IRQHandler(void)
{
	/* USART2 прием данных */

	if(LL_DMA_IsActiveFlag_HT6(DMA1))
	{// прерывание по приему половины буфера
		LL_DMA_ClearFlag_HT6(DMA1);
		RxBufferUart2.WriteIndex = RxBufferUart2.GetVolume() - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);
	}
	else if(LL_DMA_IsActiveFlag_TC6(DMA1))
	{// прерывание по приему всего буфера
		LL_DMA_ClearFlag_TC6(DMA1);
		RxBufferUart2.WriteIndex = RxBufferUart2.GetVolume() - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);
	}
	else if(LL_DMA_IsActiveFlag_TE6(DMA1))
	{// прерывание по ошибке приема
		LL_DMA_ClearFlag_TE6(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);
	}
}


void DMA1_Channel7_IRQHandler(void)
{
	/* USART2 передача данных */

	if(LL_DMA_IsActiveFlag_TC7(DMA1))
	{// ДМА передача в УАРТ завершена
		LL_DMA_ClearFlag_TC7(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7); // TODO Off channel 7 (Tx)
	}
	else if(LL_DMA_IsActiveFlag_TE7(DMA1))
	{// ошибка ДМА передачи в УАРТ
		LL_DMA_ClearFlag_TE7(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
	}
}


