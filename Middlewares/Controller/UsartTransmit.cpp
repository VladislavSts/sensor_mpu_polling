/*
 * UsartTransmit.cpp
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#include "main.h"
#include "SetupController.h"
#include <stdio.h>
#include <string.h>

VOID UsartTransmitThread(ULONG thread_input)
{
	constexpr int SizeBuffer = 256;
	char TransmitBuffer[SizeBuffer] = {0};
	sprintf(TransmitBuffer, "Hello!\r\n");

    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_7, (uint32_t)TransmitBuffer, LL_USART_DMA_GetRegAddr(USART2),
    		LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_7));
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, SizeBuffer);

    float Temperature = 26.45;
    int Test = 0;

    int Offset = 0;

	while (1)
	{
	    // Запись значения Temperature в буфер
		Offset += snprintf(TransmitBuffer + Offset, sizeof(TransmitBuffer) - Offset, "Temperature: %.2f *C \r\n", Temperature);
	    // Запись значения Test в буфер
		Offset += snprintf(TransmitBuffer + Offset, sizeof(TransmitBuffer) - Offset, "TestUsart2Transmit: %d \r\n", Test);

	    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

	    Test++;
	    Offset = 0;

		sleep(_sec(1));
	}
}




