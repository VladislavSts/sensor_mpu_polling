/*
 * UsartReceive.cpp
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#include "tx_api.h"
#include "main.h"
#include "BspUart.h"
#include "SetupController.h"

bool StateIdle = false;

VOID UsartReceiveThread(ULONG thread_input)
{
	UartInit(Uart_e::UART_2);
	uint32_t Value = 0;

	while (1)
	{
		if (StateIdle) {
			StateIdle = false;
			Value = LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);

			// Здесь уже можно работать с принятыми данными
			// Флаг StateIdle устанавливается в прерывании

		}
		sleep(_ms(5));
	}
}




