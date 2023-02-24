#include <pthread.h>

#include "joystickReader.h"
#include "joystick.h"
#include "shutdownManager.h"
#include "audioMixer.h"
#include "beatsMaker.h"
#include "periodTimer.h"

#define JOYSTICK_VOLUME_INCREMENT 5
#define JOYSTICK_BPM_INCREMENT 5

static const int joystickSleepMs = 10;
static const int joystickPushSleepMS = 300;
static const int joystickDirectionSleepMS = 250;
static void* joystickThreadFunction(void* arg);
static pthread_t joystickThread;

void JoystickReader_startReading(void)
{
    pthread_create(&joystickThread, NULL, joystickThreadFunction, NULL);
}

void JoystickReader_stopReading(void)
{
    pthread_join(joystickThread, NULL);
}

void* joystickThreadFunction(void* arg)
{
    while(!Shutdown_isShuttingDown()) {
        JoystickDirection direction = Joystick_getDirection();

        if (direction == JOYSTICK_NO_DIRECTION) {
            Timer_sleepForMs(joystickSleepMs);
            continue;
        }

        if (direction == JOYSTICK_PUSH) {
            BeatsMaker_cycleMode();
            Timer_sleepForMs(joystickPushSleepMS);
            continue;
        }

        if (direction == JOYSTICK_UP) {
            AudioMixer_addVolume(JOYSTICK_VOLUME_INCREMENT);
        } else if (direction == JOYSTICK_DOWN) {
            AudioMixer_addVolume(-JOYSTICK_VOLUME_INCREMENT);
        } else if (direction == JOYSTICK_RIGHT) {
            BeatsMaker_addBpm(JOYSTICK_BPM_INCREMENT);
        } else if (direction == JOYSTICK_LEFT) {
            BeatsMaker_addBpm(-JOYSTICK_BPM_INCREMENT);
        }

        Timer_sleepForMs(joystickDirectionSleepMS);
    }
    return NULL;
}