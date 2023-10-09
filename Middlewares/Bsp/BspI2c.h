/*
 * BspI2c.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Vladislav
 */

#pragma once

#include "main.h"
#include "BspGpio.h"

enum class I2C_e {
	I2C_1 = 0u,
	I2C_COUNT,
};

struct ConfigI2C_t {
	I2C_HandleTypeDef I2cHandle;
	Gpio_e SDA;
	Gpio_e SCL;
};



