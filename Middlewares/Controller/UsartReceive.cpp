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

extern TX_EVENT_FLAGS_GROUP MyEventGroup;
extern ULONG actual_events;
extern Uart_c Usart2;
extern LineBuffer_c<uint8_t, 256> RxBufferUart2;

ULONG Time;

VOID UsartReceiveThread(ULONG thread_input)
{
	Usart2.Init();

	while (1)
	{
		if (tx_event_flags_get(&MyEventGroup, (ULONG)Flags_e::FULL_DATA_RECEIVED,
				TX_OR_CLEAR, &actual_events, TX_NO_WAIT) == TX_SUCCESS)
		{
			// Здесь уже можно работать с принятыми данными
			// Флаг StateIdle устанавливается в прерывании
			bool Command = RxBufferUart2.IsStringInBuffer("Start");

			if (Command) {
				tx_event_flags_set(&MyEventGroup, (ULONG)Flags_e::START_POLLING_SENSOR, TX_OR);
			}

			RxBufferUart2.Clear();

			// Ожидать команды на инициализацию mpu6050
		}
		sleep(_ms(5));
	}
}




