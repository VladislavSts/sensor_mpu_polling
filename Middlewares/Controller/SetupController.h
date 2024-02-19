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
	TRANSMIT_DATA_SENSOR
};

#define sleep(X) 	tx_thread_sleep(X)
#define _sec(X)		(X*1000)
#define _ms(X)		(X)

extern ULONG actual_events;

#define SendCommand(queue, command) \
    do { \
        Command_e cmd = command; \
        tx_queue_send(&queue, &cmd, 1000); \
    } while(0)

extern TX_QUEUE TxBllinkLedQueue;
extern TX_QUEUE TxUsartQueue;
extern TX_QUEUE TxPollingSensorQueue;

/* Возможность подключить один сенсор из предложенных на выбор, все будет работать :) */
#define MPU6050
//#define ADXL345
