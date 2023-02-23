// accelerometer.h
// Module to manage the accelerometer
#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

void Accelerometer_init(void);
void Accelerometer_cleanup(void);

void Accelerometer_readAndCalculateValues(void);

int Accelerometer_getX(void);
int Accelerometer_getY(void);
int Accelerometer_getZ(void);

#endif