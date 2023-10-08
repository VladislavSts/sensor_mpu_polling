/*
 * Logic.cpp
 *
 *  Created on: Oct 6, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "tx_api.h"
#include "Bsp.h"

VOID BlinkLedThread(ULONG thread_input)
{
	GpioInit(Gpio_e::LED);

	while (1)
	{
		GpioOn(Gpio_e::LED);  sleep(_ms(50));
		GpioOff(Gpio_e::LED); sleep(_ms(50));
		GpioOn(Gpio_e::LED);  sleep(_ms(50));
		GpioOff(Gpio_e::LED); sleep(_ms(50));
		GpioOn(Gpio_e::LED);  sleep(_ms(50));
		GpioOff(Gpio_e::LED); sleep(_ms(50));
	}
}

