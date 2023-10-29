/*
 * PollingSensor.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "Mpu6050.h"
#include "BspI2c.h"

DataMpu MpuData = {0};
extern I2c_c I2c1;
MPU6050Sensor Mpu(I2c1, MpuData);

VOID PollingSensorThread(ULONG thread_input)
{
	UINT Result;
	ULONG TimeWait = TX_WAIT_FOREVER;
	Command_e Command;
	bool StartPoling = false;

	while(1)
	{
		Result = tx_queue_receive(&TxPollingSensorQueue, &Command, TimeWait);

		if (Result == TX_SUCCESS)
		{
			switch(Command)
			{
				case Command_e::COMMAND_START:
					Mpu.MPU6050_Init();
					if (Mpu.State != State_e::INIT) {
						// ошибка инициализации сенсора
					}
					else {
						// сенсор успешно инициализирован
						SendCommand(TxUsartTransmitQueue, Command_e::SENSOR_IS_READY);
					}
					break;

				case Command_e::START_POLLING_SENSOR:
					TimeWait = 20;
					StartPoling = true;
					SendCommand(TxBllinkLedQueue, Command_e::START_POLLING_SENSOR);
					break;

				case Command_e::STOP_POLLING_SENSOR:
					TimeWait = TX_WAIT_FOREVER;
					StartPoling = false;
					SendCommand(TxBllinkLedQueue, Command_e::STOP_POLLING_SENSOR);
					Mpu.MPU6050_DeInit();
					break;

				default:
					;
			}
		}
		else {
			/* Не получили сообщение из очереди, опрашиваем датчик */
			if (StartPoling) {
				Error_e Result;
				Result = Mpu.ReadAllData();
				if (Result == Error_e::OK)
					SendCommand(TxUsartTransmitQueue, Command_e::TRANSMIT_DATA_SENSOR);
			}
		}
	}
}


