/*
 * BspI2c.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#pragma once

#include "main.h"
#include "BspGpio.h"


struct Bsp_I2c_Config
{
	Gpio_e SDA;
	Gpio_e SCL;
};


class I2c_c {
private:
	I2C_HandleTypeDef* I2cHdl;
	const Bsp_I2c_Config* const I2cCfg;

public:
	I2c_c(I2C_HandleTypeDef* I2cHdl, const Bsp_I2c_Config* I2cConfig) : I2cHdl(I2cHdl), I2cCfg(I2cConfig) {}

	void Init();
};



