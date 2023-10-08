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

#ifdef __cplusplus
extern "C"
{
#endif

void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void USART2_IRQHandler(void);

#ifdef __cplusplus
}
#endif


enum class Uart_e : uint8_t {
	UART_2,
	UART_COUNT,
};


struct ConfigUart_t {
	LL_USART_InitTypeDef Handle;
	Gpio_e Tx;
	Gpio_e Rx;
	Gpio_e Dir;
};

void UartInit(Uart_e Index);

