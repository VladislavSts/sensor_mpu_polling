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

extern LineBuffer_c<char, 512> TxBufferUart2;
DataMpu MpuData = {0};

VOID UsartTransmitThread(ULONG thread_input)
{
    TxBufferUart2.Clear();

	while (1)
	{
		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::SENSOR_IS_READY,
				TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			char Ready[] = "stm32ready\r\n";
			sprintf((char*)TxBufferUart2.GetAddressBuffer(), (const char*)Ready);
			LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, sizeof(Ready));
			LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

		}

		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::TRANSMIT_DATA_SENSOR,
				TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			int NbData = sprintf((char*)TxBufferUart2.GetAddressBuffer(),
					"Accel_x:%.4f Accel_y:%.4f Accel_z:%.4f Gyro_x:%.4f Gyro_y:%.4f Gyro_z:%.4f Temperature:%.4f \r\n",
					MpuData.Ax,MpuData.Ay, MpuData.Az, MpuData.Gx, MpuData.Gy, MpuData.Gz, MpuData.Temp);
			LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, NbData);
			LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
		}
		sleep(_ms(5));
	}
}




