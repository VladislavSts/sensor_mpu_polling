/*
 * SetupController.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#pragma once

#include "tx_api.h"
#include "stdint.h"

enum class Command_e : uint8_t
{
	FULL_DATA_RECEIVED,
	FULL_DATA_TRANSMIT,
	COMMAND_START,
	SENSOR_IS_READY,
	START_POLLING_SENSOR,
	STOP_POLLING_SENSOR,
	TRANSMIT_DATA_SENSOR,
};

#define sleep(X) 	tx_thread_sleep(X)
#define _sec(X)		(X*1000)
#define _ms(X)		(X)

extern ULONG actual_events;


