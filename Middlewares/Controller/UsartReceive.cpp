/*
 * UsartReceive.cpp
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#include "main.h"
#include "SetupController.h"

extern TX_EVENT_FLAGS_GROUP FullDataReceived;
extern ULONG actual_events;

VOID UsartReceiveThread(ULONG thread_input)
{
	UartInit(Uart_e::UART_2);

	while (1)
	{
		if (tx_event_flags_get(&FullDataReceived, (ULONG)Flags_e::FULL_DATA_RECEIVED,
				TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			// Здесь уже можно работать с принятыми данными
			// Флаг StateIdle устанавливается в прерывании

			/* сброс группы флагов событий */
			tx_event_flags_set(&FullDataReceived, 0, TX_AND);
		}
		sleep(_ms(5));
	}
}




