/*
 * PollingSensor.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "Mpu6050.h"
#include "BspI2c.h"

extern DataMpu MpuData;
extern I2c_c I2c1;
MPU6050Sensor Mpu(I2c1, MpuData);

extern TX_QUEUE TxUsartTransmitQueue;
extern TX_QUEUE TxPollingSensorQueue;
extern TX_QUEUE TxBllinkLedQueue;

VOID PollingSensorThread(ULONG thread_input)
{
	UINT Result;
	Command_e Command;
	ULONG TimeWait = TX_WAIT_FOREVER;
	bool StartPoling = false;
	Command_e CommandTo;

	while(1)
	{
		Result = tx_queue_receive(&TxPollingSensorQueue, &Command, TimeWait);

		if (Result == TX_SUCCESS)
		{
			switch(Command)
			{
				case Command_e::COMMAND_START:
					if (Mpu.MPU6050_Init()) {
						// ошибка инициализации сенсора
					}
					else {
						// сенсор успешно инициализирован
						CommandTo = Command_e::SENSOR_IS_READY;
						tx_queue_send(&TxUsartTransmitQueue, &CommandTo, TX_NO_WAIT);
					}
					break;

				case Command_e::START_POLLING_SENSOR:
					TimeWait = 15;
					StartPoling = true;
					CommandTo = Command_e::START_POLLING_SENSOR;
					tx_queue_send(&TxBllinkLedQueue, &CommandTo, TX_NO_WAIT);
					break;

				case Command_e::STOP_POLLING_SENSOR:
					TimeWait = TX_WAIT_FOREVER;
					StartPoling = false;
					CommandTo = Command_e::STOP_POLLING_SENSOR;
					tx_queue_send(&TxBllinkLedQueue, &CommandTo, TX_NO_WAIT);
					Mpu.MPU6050_DeInit();
					break;

				default:
					;
			}
		}
		else {
			/* Не получили сообщение из очереди */
			if (StartPoling) {
				Mpu.MPU6050_Read_Accel();
				Mpu.MPU6050_Read_Gyro();
				Mpu.MPU6050_Read_Temp();

				CommandTo = Command_e::TRANSMIT_DATA_SENSOR;
				tx_queue_send(&TxUsartTransmitQueue, &CommandTo, TX_NO_WAIT);
			}
		}
	}
}


