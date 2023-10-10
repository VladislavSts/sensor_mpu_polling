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
	FULL_DATA_RECEIVED	 = 1u,
	FULL_DATA_TRANSMIT   = (FULL_DATA_RECEIVED << 1),
	SENSOR_IS_READY		 = (FULL_DATA_TRANSMIT << 1),
	START_POLLING_SENSOR = (SENSOR_IS_READY << 1),
};

#define sleep(X) 	tx_thread_sleep(X)
#define _sec(X)		(X*1000)
#define _ms(X)		(X)




