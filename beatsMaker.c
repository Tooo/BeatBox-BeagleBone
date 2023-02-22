#include <stdlib.h>
#include <pthread.h>

#include "beatsMaker.h"
#include "audioMixer.h"
#include "shutdownManager.h"
#include "periodTimer.h"

void* beatsThreadFunction(void* arg);
static pthread_t beatsThread;

void BeatsMaker_startMaking(void)
{
    pthread_create(&beatsThread, NULL, beatsThreadFunction, NULL);
}

void BeatsMaker_stopMaking(void)
{
    pthread_join(beatsThread, NULL);
}

void* beatsThreadFunction(void* arg)
{
    while(!Shutdown_isShuttingDown()) {

        wavedata_t* pSound;
        pSound = malloc(sizeof(*pSound));

        AudioMixer_readWaveFileIntoMemory("beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav", pSound);
        AudioMixer_queueSound(pSound);


        wavedata_t* pSound2;
        pSound2 = malloc(sizeof(*pSound2));

        AudioMixer_readWaveFileIntoMemory("beatbox-wav-files/100061__menegass__gui-drum-splash-soft.wav", pSound2);
        AudioMixer_queueSound(pSound2);


        Timer_sleepForMs(1000);
        
    }
    return NULL;
}