/*
 * SetupController.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#pragma once

#include "tx_api.h"

#define FLAG(X) (1 << X)

/* флаги для группы флагов событий */
enum class Flags_e
{
	FULL_DATA_RECEIVED	 = 1u,							// данные полностью приняты
	FULL_DATA_TRANSMIT   = (FULL_DATA_RECEIVED << 1),	// данные полностью отправлены
	COMMAND_START		 = (FULL_DATA_TRANSMIT << 1),	// пришла команда старт
	SENSOR_IS_READY		 = (COMMAND_START << 1),		// сенсор инициализирован и готов к работе
	START_POLLING_SENSOR = (SENSOR_IS_READY << 1),		// запуск опроса сенсора
	TRANSMIT_DATA_SENSOR = (START_POLLING_SENSOR << 1)	// запуск отправки данных
};

#define sleep(X) 	tx_thread_sleep(X)
#define _sec(X)		(X*1000)
#define _ms(X)		(X)

#define FREQ_POLING_MS (30)

extern TX_EVENT_FLAGS_GROUP MyEventGroup;
extern ULONG actual_events;


