/*
 * BspI2c.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#include "BspI2c.h"
#include "main.h"

I2C_HandleTypeDef I2c1Hdl =
{
// I2C1 // ************************************************ //
   Instance : I2C1,
   Init : {
	  ClockSpeed : 100000,
	  DutyCycle : I2C_DUTYCYCLE_2,
	  OwnAddress1 : 0,
	  AddressingMode : I2C_ADDRESSINGMODE_7BIT,
	  DualAddressMode : I2C_DUALADDRESS_DISABLE,
	  OwnAddress2 : 0,
	  GeneralCallMode : I2C_GENERALCALL_DISABLE,
	  NoStretchMode : I2C_NOSTRETCH_DISABLE }
};

const Bsp_I2c_Config I2c1Cfg =
{
	SDA : Gpio_e::I2C_1_SDA,
	SCL : Gpio_e::I2C_1_SCL
};

//===============================================================================================//

I2c_c I2c1(&I2c1Hdl, &I2c1Cfg);

//===============================================================================================//
void I2c_c::Init()
{
	if (State == State_e::NOT_INIT) {
		GpioInit(I2c1Cfg.SDA);
		GpioInit(I2c1Cfg.SCL);

		if (I2cHdl->Instance == I2C1) {
			__HAL_RCC_I2C1_CLK_ENABLE();
		}

		if (HAL_I2C_Init(&I2c1Hdl) != HAL_OK) {
			Error_Handler();
		}

		State = State_e::INIT;
	}
}
//===============================================================================================//
Error_e I2c_c::MemmoryRead(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
	if (HAL_I2C_Mem_Read(I2cHdl, DevAddress, MemAddress, MemAddSize, pData, Size, 100) != HAL_OK)
		return Error_e::ERROR;

	return Error_e::OK;

}

Error_e I2c_c::MemmoryWrite(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
	if (HAL_I2C_Mem_Write(I2cHdl, DevAddress, MemAddress, MemAddSize, pData, Size, 100) != HAL_OK)
		return Error_e::ERROR;

	return Error_e::OK;

}


