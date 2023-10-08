/*
 * UsartTransmit.cpp
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#include "main.h"
#include "SetupController.h"

VOID UsartTransmitThread(ULONG thread_input)
{

	uint8_t Str[] = "Hello, world!\r\n";

    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_7, (uint32_t)Str, LL_USART_DMA_GetRegAddr(USART2),
    		LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_7));
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, strlen((const char*)Str));

	while (1)
	{
	    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
		sleep(_sec(1));
	}
}




