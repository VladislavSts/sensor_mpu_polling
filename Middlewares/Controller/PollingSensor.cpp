/*
 * PollingSensor.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "Mpu6050.h"
#include "BspI2c.h"

#include "Adxl345.h"

extern I2c_c I2c1;

#ifdef ADXL345
DataAdxl AdxlData = {0};
Adxl345 Adxl(I2c1, AdxlData);
#endif

#ifdef MPU6050
DataMpu MpuData = {0};
MPU6050Sensor Mpu(I2c1, MpuData);
#endif

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
#ifdef MPU6050
					Mpu.MPU6050_Init();
					SendCommand(TxUsartTransmitQueue, Command_e::SENSOR_IS_READY);
#endif
#ifdef ADXL345
					Adxl.Init();
					SendCommand(TxUsartTransmitQueue, Command_e::SENSOR_IS_READY);
#endif
					break;

				case Command_e::START_POLLING_SENSOR:
					TimeWait = 15;
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
#ifdef MPU6050
				Error_e Result;
				Result = Mpu.ReadAllData();
				if (Result == Error_e::OK)
					SendCommand(TxUsartTransmitQueue, Command_e::TRANSMIT_DATA_SENSOR);
#endif
#ifdef ADXL345
				Adxl.ReadValues (0x32);
				SendCommand(TxUsartTransmitQueue, Command_e::TRANSMIT_DATA_SENSOR);
#endif
			}
		}
	}
}


