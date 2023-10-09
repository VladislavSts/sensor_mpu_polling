/*
 * BspUart.h
 *
 *  Created on: Oct 7, 2023
 *      Author: Vladislav
 */

#pragma once

#include "main.h"
#include "BspGpio.h"

#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256

struct Bsp_Uart_Config {
	Gpio_e Tx;
	Gpio_e Rx;
	Gpio_e Dir;
};

class Uart_c {
private:
	USART_TypeDef *UsartX;
	LL_USART_InitTypeDef* UartHdl;
	const Bsp_Uart_Config* const UartCfg;

public:
	Uart_c(USART_TypeDef *UsartX, LL_USART_InitTypeDef* UartHdl, const Bsp_Uart_Config* UartCfg) :
		UsartX(UsartX), UartHdl(UartHdl), UartCfg(UartCfg) {}

	void Init();
};

