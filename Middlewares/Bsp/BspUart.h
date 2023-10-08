/*
 * BspUart.h
 *
 *  Created on: Oct 7, 2023
 *      Author: Vladislav
 */

#pragma once

#include "main.h"
#include "BspGpio.h"

enum class Uart_e : uint8_t {
	UART_1,
	UART_2,
	UART_COUNT,
};


//struct ConfigUart_t {
//	LL_USART_InitTypeDef Handle;
//	Gpio_e Tx;
//	Gpio_e Rx;
//	Gpio_e Dir;
//};

