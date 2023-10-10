/*
 * PollingSensor.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "Mpu6050.h"

// Создание экземпляра MPU6050_t с помощью данных из ConfigI2C_t
MPU6050_t DataMpu;
extern I2C_HandleTypeDef I2c1Hdl;
MPU6050Sensor Mpu(&I2c1Hdl, &DataMpu);

VOID PollingSensorThread(ULONG thread_input)
{
	Mpu.MPU6050_Init();

	while(1)
	{
		// Ожидать флаг готовности

		Mpu.MPU6050_Read_All();
		sleep(_sec(2));
	}
}


