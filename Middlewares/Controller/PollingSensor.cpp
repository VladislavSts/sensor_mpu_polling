/*
 * PollingSensor.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "Mpu6050.h"

// Создание экземпляра MPU6050_t с помощью данных из ConfigI2C_t


VOID PollingSensorThread(ULONG thread_input)
{
	while(1)
	{
		sleep(_sec(2));
	}
}


