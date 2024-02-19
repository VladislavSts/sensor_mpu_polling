/*
 * SetupController.cpp
 *
 *  Created on: Oct 6, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"
#include "main.h"

//===============================================================================================//
/* Размер пула памяти, из которого будет выделяться память для демо-потоков, в байтах */
#define     STACK_SIZE         1024
#define     BYTE_POOL_SIZE     9120
#define     NUMBER_OF_MESSAGES 100
#define     MESSAGE_SIZE       TX_1_ULONG
#define     QUEUE_SIZE         NUMBER_OF_MESSAGES*MESSAGE_SIZE*sizeof(ULONG)

/* Область памяти для пула TX_BYTE_POOL */
UCHAR MemoryArea[BYTE_POOL_SIZE];

/* Структура, хранящая информацию о пуле памяти */
TX_BYTE_POOL BytePool;

/* Массив структур, каждая из которых хранит информацию о потоке (thread control block) */
TX_THREAD TxBlinkLedThread, TxUsartThread, TxPollingSensorThread;

/* Очереди */
TX_QUEUE TxBllinkLedQueue, TxUsartQueue, TxPollingSensorQueue;

VOID BlinkLedThread(ULONG thread_input);
VOID UsartThread(ULONG thread_input);
VOID PollingSensorThread(ULONG thread_input);

//===============================================================================================//
void tx_application_define(void *first_unused_memory)
{
	UINT ResultRequest;

/* Указатели на память для стека каждого потока */
	CHAR *BlinkLed, *Usart, *PollingSensor;

	CHAR *BllinkLedQueue, *UsartQueue, *PollingSensorQueue;

/* Создаем byte memory pool, из которого будем выделять память для стека каждого потока */
	tx_byte_pool_create(&BytePool, (char*)"byte_pool", MemoryArea, BYTE_POOL_SIZE);

// ВЫДЕЛЕНИЕ СТЕКА ДЛЯ ПОТОКОВ
	tx_byte_allocate(&BytePool, (VOID**) &BlinkLed, 		STACK_SIZE, TX_NO_WAIT);
	tx_byte_allocate(&BytePool, (VOID**) &Usart, 			STACK_SIZE, TX_NO_WAIT);
	tx_byte_allocate(&BytePool, (VOID**) &PollingSensor, 	STACK_SIZE, TX_NO_WAIT);

// ВЫДЕЛЕНИЕ СТЕКА ДЛЯ ОЧЕРЕДЕЙ
	tx_byte_allocate(&BytePool, (VOID**)&BllinkLedQueue, 		QUEUE_SIZE, TX_NO_WAIT);
	tx_byte_allocate(&BytePool, (VOID**)&UsartQueue, 	QUEUE_SIZE, TX_NO_WAIT);
	tx_byte_allocate(&BytePool, (VOID**)&PollingSensorQueue, 	QUEUE_SIZE, TX_NO_WAIT);

// СОЗДАНИЕ ОЧЕРЕДЕЙ
	ResultRequest = tx_queue_create(&TxBllinkLedQueue, 		(CHAR*)"TxBllinkLedQueue", 	   MESSAGE_SIZE, (VOID**)&BllinkLedQueue, 	  QUEUE_SIZE);
	ResultRequest = tx_queue_create(&TxUsartQueue, 			(CHAR*)"TxUsartQueue",  		MESSAGE_SIZE, (VOID**)&UsartQueue,  	  QUEUE_SIZE);
	ResultRequest = tx_queue_create(&TxPollingSensorQueue, 	(CHAR*)"TxPollingSensorQueue", MESSAGE_SIZE, (VOID**)&PollingSensorQueue, QUEUE_SIZE);

	if (ResultRequest != TX_SUCCESS) {
		Error_Handler();
	}

// СОЗДАНИЕ ПОТОКОВ
	tx_thread_create(&TxBlinkLedThread, (char*)"TxBlinkLedThread", BlinkLedThread, 1,
			BlinkLed, 1024, 10, 10, TX_NO_TIME_SLICE, TX_AUTO_START);

	tx_thread_create(&TxUsartThread, (char*)"TxUsartReceiveThread", UsartThread, 2,
			Usart, 1024, 10, 10, TX_NO_TIME_SLICE, TX_AUTO_START);

	tx_thread_create(&TxPollingSensorThread, (char*)"TxPollingSensorThread", PollingSensorThread, 4,
			PollingSensor, 1024, 10, 10, TX_NO_TIME_SLICE, TX_AUTO_START);
}



