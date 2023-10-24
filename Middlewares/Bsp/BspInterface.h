
/*
 * BspInitInterface.h
 *
 *  Created on: Oct 12, 2023
 *      Author: Vladislav
 */

#pragma once

enum class State_e {
	INIT = 0u,
	NOT_INIT,
	INIT_ERROR,
};

enum class Error_e {
	OK = 0u,
	ERROR,
};

class I2c_i {
public:
	State_e State = State_e::NOT_INIT;

	void virtual Init() = 0;
	void virtual DeInit() = 0;

	Error_e virtual MemmoryRead(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize,
			uint8_t *pData, uint16_t Size) = 0;

	Error_e virtual MemmoryWrite(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize,
				uint8_t *pData, uint16_t Size) = 0;

	virtual ~I2c_i() {};
};
