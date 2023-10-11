/*
 * UsartReceive.cpp
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#include "main.h"
#include "BspUart.h"
#include "SetupController.h"
#include "Buffer.h"
#include <string.h>

extern Uart_c Usart2;
extern LineBuffer_c<char, 256> RxBufferUart2;

ULONG Time;

VOID UsartReceiveThread(ULONG thread_input)
{
	Usart2.Init();
	RxBufferUart2.Clear();

	Result_e Command = Result_e::ERROR;

	while (1)
	{
		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::FULL_DATA_RECEIVED,
				TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
//			/* Обработчик принятия полного пакета данных */
			Command = RxBufferUart2.FindString("start");
			if (Command == Result_e::OK) {
				tx_event_flags_set(&MyEventGroup, (ULONG)Flags_e::SENSOR_IS_READY, TX_OR);
			}

			Command = RxBufferUart2.FindString("getdata");
			if (Command == Result_e::OK) {
				tx_event_flags_set(&MyEventGroup, (ULONG)Flags_e::START_POLLING_SENSOR, TX_OR);
			}

			Command = RxBufferUart2.FindString("stopdata");
			if (Command == Result_e::OK) {
				tx_event_flags_set(&MyEventGroup, ~(ULONG)(Flags_e::START_POLLING_SENSOR), TX_AND);
			}
		}
		sleep(_ms(5));
	}
}




