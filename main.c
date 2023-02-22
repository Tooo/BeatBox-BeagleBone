// main.c
// Main Thread of BeatBox program
#include <stdio.h>

#include "shutdownManager.h"
#include "periodTimer.h"
#include "audioMixer.h"
#include "joystick.h"

// Initialize/cleanup the module's data structures.
static void main_init(void);
static void main_cleanup(void);

int main(void)
{
    main_init();
    Shutdown_wait();
    main_cleanup();
    return 0;
}

static void main_init(void)
{
    AudioMixer_init();
    Period_init();
    Joystick_init();
    Shutdown_init();
}

static void main_cleanup(void)
{
    Shutdown_cleanup();
    Joystick_cleanup();
    Period_cleanup();
    AudioMixer_cleanup();
}