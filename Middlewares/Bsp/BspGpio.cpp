/*
 * BspLed.cpp
 *
 *  Created on: Oct 6, 2023
 *      Author: Vladislav
 */

#include "BspGpio.h"
//===============================================================================================//

const ConfigGPIO_t ConfigGpio[(int)Gpio_e::GPIO_COUNT] =
{
// LED // ************************************************ //
	{
		.Port 		= GPIOB,
		.Pin 		= GPIO_PIN_2,
		.Speed 		= GPIO_SPEED_LOW,
		.StateOn 	= GPIO_PIN_SET,
		.StateOff 	= GPIO_PIN_RESET,
		.Mode 		= GPIO_MODE_OUTPUT_PP
	},

// UART_2_TX // ************************************************ //
	{
		.Port 		= GPIOA,
		.Pin 		= GPIO_PIN_2,
		.Speed 		= GPIO_SPEED_FREQ_HIGH,
		.StateOn 	= GPIO_PIN_RESET,
		.StateOff 	= GPIO_PIN_RESET,
		.Mode 		= GPIO_MODE_AF_PP
	},

// UART_3_RX // ************************************************ //
	{
		.Port 		= GPIOA,
		.Pin 		= GPIO_PIN_3,
		.Speed 		= GPIO_SPEED_FREQ_HIGH, // no effect
		.StateOn 	= GPIO_PIN_RESET,
		.StateOff 	= GPIO_PIN_RESET,
		.Mode 		= GPIO_MODE_INPUT,
		.Pull		= GPIO_NOPULL
	},

// I2C_1_SDA // ************************************************ //
	{
		.Port 		= GPIOB,
		.Pin 		= GPIO_PIN_7,
		.Speed 		= GPIO_SPEED_FREQ_HIGH,
		.StateOn 	= GPIO_PIN_RESET,
		.StateOff 	= GPIO_PIN_RESET,
		.Mode 		= GPIO_MODE_AF_OD
	},


// I2C_1_SCL // ************************************************ //
	{
		.Port 		= GPIOB,
		.Pin 		= GPIO_PIN_6,
		.Speed 		= GPIO_SPEED_FREQ_HIGH,
		.StateOn 	= GPIO_PIN_RESET,
		.StateOff 	= GPIO_PIN_RESET,
		.Mode 		= GPIO_MODE_AF_OD
	},
};

//===============================================================================================//
void GpioInitLevel (Gpio_e Index, GPIO_PinState State)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	HAL_GPIO_WritePin(ConfigGpio[(int) Index].Port, ConfigGpio[(int) Index].Pin, State);
	GPIO_InitStructure.Mode = ConfigGpio[(int) Index].Mode;
	GPIO_InitStructure.Speed = ConfigGpio[(int) Index].Speed;
	GPIO_InitStructure.Pull = ConfigGpio[(int) Index].Pull;
	GPIO_InitStructure.Pin = ConfigGpio[(int) Index].Pin;
	HAL_GPIO_Init(ConfigGpio[(int) Index].Port, &GPIO_InitStructure);
}
//===============================================================================================//
void GpioInit (Gpio_e Index)
{
	GpioInitLevel(Index, ConfigGpio[(int) Index].StateOff);
}
//===============================================================================================//
void GpioDeinit (Gpio_e Index)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	HAL_GPIO_WritePin(ConfigGpio[(int) Index].Port, ConfigGpio[(int) Index].Pin, ConfigGpio[(int) Index].StateOff);
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Speed = ConfigGpio[(int) Index].Speed;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = ConfigGpio[(int) Index].Pin;
	HAL_GPIO_Init(ConfigGpio[(int) Index].Port, &GPIO_InitStructure);
}
//===============================================================================================//
void GpioOn (Gpio_e Index)
{
	HAL_GPIO_WritePin(ConfigGpio[(int) Index].Port, ConfigGpio[(int) Index].Pin, ConfigGpio[(int) Index].StateOn);
}
//===============================================================================================//
void GpioOff (Gpio_e Index)
{
	HAL_GPIO_WritePin(ConfigGpio[(int) Index].Port, ConfigGpio[(int) Index].Pin, ConfigGpio[(int) Index].StateOff);
}
//===============================================================================================//
bool GpioGet (Gpio_e Index)
{
	if(ConfigGpio[(int) Index].StateOn == HAL_GPIO_ReadPin(ConfigGpio[(int) Index].Port, ConfigGpio[(int) Index].Pin))
		return true;
	else
		return false;
}


