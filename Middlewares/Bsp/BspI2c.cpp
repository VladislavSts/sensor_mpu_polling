/*
 * BspI2c.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#include "BspI2c.h"

const ConfigI2C_t ConfigI2c[(int)I2C_e::I2C_COUNT] =
{
// I2C1 // ************************************************ //
	{
		I2cHandle : {
		  .Instance = I2C1,
		Init : {
		  .ClockSpeed = 100000,
		  .DutyCycle = I2C_DUTYCYCLE_2,
		  .OwnAddress1 = 0,
		  .AddressingMode = I2C_ADDRESSINGMODE_7BIT,
		  .DualAddressMode = I2C_DUALADDRESS_DISABLE,
		  .OwnAddress2 = 0,
		  .GeneralCallMode = I2C_GENERALCALL_DISABLE,
		  .NoStretchMode = I2C_NOSTRETCH_DISABLE, }
		},
		.SDA = Gpio_e::I2C_1_SDA,
		.SCL = Gpio_e::I2C_1_SCL,
	}
};
//===============================================================================================//
void I2cInit(I2C_e Index)
{
	switch(Index) {
		case I2C_e::I2C_1:
			__HAL_RCC_I2C1_CLK_ENABLE();
			break;

		default:
			;
	}

	I2C_HandleTypeDef I2C_InitStruct = {0};

	I2C_InitStruct.Instance = ConfigI2c[(int)Index].I2cHandle.Instance;
	I2C_InitStruct.Init.ClockSpeed = ConfigI2c[(int)Index].I2cHandle.Init.ClockSpeed;
	I2C_InitStruct.Init.DutyCycle = ConfigI2c[(int)Index].I2cHandle.Init.DutyCycle;
	I2C_InitStruct.Init.OwnAddress1 = ConfigI2c[(int)Index].I2cHandle.Init.OwnAddress1;
	I2C_InitStruct.Init.AddressingMode = ConfigI2c[(int)Index].I2cHandle.Init.AddressingMode;
	I2C_InitStruct.Init.DualAddressMode = ConfigI2c[(int)Index].I2cHandle.Init.DualAddressMode;
	I2C_InitStruct.Init.OwnAddress2 = ConfigI2c[(int)Index].I2cHandle.Init.OwnAddress2;
	I2C_InitStruct.Init.GeneralCallMode = ConfigI2c[(int)Index].I2cHandle.Init.GeneralCallMode;
	I2C_InitStruct.Init.NoStretchMode = ConfigI2c[(int)Index].I2cHandle.Init.NoStretchMode;

	if (HAL_I2C_Init(&I2C_InitStruct) != HAL_OK) {
		Error_Handler();
	}
}
