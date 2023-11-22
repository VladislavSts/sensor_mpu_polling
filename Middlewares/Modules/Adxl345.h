/*
 * Adxl345.h
 *
 *  Created on: 1 нояб. 2023 г.
 *      Author: Vladislav
 */

#pragma once

#include "BspInterface.h"
#include "stdint.h"

#define ADXL_ADDRESS (0x53 << 1)

struct DataAdxl
{
	float xg;
	float yg;
	float zg;
};

class Adxl345
{
private:
	I2c_i& I2c;
	DataAdxl& Data;

	int16_t x = 0, y = 0, z = 0;

	uint8_t data_rec[6] = {0};
	uint8_t chipid = 0;

	Error_e ReadAddress(uint8_t reg)
	{
		if (I2c.MemmoryRead(ADXL_ADDRESS, reg, 1, &chipid, 1) == Error_e::OK)
			return Error_e::OK;

		return Error_e::ERROR;
	}

	Error_e WriteData(uint8_t reg, uint8_t value)
	{
		uint8_t data[2] = {0};
		data[0] = reg;
		data[1] = value;

		if (I2c.MasterTransmit(ADXL_ADDRESS, data, 2) == Error_e::OK)
			return Error_e::OK;

		return Error_e::ERROR;
	}

public:
	State_e State = State_e::NOT_INIT;

	Adxl345(I2c_i& I2c, DataAdxl& Data) : I2c(I2c), Data(Data) {}

	Error_e ReadValues(uint8_t reg)
	{
		if (I2c.MemmoryRead(ADXL_ADDRESS, reg, 1, (uint8_t *)data_rec, 6) == Error_e::OK) {
			x = ((data_rec[1]<<8)|data_rec[0]);
			y = ((data_rec[3]<<8)|data_rec[2]);
			z = ((data_rec[5]<<8)|data_rec[4]);

			Data.xg = x * .0078;
			Data.yg = y * .0078;
			Data.zg = z * .0078;

			return Error_e::OK;
		}
		return Error_e::ERROR;
	}

	void Init()
	{
		if (ReadAddress(0x00) != Error_e::OK) // read the DEVID
			State = State_e::INIT_ERROR;

		if (WriteData(0x31, 0x01) != Error_e::OK) // data_format range= +- 4g
			State = State_e::INIT_ERROR;

		if (WriteData (0x2d, 0x00) != Error_e::OK)  // reset all bits
			State = State_e::INIT_ERROR;

		if (WriteData (0x2d, 0x08) != Error_e::OK)  // power_cntl measure and wake up 8hz
			State = State_e::INIT_ERROR;

		if (State != State_e::INIT_ERROR) // ошибок на этапе инициализации не возникло
			State = State_e::INIT;
	}
};

//void adxl_write (uint8_t reg, uint8_t value)
//{
//	uint8_t data[2];
//	data[0] = reg;
//	data[1] = value;
//	HAL_I2C_Master_Transmit (&hi2c1, ADXL_ADDRESS, data, 2, 100);
//}
//
//void adxl_read_values (uint8_t reg)
//{
//	HAL_I2C_Mem_Read (&hi2c1, ADXL_ADDRESS, reg, 1, (uint8_t *)data_rec, 6, 100);
//}
//
//void adxl_read_address (uint8_t reg)
//{
//	HAL_I2C_Mem_Read (&hi2c1, ADXL_ADDRESS, reg, 1, &chipid, 1, 100);
//}
//
//void adxl_init (void)
//{
//	adxl_read_address (0x00); // read the DEVID
//
//	adxl_write (0x31, 0x01);  // data_format range= +- 4g
//	adxl_write (0x2d, 0x00);  // reset all bits
//	adxl_write (0x2d, 0x08);  // power_cntl measure and wake up 8hz
//
//}
