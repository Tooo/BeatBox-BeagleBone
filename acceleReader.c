#include <pthread.h>
#include <stdio.h>

#include "acceleReader.h"
#include "accelerometer.h"
#include "shutdownManager.h"
#include "beatsMaker.h"
#include "periodTimer.h"

#define X_GFORCE_DEFAULT -400
#define Y_GFORCE_DEFAULT -300
#define Z_GFORCE_DEFAULT 17000
#define GFORCE_TOLERANCE 4000

#define AXIS_COUNT 3

static const double averageWeight = 0.001;

static long long sampleTaken = 0;
static int averages[AXIS_COUNT];

static const int acceleratorSleepMs = 10;
static const int acceleratorPlaySleepMS = 250;
static void* acceleratorThreadFunction(void* arg);
static pthread_t acceleratorThread;

void AcceleReader_startReading(void)
{
    pthread_create(&acceleratorThread, NULL, acceleratorThreadFunction, NULL);
}

void AcceleReader_stopReading(void)
{
    pthread_join(acceleratorThread, NULL);
}

static void AcceleReader_calculateAverage(int* accelerations) {
    if (sampleTaken == 1) {
        for (int i = 0; i < AXIS_COUNT; i++) {
            averages[i] = accelerations[i];
        }
    } else {
        for (int i = 0; i < AXIS_COUNT; i++) {
            averages[i] = averageWeight * accelerations[i] + (1.0-averageWeight) * averages[i];
        }
    }
}

static void* acceleratorThreadFunction(void* arg)
{
    while(!Shutdown_isShuttingDown()) {
        Accelerometer_readAndCalculateValues();
        int x = Accelerometer_getX();
        int y = Accelerometer_getY();
        int z = Accelerometer_getZ();

        int accelerations[3] = {x, y, z};
        Period_markEvent(PERIOD_EVENT_SAMPLE_ACCELEROMETER);
        sampleTaken++;

        AcceleReader_calculateAverage(accelerations);

        bool played = false;
        for (int i = 0; i < AXIS_COUNT; i++) {
            if (accelerations[i] < averages[i] - GFORCE_TOLERANCE )  {
                BeatsMaker_playSound(i);
                played = true;
            }
        }
        if (played) {
            Timer_sleepForMs(acceleratorPlaySleepMS);
        } else {
            Timer_sleepForMs(acceleratorSleepMs);
        }
        
    }
    return NULL;
}