/*
 * BspI2c.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#pragma once

#include "main.h"
#include "BspGpio.h"
#include "BspInterface.h"

struct Bsp_I2c_Config
{
	Gpio_e SDA;
	Gpio_e SCL;
};


class I2c_c : public I2c_i {
private:
	I2C_HandleTypeDef* I2cHdl;
	const Bsp_I2c_Config* const I2cCfg;

public:
	I2c_c(I2C_HandleTypeDef* I2cHdl, const Bsp_I2c_Config* I2cConfig) : I2cHdl(I2cHdl), I2cCfg(I2cConfig) {}

	~I2c_c() {};

	void Init() override;

	Error_e MemmoryRead(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize,
			uint8_t *pData, uint16_t Size) override;
};



