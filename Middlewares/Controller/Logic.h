/*
 * Logic.h
 *
 *  Created on: Oct 6, 2023
 *      Author: Vladislav
 */

#pragma once

#define sleep(X) 	tx_thread_sleep(X)
#define _sec(X)		(X*1000)
#define _ms(X)		(X)

VOID BlinkLedThread(ULONG);



