/*
 * BspI2c.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#include "BspI2c.h"

I2C_HandleTypeDef I2c1Hdl =
{
// I2C1 // ************************************************ //
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
};

const Bsp_I2c_Config I2c1Cfg =
{
	.SDA = Gpio_e::I2C_1_SDA,
	.SCL = Gpio_e::I2C_1_SCL
};

//===============================================================================================//

I2c_c I2c1(&I2c1Hdl, &I2c1Cfg);

//===============================================================================================//
void I2c_c::Init()
{
	if (I2cHdl->Instance == I2C1)
		__HAL_RCC_I2C1_CLK_ENABLE();


	I2C_HandleTypeDef I2C_InitStruct = {0};

	I2C_InitStruct.Instance = I2cHdl->Instance;
	I2C_InitStruct.Init.ClockSpeed = I2cHdl->Init.ClockSpeed;
	I2C_InitStruct.Init.DutyCycle = I2cHdl->Init.DutyCycle;
	I2C_InitStruct.Init.OwnAddress1 = I2cHdl->Init.OwnAddress1;
	I2C_InitStruct.Init.AddressingMode = I2cHdl->Init.AddressingMode;
	I2C_InitStruct.Init.DualAddressMode = I2cHdl->Init.DualAddressMode;
	I2C_InitStruct.Init.OwnAddress2 = I2cHdl->Init.OwnAddress2;
	I2C_InitStruct.Init.GeneralCallMode = I2cHdl->Init.GeneralCallMode;
	I2C_InitStruct.Init.NoStretchMode = I2cHdl->Init.NoStretchMode;

	if (HAL_I2C_Init(&I2C_InitStruct) != HAL_OK) {
		Error_Handler();
	}
}
