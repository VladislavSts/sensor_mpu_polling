/*
 * SetupController.cpp
 *
 *  Created on: Oct 6, 2023
 *      Author: Vladislav
 */

#include "SetupController.h"

//===============================================================================================//
/* Размер пула памяти, из которого будет выделяться память для демо-потоков, в байтах */
#define DEMO_BYTE_POOL_SIZE 10240

/* Область памяти для пула TX_BYTE_POOL */
UCHAR MemoryArea[DEMO_BYTE_POOL_SIZE];

/* Структура, хранящая информацию о пуле памяти */
TX_BYTE_POOL BytePool;

/* Группа флагов событий */
TX_EVENT_FLAGS_GROUP FullDataReceived;
ULONG actual_events;

/* Массив структур, каждая из которых хранит информацию о потоке (thread control block) */
TX_THREAD TxBlinkLedThread;
TX_THREAD TxUsartReceiveThread;
TX_THREAD TxUsartTransmitThread;


VOID BlinkLedThread(ULONG thread_input);
VOID UsartReceiveThread(ULONG thread_input);
VOID UsartTransmitThread(ULONG thread_input);

//===============================================================================================//
void tx_application_define(void *first_unused_memory)
{
/* Указатели на память для стека каждого потока */
	CHAR *BlinkLed, *UsartReceive, *UsartTransmit;

/* Создаем byte memory pool, из которого будем выделять память для стека каждого потока */
	tx_byte_pool_create(&BytePool, (char*)"byte_pool", MemoryArea, DEMO_BYTE_POOL_SIZE);

// СОЗДАНИЕ ГРУППЫ ФЛАГОВ СОБЫТИЙ
	tx_event_flags_create(&FullDataReceived, (CHAR*)"FullDataReceived");

// ВЫДЕЛЕНИЕ СТЕКА ДЛЯ ПОТОКОВ
	tx_byte_allocate(&BytePool, (VOID**) &BlinkLed, 1024, TX_NO_WAIT);

	tx_byte_allocate(&BytePool, (VOID**) &UsartReceive, 1024, TX_NO_WAIT);

	tx_byte_allocate(&BytePool, (VOID**) &UsartTransmit, 1024, TX_NO_WAIT);

// СОЗДАНИЕ ПОТОКОВ
	tx_thread_create(&TxBlinkLedThread, (char*)"TxBlinkLedThread", BlinkLedThread, 1,
			BlinkLed, 1024, 15, 15, TX_NO_TIME_SLICE, TX_AUTO_START);

	tx_thread_create(&TxUsartReceiveThread, (char*)"TxUsartReceiveThread", UsartReceiveThread, 2,
			UsartReceive, 1024, 15, 15, TX_NO_TIME_SLICE, TX_AUTO_START);

	tx_thread_create(&TxUsartTransmitThread, (char*)"TxUsartTransmitThread", UsartTransmitThread, 3,
			UsartTransmit, 1024, 15, 15, TX_NO_TIME_SLICE, TX_AUTO_START);
}



