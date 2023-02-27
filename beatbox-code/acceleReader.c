#include <pthread.h>
#include <stdio.h>

#include "acceleReader.h"
#include "accelerometer.h"
#include "shutdownManager.h"
#include "beatsMaker.h"
#include "periodTimer.h"

// Average calculations
#define GFORCE_TOLERANCE 4000
static const double averageWeight = 0.001;
static long long sampleTaken = 0;
static int averages[AXIS_COUNT];

// Accelerometer thread
static const int accelerometerSleepMs = 10;
static const int accelerometerPlaySleepMS = 250;
static void* accelerometerThreadFunction(void* arg);
static pthread_t accelerometerThread;

void AcceleReader_startReading(void)
{
    pthread_create(&accelerometerThread, NULL, accelerometerThreadFunction, NULL);
}

void AcceleReader_stopReading(void)
{
    pthread_join(accelerometerThread, NULL);
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

static void* accelerometerThreadFunction(void* arg)
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
            Timer_sleepForMs(accelerometerPlaySleepMS);
        } else {
            Timer_sleepForMs(accelerometerSleepMs);
        }
        
    }
    return NULL;
}