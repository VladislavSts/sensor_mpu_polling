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
#include "Mpu6050.h"
#include <string.h>

extern Uart_c Usart2;
extern LineBuffer_c<char, 256> RxBufferUart2;
extern MPU6050Sensor Mpu;

extern TX_QUEUE TxUsartReceiveQueue;
extern TX_QUEUE TxPollingSensorQueue;

VOID UsartReceiveThread(ULONG thread_input)
{
	Usart2.Init();
	RxBufferUart2.Clear();

	Result_e CommandRx = Result_e::ERROR;

	UINT Result;
	Command_e Command;

	while (1)
	{
		Result = tx_queue_receive(&TxUsartReceiveQueue, &Command, TX_WAIT_FOREVER);

		if (Result == TX_SUCCESS)
		{
			/* Обработчик принятия полного пакета данных */
			CommandRx = RxBufferUart2.FindString("start");
			if (CommandRx == Result_e::OK) {
				Command_e Command = Command_e::COMMAND_START;
			    tx_queue_send(&TxPollingSensorQueue, &Command, TX_NO_WAIT);
			}

			CommandRx = RxBufferUart2.FindString("getdata");
			if (CommandRx == Result_e::OK && Mpu.State == State_e::INIT) {
				Command_e Command = Command_e::START_POLLING_SENSOR;
			    tx_queue_send(&TxPollingSensorQueue, &Command, TX_NO_WAIT);
			}

			CommandRx = RxBufferUart2.FindString("stopdata");
			if (CommandRx == Result_e::OK && Mpu.State == State_e::INIT) {
				Command_e Command = Command_e::STOP_POLLING_SENSOR;
			    tx_queue_send(&TxPollingSensorQueue, &Command, TX_NO_WAIT);
			}
		}
	}
}




