/*
 * Mpu6050.cpp
 *
 *  Created on: Oct 12, 2023
 *      Author: Vladislav
 */

#include "Mpu6050.h"
#include "tx_api.h"
#include "stm32f1xx_hal.h"

#define MPU6050_ADDR 0xD0

#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75


uint8_t MPU6050Sensor::MPU6050_Init() {
	if (I2c.State == State_e::NOT_INIT) {
		I2c.Init();
	}

	uint8_t check = 0;
	uint8_t Data = 0;

	// check device ID WHO_AM_I
	I2c.MemmoryRead(MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1);

	if (check == 104) {
		// 0x68 will be returned by the sensor if everything goes well
		// power management register 0X6B we should write all 0's to wake the sensor up
		Data = 0;
		I2c.MemmoryRead(MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1);

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		I2c.MemmoryRead(MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
		Data = 0x00;
		I2c.MemmoryRead(MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
		Data = 0x00;
		I2c.MemmoryRead(MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1);
		return 0;
	}
	return 1;
}
//===============================================================================================//
void MPU6050Sensor::MPU6050_Read_Accel() {
	uint8_t Rec_Data[6] = {0};
	// Read 6 BYTES of data starting from ACCEL_XOUT_H register
	I2c.MemmoryRead(MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6);

	Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	/*** convert the RAW values into acceleration in 'g'
		 we have to divide according to the Full scale value set in FS_SEL
		 I have configured FS_SEL = 0. So I am dividing by 16384.0
		 for more details check ACCEL_CONFIG Register              ****/

	Data.Ax = Accel_X_RAW/16384.0;
	Data.Ay = Accel_Y_RAW/16384.0;
	Data.Az = Accel_Z_RAW/16384.0;
}
//===============================================================================================//
void MPU6050Sensor::MPU6050_Read_Gyro() {
	uint8_t Rec_Data[6] = {0};
	// Read 6 BYTES of data starting from GYRO_XOUT_H register
	I2c.MemmoryRead(MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6);

	Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	/*** convert the RAW values into dps (°/s)
		 we have to divide according to the Full scale value set in FS_SEL
		 I have configured FS_SEL = 0. So I am dividing by 131.0
		 for more details check GYRO_CONFIG Register              ****/

	Data.Gx = Gyro_X_RAW/131.0;
	Data.Gy = Gyro_Y_RAW/131.0;
	Data.Gz = Gyro_Z_RAW/131.0;
}
//===============================================================================================//
void MPU6050Sensor::MPU6050_Read_Temp() {
	uint8_t Rec_Data[2] = {0};
	int16_t temp;
	// Read 2 BYTES of data starting from TEMP_OUT_H_REG register
	I2c.MemmoryRead(MPU6050_ADDR, TEMP_OUT_H_REG, 1, Rec_Data, 2);

	temp = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
	Data.Temp = (float) ((int16_t) temp / (float) 340.0 + (float) 36.53);
}



