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

VOID PollingSensorThread(ULONG thread_input)
{
	while(1)
	{
		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::COMMAND_START,
		TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			if (Mpu.MPU6050_Init()) {
				// ошибка инициализации сенсора
			}
			else {
				// сенсор успешно инициализирован
				tx_event_flags_set(&MyEventGroup, (ULONG)Flags_e::SENSOR_IS_READY, TX_OR);
			}
		}

		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::START_POLLING_SENSOR,
		TX_OR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			Mpu.MPU6050_Read_Accel();
			Mpu.MPU6050_Read_Gyro();
			Mpu.MPU6050_Read_Temp();
			tx_event_flags_set(&MyEventGroup, (ULONG)Flags_e::TRANSMIT_DATA_SENSOR, TX_OR);
		}

		sleep(_ms(20));
	}
}


