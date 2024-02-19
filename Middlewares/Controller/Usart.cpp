/*
 * Usart.cpp
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#include "main.h"
#include "BspUart.h"
#include "SetupController.h"
#include "Buffer.h"
#include "Mpu6050.h"
#include <string.h>

extern Uart_c Usart2;
extern LineBuffer_c<char, 256> RxBufferUart2;
extern LineBuffer_c<char, 256> TxBufferUart2;
extern MPU6050Sensor Mpu;
extern DataMpu MpuData;

#ifdef MPU6050
#define CheckMpuState Mpu.State == State_e::INIT
#else
#define CheckMpuState 1
#endif

VOID UsartThread(ULONG thread_input)
{
	Usart2.Init();
	RxBufferUart2.Clear();

	UINT Result;
	Command_e Command;
	Result_e CommandRx = Result_e::ERROR;

	while (1)
	{
		Result = tx_queue_receive(&TxUsartQueue, &Command, TX_WAIT_FOREVER);

		if (Result == TX_SUCCESS)
		{
			/* Обработка полученной команды */
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

				case Command_e::FULL_DATA_RECEIVED:
					/* Приняты все данные - разбираем команду */
					CommandRx = RxBufferUart2.FindString("start");
					if (CommandRx == Result_e::OK) {
					    SendCommand(TxPollingSensorQueue, Command_e::COMMAND_START);
					}

					CommandRx = RxBufferUart2.FindString("getdata");
					if (CommandRx == Result_e::OK && CheckMpuState) {
					    SendCommand(TxPollingSensorQueue, Command_e::START_POLLING_SENSOR);
					}

					CommandRx = RxBufferUart2.FindString("stopdata");
					if (CommandRx == Result_e::OK && CheckMpuState) {
						SendCommand(TxPollingSensorQueue, Command_e::STOP_POLLING_SENSOR);
					}
					break;

				default:
					;

			}
		}
	}
}




