/*
 * Mpu6050.cpp
 *
 *  Created on: Oct 12, 2023
 *      Author: Vladislav
 */

#include "Mpu6050.h"
#include "tx_api.h"
#include "math.h"

#define MPU6050_ADDR 		0xD0	// Адрес mpu6050 на шине I2c

#define WHO_AM_I_REG 		0x75	// Идентификация устройства
#define SMPLRT_DIV_REG 		0x19	// Настройка делителя частоты выборки
#define GYRO_CONFIG_REG 	0x1B	// Настройка конфигурации гироскопа
#define ACCEL_CONFIG_REG 	0x1C	// Настройка конфигурации акселерометра
#define ACCEL_XOUT_H_REG 	0x3B	// Чтение старшего байта данных акселерометра
#define TEMP_OUT_H_REG	 	0x41	// Чтение старшего байта данных температуры
#define GYRO_XOUT_H_REG 	0x43	// Чтение старшего байта данных гироскопа
#define PWR_MGMT_1_REG 		0x6B	// Режим управления питанием

#define RAD_TO_DEG 57.295779513082320876798154814105

#define CORRECT_ACCEL 2048.0
#define CORRECT_GYRO  16.4

const double Accel_Z_corrector = 14418.0;
uint32_t timer;

void MPU6050Sensor::MPU6050_Init()
{
	if (State == State_e::NOT_INIT) {
		uint8_t check = 0;
		uint8_t Data = 0;

		// check device ID WHO_AM_I
		I2c.MemmoryRead(MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1);

		if (check == 104) {
			// 0x68 will be returned by the sensor if everything goes well
			// power management register 0X6B we should write all 0's to wake the sensor up
			Data = 0;
			I2c.MemmoryWrite(MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1);

			// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
			Data = 0x07;
			I2c.MemmoryWrite(MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1);

			// Set accelerometer configuration in ACCEL_CONFIG Register
			// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> +-16g
			Data = 0x18;
			I2c.MemmoryWrite(MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1);

			// Set Gyroscopic configuration in GYRO_CONFIG Register
			// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> +-2000*/s
			Data = 0x18;
			I2c.MemmoryWrite(MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1);

			State = State_e::INIT;
		}
		else {
			I2c.DeInit();
			State = State_e::INIT_ERROR;
		}
	}
}

//===============================================================================================//

void MPU6050Sensor::MPU6050_DeInit()
{
	if (State == State_e::INIT) {
		I2c.DeInit();
		State = State_e::NOT_INIT;
	}
}

//===============================================================================================//
Error_e MPU6050Sensor::MPU6050_Read_Accel()
{
	Error_e Result;

	if (State == State_e::NOT_INIT)
		MPU6050_Init();

	if (State == State_e::INIT) {
		uint8_t Rec_Data[6] = {0};
		// Read 6 BYTES of data starting from ACCEL_XOUT_H register
		if (I2c.MemmoryRead(MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6) == Error_e::OK) {
			Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
			Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
			Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

			/*** convert the RAW values into acceleration in 'g'
				 we have to divide according to the Full scale value set in FS_SEL
				 I have configured FS_SEL = 0. So I am dividing by 16384.0
				 for more details check ACCEL_CONFIG Register              ****/

			Data.Ax = Accel_X_RAW/CORRECT_ACCEL;
			Data.Ay = Accel_Y_RAW/CORRECT_ACCEL;
			Data.Az = Accel_Z_RAW/CORRECT_ACCEL;

			Result = Error_e::OK;
		}
		else Result = Error_e::ERROR;
	}
	else Result = Error_e::ERROR;

	return Result;
}
//===============================================================================================//
Error_e MPU6050Sensor::MPU6050_Read_Gyro()
{
	Error_e Result;

	if (State == State_e::NOT_INIT)
		MPU6050_Init();

	if (State == State_e::INIT) {
		uint8_t Rec_Data[6] = {0};
		// Read 6 BYTES of data starting from GYRO_XOUT_H register
		if (I2c.MemmoryRead(MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6) == Error_e::OK) {
			Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
			Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
			Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

			/*** convert the RAW values into dps (°/s)
				 we have to divide according to the Full scale value set in FS_SEL
				 I have configured FS_SEL = 0. So I am dividing by 131.0
				 for more details check GYRO_CONFIG Register              ****/

			Data.Gx = Gyro_X_RAW/CORRECT_GYRO;
			Data.Gy = Gyro_Y_RAW/CORRECT_GYRO;
			Data.Gz = Gyro_Z_RAW/CORRECT_GYRO;

			Result = Error_e::OK;
		}
		else Result = Error_e::ERROR;
	}
	else Result = Error_e::ERROR;

	return Result;
}
//===============================================================================================//
Error_e MPU6050Sensor::MPU6050_Read_Temp()
{
	Error_e Result;

	if (State == State_e::NOT_INIT)
		MPU6050_Init();

	if (State == State_e::INIT) {
		uint8_t Rec_Data[2] = {0};
		int16_t temp;
		// Read 2 BYTES of data starting from TEMP_OUT_H_REG register
		if (I2c.MemmoryRead(MPU6050_ADDR, TEMP_OUT_H_REG, 1, Rec_Data, 2) == Error_e::OK) {
			temp = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
			Data.Temp = (float) ((int16_t) temp / (float) 340.0 + (float) 36.53);

			Result = Error_e::OK;
		}
		else Result = Error_e::ERROR;
	}
	else Result = Error_e::ERROR;

	return Result;
}
//===============================================================================================//
extern uint32_t uwTick;

Error_e MPU6050Sensor::ReadAllData()
{
	Error_e Result;

	if (State == State_e::NOT_INIT)
		MPU6050_Init();

	if (State == State_e::INIT) {
	// Read 14 BYTES of data starting from ACCEL_XOUT_H register
		uint8_t Rec_Data[14] = {0};
		int16_t temp = 0;

		if (I2c.MemmoryRead(MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 14) == Error_e::OK)
		{
			Accel_X_RAW = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
			Accel_Y_RAW = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
			Accel_Z_RAW = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);
			temp = (int16_t) (Rec_Data[6] << 8 | Rec_Data[7]);
			Gyro_X_RAW = (int16_t) (Rec_Data[8] << 8 | Rec_Data[9]);
			Gyro_Y_RAW = (int16_t) (Rec_Data[10] << 8 | Rec_Data[11]);
			Gyro_Z_RAW = (int16_t) (Rec_Data[12] << 8 | Rec_Data[13]);

			Data.Ax = Accel_X_RAW / CORRECT_ACCEL;
			Data.Ay = Accel_Y_RAW / CORRECT_ACCEL;
			Data.Az = Accel_Z_RAW / CORRECT_ACCEL;

			Data.Temp = (float) ((int16_t) temp / (float) 340.0 + (float) 36.53);

			Data.Gx = Gyro_X_RAW / CORRECT_GYRO;
			Data.Gy = Gyro_Y_RAW / CORRECT_GYRO;
			Data.Gz = Gyro_Z_RAW / CORRECT_GYRO;

			// Kalman angle solve
			double dt = (double) (tx_time_get() - timer) / 1000;
			timer = tx_time_get();

			double roll;
			double roll_sqrt = sqrt(Accel_X_RAW * Accel_X_RAW + Accel_Z_RAW * Accel_Z_RAW);

			if (roll_sqrt != 0.0) {
				roll = atan(Accel_Y_RAW / roll_sqrt) * RAD_TO_DEG;
			} else {
				roll = 0.0;
			}

			double pitch = atan2(-Accel_X_RAW, Accel_Z_RAW) * RAD_TO_DEG;

			if ((pitch < -90 && Data.KalmanAngleY > 90) || (pitch > 90 && Data.KalmanAngleY < -90)) {
				KalmanY.angle = pitch;
				Data.KalmanAngleY = pitch;
			}
			else {
				Data.KalmanAngleY = Kalman_getAngle(&KalmanY, pitch, Data.Gy, dt);
			}

			if (fabs(Data.KalmanAngleY) > 90) {
				Data.Gx = -Data.Gx;
			}
			Data.KalmanAngleX = Kalman_getAngle(&KalmanX, roll, Data.Gy, dt);

			Result = Error_e::OK;
		}
		else Result = Error_e::ERROR;
	}
	else Result = Error_e::ERROR;

	return Result;
}

/* Фильтр Калмана */
double MPU6050Sensor::Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt) {
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
};
