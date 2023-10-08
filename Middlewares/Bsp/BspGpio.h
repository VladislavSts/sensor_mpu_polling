/*
 * BspGpio.h
 *
 *  Created on: Oct 6, 2023
 *      Author: Vladislav
 */

#pragma once

#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum class Gpio_e : uint8_t {
	LED,

	UART_1_TX,
	UART_1_RX,

	GPIO_COUNT,
};
//===============================================================================================//
struct ConfigGPIO_t
{
	GPIO_TypeDef *Port;
	uint16_t Pin;
	uint32_t Speed;
	GPIO_PinState StateOn;
	GPIO_PinState StateOff;
	uint32_t Mode;
	uint32_t Pull;
	uint32_t Alternate;
};
//===============================================================================================//
void GpioInitLevel (Gpio_e, GPIO_PinState);
void GpioInit (Gpio_e);
void GpioDeinit (Gpio_e);
void GpioOn (Gpio_e);
void GpioSet (Gpio_e, float);
void GpioOff (Gpio_e);
bool GpioGet (Gpio_e);
//===============================================================================================//
#ifdef __cplusplus
}
#endif


