#include <pthread.h>

#include "acceleReader.h"
#include "shutdownManager.h"
#include "beatsMaker.h"
#include "periodTimer.h"

static const int acceleratorSleepMs = 10;
// static const int acceleratorPushSleepMS = 300;
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

void* acceleratorThreadFunction(void* arg)
{
    while(!Shutdown_isShuttingDown()) {

        Period_markEvent(PERIOD_EVENT_SAMPLE_ACCELEROMETER);
        Timer_sleepForMs(acceleratorSleepMs);
    }
    return NULL;
}