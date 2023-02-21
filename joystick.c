#include "joystick.h"

#include <string.h>
#include <stdio.h>

#include "system.h"
#include "periodTimer.h"

static char* directionFile = "/sys/class/gpio/gpio%d/direction";
static char* valueFile = "/sys/class/gpio/gpio%d/value";

// Up, Down, Left, Right, Push
static int joystickGPIO[JOYSTICK_DIRECTION_COUNT] = {26, 46, 65, 47, 27};

void Joystick_intialize(void)
{
    // Configure pin's functions
    System_runCommand("config-pin p8.14 gpio");
    System_runCommand("config-pin p8.15 gpio");
    System_runCommand("config-pin p8.16 gpio");
    System_runCommand("config-pin p8.18 gpio");
    System_runCommand("config-pin p8.17 gpio");

    // Assume GPIO already exported

    // Configure pin to GPIO
    for (int i = 0; i < JOYSTICK_DIRECTION_COUNT; i++) {
        char filePath[BUFFER_MAX_LENGTH];
        snprintf(filePath, BUFFER_MAX_LENGTH, directionFile, joystickGPIO[i]);
        System_writeFile(filePath, "in");
    }
}

void Joystick_cleanUp(void)
{
    // do nothing
}

JoystickDirection Joystick_getDirection(void)
{
    for (int i = 0; i < JOYSTICK_DIRECTION_COUNT; i++) {
        char buffer[BUFFER_MAX_LENGTH];
        char filePath[BUFFER_MAX_LENGTH];

        snprintf(filePath, BUFFER_MAX_LENGTH, valueFile, joystickGPIO[i]);
        System_readFile(filePath, buffer);
        if (strncmp(buffer, "0", 1) == 0) {
            return i;
        }
    }
    return JOYSTICK_NO_DIRECTION;
}