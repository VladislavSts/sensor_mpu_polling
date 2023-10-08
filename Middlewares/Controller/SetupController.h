/*
 * SetupController.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#pragma once

#include "BspGpio.h"
#include "BspUart.h"
#include "Buffer.h"

#define FLAG(x) (1 << x)

/* флаги для группы флагов событий приема данных */
enum class Flags_e
{
	FULL_DATA_RECEIVED	= 1u,
};

#define sleep(X) 	tx_thread_sleep(X)
#define _sec(X)		(X*1000)
#define _ms(X)		(X)




