#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "output.h"
#include "shutdownManager.h"
#include "periodTimer.h"
#include "audioMixer.h"
#include "beatsMaker.h"

// Output Thread
static const int outputSleepMs = 1000;
static pthread_t outputThread;
static void* Output_threadFunction(void* args);

// Output Functions
static void Output_printStats(void);
static void Output_printTimingJitter(enum Period_whichEvent event, char* eventName);

void Output_startOutputing(void)
{
    pthread_create(&outputThread, NULL, &Output_threadFunction, NULL);
}

void Output_stopOutputing(void)
{
    pthread_join(outputThread, NULL);
}

static void* Output_threadFunction(void* args)
{
    while (!Shutdown_isShuttingDown()) {
        Output_printStats();
        Output_printTimingJitter(PERIOD_EVENT_REFILL_PLAYBACK_BUFFER, "Audio");
        printf("  ");
        Output_printTimingJitter(PERIOD_EVENT_SAMPLE_ACCELEROMETER, "Accel");
        printf("\n");
        Timer_sleepForMs(outputSleepMs);
    }
    return NULL;
}

static void Output_printStats(void)
{
    printf("M%d ", BeatsMaker_getMode());
    printf("%3dbpm ", BeatsMaker_getBpm());
    printf("vol:%3d  ", AudioMixer_getVolume());
}

static void Output_printTimingJitter(enum Period_whichEvent event, char* eventName)
{
    Period_statistics_t pStats;
    Period_getStatisticsAndClear(event, &pStats);
    char* timingJitterString = "%s[%6.3f, %6.3f] avg %6.3f/%2d";
    printf(timingJitterString, eventName, pStats.minPeriodInMs, pStats.maxPeriodInMs, pStats.avgPeriodInMs, pStats.numSamples);
}