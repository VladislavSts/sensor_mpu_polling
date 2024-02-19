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
#include "Adxl345.h"

extern LineBuffer_c<char, 256> TxBufferUart2;
extern DataMpu MpuData;
extern DataAdxl AdxlData;

VOID UsartTransmitThread(ULONG thread_input)
{
    TxBufferUart2.Clear();
    UINT Result;
    Command_e Command;

	while (1)
	{
		Result = tx_queue_receive(&TxUsartTransmitQueue, &Command, TX_WAIT_FOREVER);

		if (Result == TX_SUCCESS)
		{
			switch(Command)
			{
				case Command_e::SENSOR_IS_READY: {
					char Ready[] = "stm32ready\r\n";
					sprintf((char*)TxBufferUart2.GetAddressBuffer(), (const char*)Ready);
					LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, sizeof(Ready));
					LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
					break;
				}

				case Command_e::TRANSMIT_DATA_SENSOR: {
#ifdef MPU6050
					int NbData = sprintf((char*)TxBufferUart2.GetAddressBuffer(),
							"Accel_x:%.4f Accel_y:%.4f Accel_z:%.4f Gyro_x:%.4f Gyro_y:%.4f Gyro_z:%.4f Temperature:%.4f \r\n",
							MpuData.Ax,MpuData.Ay, MpuData.Az, MpuData.Gx, MpuData.Gy, MpuData.Gz, MpuData.Temp);
#endif
#ifdef ADXL345
					int NbData = sprintf((char*)TxBufferUart2.GetAddressBuffer(),
							"Accel_x:%.4f Accel_y:%.4f Accel_z:%.4f\r\n",
							AdxlData.xg, AdxlData.yg, AdxlData.zg);
#endif
					LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, NbData);
					LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
					break;
				}

				default:
					;

			}
		}
	}
}




