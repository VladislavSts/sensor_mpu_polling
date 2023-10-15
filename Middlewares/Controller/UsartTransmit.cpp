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
#include "Mpu6050.h"

extern LineBuffer_c<char, 256> TxBufferUart2;
extern MPU6050_t DataMpu;

VOID UsartTransmitThread(ULONG thread_input)
{
    int Test = 0;
    int Offset = 0;

    TxBufferUart2.Clear();

	while (1)
	{
		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::SENSOR_IS_READY,
				TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			char Ready[] = "stm32ready";
			sprintf((char*)TxBufferUart2.GetAddressBuffer(), (const char*)Ready);
			LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, sizeof(Ready));
			LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

		}

		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::TRANSMIT_DATA_SENSOR,
				TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			// Запись значения Temperature в буфер
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"Temperature: %.2f *C\r\n", DataMpu.Temperature);
			// Запись значения Test в буфер
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"TestUsart2Transmit: %d\r\n", Test);

			LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, Offset);
			LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

			Test++;
			Offset = 0;
		}

		sleep(_ms(5));
	}
}




