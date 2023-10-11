/*
 * UsartTransmit.cpp
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#include "main.h"
#include "SetupController.h"
#include "Buffer.h"
#include <stdio.h>
#include <string.h>

extern LineBuffer_c<char, 256> TxBufferUart2;

VOID UsartTransmitThread(ULONG thread_input)
{
    float Temperature = 26.45;
    int Test = 0;
    int Offset = 0;

    TxBufferUart2.Clear();

	while (1)
	{
		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::SENSOR_IS_READY,
				TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			sprintf((char*)TxBufferUart2.GetAddressBuffer(), "stm32ready");
			LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
		}

		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::TRANSMIT_DATA_SENSOR,
				TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			// Запись значения Temperature в буфер
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"Temperature: %.2f *C \r\n", Temperature);
			// Запись значения Test в буфер
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"TestUsart2Transmit: %d \r\n", Test);

			LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

			Test++;
			Offset = 0;
		}

		sleep(_ms(5));
	}
}



