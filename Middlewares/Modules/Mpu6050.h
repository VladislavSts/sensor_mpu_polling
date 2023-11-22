/*
 * mpu6050.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Vladislav
 *
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

    double KalmanAngleX;
    double KalmanAngleY;
};

// Kalman structure
typedef struct {
    double Q_angle;
    double Q_bias;
    double R_measure;
    double angle;
    double bias;
    double P[2][2];
} Kalman_t;

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

	Error_e MPU6050_Read_Accel();
	Error_e MPU6050_Read_Gyro();
	Error_e MPU6050_Read_Temp();
	double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt);

public:
	MPU6050Sensor(I2c_i& I2c, DataMpu& Data) : I2c(I2c), Data(Data) {}

	State_e State = State_e::NOT_INIT;

	void MPU6050_Init();
	void MPU6050_DeInit();

	Error_e ReadAllData();

};

