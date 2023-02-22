#include <pthread.h>

#include "shutdownManager.h"
#include "audioMixer.h"
#include "beatsMaker.h"

// Mutex variables
static pthread_mutex_t shutdownMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t shutdownCond = PTHREAD_COND_INITIALIZER;

// Shutdown boolean
static bool isShuttingDown;

void Shutdown_init(void)
{   
    isShuttingDown = false;
    AudioMixer_startMixing();
    BeatsMaker_startMaking();
}

void Shutdown_cleanup(void)
{
    AudioMixer_stopMixing();
    BeatsMaker_stopMaking();
}

void Shutdown_wait(void)
{
    pthread_mutex_lock(&shutdownMutex);
    {
        pthread_cond_wait(&shutdownCond, &shutdownMutex);
    }
    pthread_mutex_unlock(&shutdownMutex);
}

void Shutdown_trigger(void)
{
    isShuttingDown = true;
    pthread_mutex_lock(&shutdownMutex);
    {
        pthread_cond_signal(&shutdownCond);
    }
    pthread_mutex_unlock(&shutdownMutex);
}

bool Shutdown_isShuttingDown(void)
{
    return isShuttingDown;
}