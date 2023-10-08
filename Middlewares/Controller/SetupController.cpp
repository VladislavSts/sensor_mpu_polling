/*
 * SetupController.cpp
 *
 *  Created on: Oct 6, 2023
 *      Author: Vladislav
 */

#include "tx_api.h"
#include "Logic.h"
//===============================================================================================//
/* Размер пула памяти, из которого будет выделяться память для демо-потоков, в байтах */
#define DEMO_BYTE_POOL_SIZE 10240

//===============================================================================================//
/* Область памяти для пула TX_BYTE_POOL */
UCHAR MemoryArea[DEMO_BYTE_POOL_SIZE];

//===============================================================================================//
/* Структура, хранящая информацию о пуле памяти */
TX_BYTE_POOL BytePool;

//===============================================================================================//
/* Массив структур, каждая из которых хранит информацию о потоке (thread control block) */
TX_THREAD TxBlinkLedThread;

//===============================================================================================//
void tx_application_define(void *first_unused_memory)
{
/* Указатели на память для стека каждого потока */
	CHAR *BlinkLed;

/* Создаем byte memory pool, из которого будем выделять память для стека каждого потока */
	tx_byte_pool_create(&BytePool, (char*)"byte_pool", MemoryArea, DEMO_BYTE_POOL_SIZE);

// ВЫДЕЛЕНИЕ СТЕКА ДЛЯ ПОТОКОВ
	tx_byte_allocate(&BytePool, (VOID**) &BlinkLed, 1024, TX_NO_WAIT);


// СОЗДАНИЕ ПОТОКОВ
	tx_thread_create(&TxBlinkLedThread, (char*)"TxBlinkLedThread", BlinkLedThread, 1,
			BlinkLed, 1024, 15, 15, TX_NO_TIME_SLICE, TX_AUTO_START);
}



