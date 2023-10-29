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

VOID UsartReceiveThread(ULONG thread_input)
{
	Usart2.Init();
	RxBufferUart2.Clear();

	UINT Result;
	Command_e Command;
	Result_e CommandRx = Result_e::ERROR;

	while (1)
	{
		Result = tx_queue_receive(&TxUsartReceiveQueue, &Command, TX_WAIT_FOREVER);

		if (Result == TX_SUCCESS)
		{
			/* Обработчик принятия полного пакета данных */
			CommandRx = RxBufferUart2.FindString("start");
			if (CommandRx == Result_e::OK) {
			    SendCommand(TxPollingSensorQueue, Command_e::COMMAND_START);
			}

			CommandRx = RxBufferUart2.FindString("getdata");
			if (CommandRx == Result_e::OK && Mpu.State == State_e::INIT) {
			    SendCommand(TxPollingSensorQueue, Command_e::START_POLLING_SENSOR);
			}

			CommandRx = RxBufferUart2.FindString("stopdata");
			if (CommandRx == Result_e::OK && Mpu.State == State_e::INIT) {
				SendCommand(TxPollingSensorQueue, Command_e::STOP_POLLING_SENSOR);
			}
		}
	}
}




