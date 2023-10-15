/*
 * Mpu6050.cpp
 *
 *  Created on: Oct 12, 2023
 *      Author: Vladislav
 */

#include "Mpu6050.h"
#include "tx_api.h"
#include "stm32f1xx_hal.h"

#define RAD_TO_DEG 57.295779513082320876798154814105

#define WHO_AM_I_REG 		0x75
#define PWR_MGMT_1_REG 		0x6B
#define SMPLRT_DIV_REG 		0x19
#define ACCEL_CONFIG_REG 	0x1C
#define ACCEL_XOUT_H_REG 	0x3B
#define TEMP_OUT_H_REG 		0x41
#define GYRO_CONFIG_REG 	0x1B
#define GYRO_XOUT_H_REG 	0x43

//#define HAL_GetTick() tx_time_get()

// Setup MPU6050
#define MPU6050_ADDR 0xD0
const double Accel_Z_corrector = 14418.0;

uint32_t timer;

Kalman_t KalmanX = {
    .Q_angle = 0.001f,
    .Q_bias = 0.003f,
	.R_measure = 0.03f
};

Kalman_t KalmanY = {
	.Q_angle = 0.001f,
	.Q_bias = 0.003f,
	.R_measure = 0.03f,
};

uint8_t MPU6050Sensor::MPU6050_Init()
	{
		if (I2c.State == State_e::NOT_INIT) {
			I2c.Init();
		}

		uint8_t check;
		uint8_t Data;
		// check device ID WHO_AM_I
		I2c.MemmoryRead(MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1);

		if (check == 104)  // 0x68 will be returned by the sensor if everything goes well
		{
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
	void MPU6050Sensor::MPU6050_Read_Accel()
	{
		uint8_t Rec_Data[6];
		// Read 6 BYTES of data starting from ACCEL_XOUT_H register
		I2c.MemmoryRead(MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, sizeof(Rec_Data));

		DataMpu->Accel_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
		DataMpu->Accel_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
		DataMpu->Accel_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);

		/*** convert the RAW values into acceleration in 'g'
			 we have to divide according to the Full scale value set in FS_SEL
			 I have configured FS_SEL = 0. So I am dividing by 16384.0
			 for more details check ACCEL_CONFIG Register              ****/

		DataMpu->Ax = DataMpu->Accel_X_RAW / 16384.0;
		DataMpu->Ay = DataMpu->Accel_Y_RAW / 16384.0;
		DataMpu->Az = DataMpu->Accel_Z_RAW / Accel_Z_corrector;
	}
//===============================================================================================//
	void MPU6050Sensor::MPU6050_Read_Gyro()
	{
		uint8_t Rec_Data[6];
		// Read 6 BYTES of data starting from GYRO_XOUT_H register
		I2c.MemmoryRead(MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, sizeof(Rec_Data));

		DataMpu->Gyro_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
		DataMpu->Gyro_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
		DataMpu->Gyro_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);

		/*** convert the RAW values into dps (�/s)
		 we have to divide according to the Full scale value set in FS_SEL
		 I have configured FS_SEL = 0. So I am dividing by 131.0
		 for more details check GYRO_CONFIG Register              ****/

		DataMpu->Gx = DataMpu->Gyro_X_RAW / 131.0;
		DataMpu->Gy = DataMpu->Gyro_Y_RAW / 131.0;
		DataMpu->Gz = DataMpu->Gyro_Z_RAW / 131.0;
	}
//===============================================================================================//
	void MPU6050Sensor::MPU6050_Read_Temp()
	{
		uint8_t Rec_Data[2] = {0};
		int16_t temp;
		// Read 2 BYTES of data starting from TEMP_OUT_H_REG register
		I2c.MemmoryRead(MPU6050_ADDR, TEMP_OUT_H_REG, 1, Rec_Data, sizeof(Rec_Data));

		temp = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
		DataMpu->Temperature = (float) ((int16_t) temp / (float) 340.0 + (float) 36.53);
	}
//===============================================================================================//
	void MPU6050Sensor::MPU6050_Read_All()
	{
		uint8_t Rec_Data[14] = {0};
		int16_t temp;
		// Read 14 BYTES of data starting from ACCEL_XOUT_H register
		I2c.MemmoryRead(MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, sizeof(Rec_Data));

		DataMpu->Accel_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
		DataMpu->Accel_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
		DataMpu->Accel_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);
		temp = (int16_t) (Rec_Data[6] << 8 | Rec_Data[7]);
		DataMpu->Gyro_X_RAW = (int16_t) (Rec_Data[8] << 8 | Rec_Data[9]);
		DataMpu->Gyro_Y_RAW = (int16_t) (Rec_Data[10] << 8 | Rec_Data[11]);
		DataMpu->Gyro_Z_RAW = (int16_t) (Rec_Data[12] << 8 | Rec_Data[13]);

		DataMpu->Ax = DataMpu->Accel_X_RAW / 16384.0;
		DataMpu->Ay = DataMpu->Accel_Y_RAW / 16384.0;
		DataMpu->Az = DataMpu->Accel_Z_RAW / Accel_Z_corrector;
		DataMpu->Temperature = (float) ((int16_t) temp / (float) 340.0 + (float) 36.53);
		DataMpu->Gx = DataMpu->Gyro_X_RAW / 131.0;
		DataMpu->Gy = DataMpu->Gyro_Y_RAW / 131.0;
		DataMpu->Gz = DataMpu->Gyro_Z_RAW / 131.0;

// Kalman angle solve
		double dt = (double) (HAL_GetTick() - timer) / 1000;
		timer = HAL_GetTick();
		double roll;
		double roll_sqrt = sqrt(DataMpu->Accel_X_RAW * DataMpu->Accel_X_RAW + DataMpu->Accel_Z_RAW * DataMpu->Accel_Z_RAW);

		if (roll_sqrt != 0.0)  roll = atan(DataMpu->Accel_Y_RAW / roll_sqrt) * RAD_TO_DEG;
		else roll = 0.0;

		double pitch = atan2(-DataMpu->Accel_X_RAW, DataMpu->Accel_Z_RAW) * RAD_TO_DEG;

		if ((pitch < -90 && DataMpu->KalmanAngleY > 90) || (pitch > 90 && DataMpu->KalmanAngleY < -90)) {
			KalmanY.angle = pitch;
			DataMpu->KalmanAngleY = pitch;
		}
		else DataMpu->KalmanAngleY = Kalman_getAngle(&KalmanY, pitch, DataMpu->Gy, dt);


		if (fabs(DataMpu->KalmanAngleY) > 90) DataMpu->Gx = -DataMpu->Gx;

		DataMpu->KalmanAngleX = Kalman_getAngle(&KalmanX, roll, DataMpu->Gy, dt);
	}
//===============================================================================================//
	double MPU6050Sensor::Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt)
	{
		double rate = newRate - Kalman->bias;
		Kalman->angle += dt * rate;

		Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
		Kalman->P[0][1] -= dt * Kalman->P[1][1];
		Kalman->P[1][0] -= dt * Kalman->P[1][1];
		Kalman->P[1][1] += Kalman->Q_bias * dt;

		double S = Kalman->P[0][0] + Kalman->R_measure;
		double K[2];
		K[0] = Kalman->P[0][0] / S;
		K[1] = Kalman->P[1][0] / S;

		double y = newAngle - Kalman->angle;
		Kalman->angle += K[0] * y;
		Kalman->bias += K[1] * y;

		double P00_temp = Kalman->P[0][0];
		double P01_temp = Kalman->P[0][1];

		Kalman->P[0][0] -= K[0] * P00_temp;
		Kalman->P[0][1] -= K[0] * P01_temp;
		Kalman->P[1][0] -= K[1] * P00_temp;
		Kalman->P[1][1] -= K[1] * P01_temp;

		return Kalman->angle;
	}


