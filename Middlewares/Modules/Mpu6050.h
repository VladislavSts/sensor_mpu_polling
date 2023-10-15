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

// MPU6050 structure
typedef struct {

    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
    double Gx;
    double Gy;
    double Gz;

    float Temperature;

    double KalmanAngleX;
    double KalmanAngleY;
} MPU6050_t;

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
	MPU6050_t* DataMpu;

public:
	MPU6050Sensor(I2c_i& I2c, MPU6050_t* DataMpu) : I2c(I2c), DataMpu(DataMpu) {}

	uint8_t MPU6050_Init();
	void MPU6050_Read_Accel();
	void MPU6050_Read_Gyro();
	void MPU6050_Read_Temp();
	void MPU6050_Read_All();
	double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt);
};

