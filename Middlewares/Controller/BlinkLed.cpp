/*
 * BlinkLed.cpp
 *
 *  Created on: Oct 6, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "Bsp.h"

VOID BlinkLedThread(ULONG thread_input)
{
	GpioInit(Gpio_e::LED);
	UINT Result;
	Command_e Command;
	ULONG TimeWait = TX_WAIT_FOREVER;
	bool FlagBlink = false;

	while (1)
	{
		Result = tx_queue_receive(&TxBllinkLedQueue, &Command, TimeWait);

		if (Result == TX_SUCCESS) {
			switch(Command)
			{
			case Command_e::START_POLLING_SENSOR:
				FlagBlink = true;
				TimeWait = 20;
				break;

			case Command_e::STOP_POLLING_SENSOR:
				FlagBlink = false;
				break;

			default:
				;
			}
		}
		else {
			if (FlagBlink) {
				GpioOn(Gpio_e::LED);  sleep(_ms(50));
				GpioOff(Gpio_e::LED); sleep(_ms(50));
			}
			else {
				GpioOff(Gpio_e::LED);
			}
		}
	}
}

