/*
 * mpu6050.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Bulanov Konstantin
 *      Refactor: Vladislav
 */

#pragma once

#include <stdint.h>
#include <math.h>
#include "BspInterface.h"

struct DataMpu {
	float Ax = 0;
	float Ay = 0;
	float Az = 0;
	float Gx = 0;
	float Gy = 0;
	float Gz = 0;
	float Temp = 0;
};

class MPU6050Sensor {
private:
	I2c_i& I2c;
	DataMpu& Data;

	int16_t Accel_X_RAW = 0;
	int16_t Accel_Y_RAW = 0;
	int16_t Accel_Z_RAW = 0;

	int16_t Gyro_X_RAW = 0;
	int16_t Gyro_Y_RAW = 0;
	int16_t Gyro_Z_RAW = 0;

public:
	MPU6050Sensor(I2c_i& I2c, DataMpu& Data) : I2c(I2c), Data(Data) {}

	uint8_t MPU6050_Init();
	void MPU6050_Read_Accel();
	void MPU6050_Read_Gyro();
	void MPU6050_Read_Temp();
};

