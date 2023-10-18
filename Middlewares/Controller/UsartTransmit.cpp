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
DataMpu MpuData = {0};

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
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"Accel_x: %.4f \r\n", MpuData.Ax);
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"Accel_y: %.4f \r\n", MpuData.Ay);
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"Accel_z: %.4f \r\n", MpuData.Az);
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"Gyro_x: %.4f \r\n", MpuData.Gx);
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"Gyro_y: %.4f \r\n", MpuData.Gy);
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"Gyro_z: %.4f \r\n", MpuData.Gz);
			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
					"Temperature: %.4f \r\n", MpuData.Temp);
//			Offset += snprintf((char*)TxBufferUart2.GetAddressBuffer() + Offset, TxBufferUart2.GetVolume() - Offset,
//					"==================== \r\n");

			LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, Offset);
			LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

			Test++;
			Offset = 0;
		}

		sleep(_ms(5));
	}
}




