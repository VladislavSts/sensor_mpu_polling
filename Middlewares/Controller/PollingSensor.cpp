/*
 * PollingSensor.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "../Modules/Mpu6050.h"

VOID PollingSensorThread(ULONG thread_input)
{
	while(1)
	{
		sleep(_sec(2));
	}
}


