#include <unistd.h>

#include "accelerometer.h"
#include "system.h"

// I2c bus and address
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_ACCEL_ADDRESS 0x1C

// I2C values for accelerometer
#define REG_ACTIVE 0x2A

// I2C file descriptor
static int i2cFileDesc;

void Accelerometer_init(void)
{
    System_runCommand("config-pin P9_18 i2c");
    System_runCommand("config-pin P9_17 i2c");

    i2cFileDesc = System_initI2cBus(I2CDRV_LINUX_BUS1, I2C_ACCEL_ADDRESS);

    System_writeI2cReg(i2cFileDesc, REG_ACTIVE, 0x01);
}

void Accelerometer_cleanup(void)
{
    System_writeI2cReg(i2cFileDesc, REG_ACTIVE, 0x00);
    close(i2cFileDesc);
}