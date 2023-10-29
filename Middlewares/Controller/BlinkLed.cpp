/*
 * Logic.cpp
 *
 *  Created on: Oct 6, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "Bsp.h"

VOID BlinkLedThread(ULONG thread_input)
{
	GpioInit(Gpio_e::LED);

	while (1)
	{
		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::START_POLLING_SENSOR,
		TX_OR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			GpioOn(Gpio_e::LED);  sleep(_ms(50));
			GpioOff(Gpio_e::LED); sleep(_ms(50));
			GpioOn(Gpio_e::LED);  sleep(_ms(50));
			GpioOff(Gpio_e::LED); sleep(_ms(50));
			GpioOn(Gpio_e::LED);  sleep(_ms(50));
			GpioOff(Gpio_e::LED); sleep(_ms(50));
		}
		sleep(_ms(5));
	}
}

