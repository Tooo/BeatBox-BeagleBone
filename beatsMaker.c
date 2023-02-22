#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "beatsMaker.h"
#include "audioMixer.h"
#include "shutdownManager.h"
#include "periodTimer.h"

#define DEFAULT_BPM 80
#define MAX_BPM 300
#define MIN_BPM 40

static int bpm;
static int beatsSleepMs;

void* beatsThreadFunction(void* arg);
static pthread_t beatsThread;

void BeatsMaker_startMaking(void)
{
    BeatsMaker_setBpm(DEFAULT_BPM);
    pthread_create(&beatsThread, NULL, beatsThreadFunction, NULL);
}

void BeatsMaker_stopMaking(void)
{
    pthread_join(beatsThread, NULL);
}

int  BeatsMaker_getBpm(void)
{
    return bpm;
}

void BeatsMaker_addBpm(int amount)
{
    int newBpm = bpm += amount;
    if (newBpm < MIN_BPM || newBpm > MAX_BPM) {
        return;
    }
    BeatsMaker_setBpm(newBpm);
}

void BeatsMaker_setBpm(int newBpm)
{
    bpm = newBpm;
    beatsSleepMs = ((60*1000)/ bpm) / 2;
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

        AudioMixer_readWaveFileIntoMemory("beatbox-wav-files/100053__menegass__gui-drum-cc.wav", pSound2);
        AudioMixer_queueSound(pSound2);

        Timer_sleepForMs(beatsSleepMs);
    }
    return NULL;
}