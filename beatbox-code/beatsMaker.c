#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#include "beatsMaker.h"
#include "audioMixer.h"
#include "shutdownManager.h"
#include "periodTimer.h"

#define DEFAULT_BPM 120
#define MAX_BPM 300
#define MIN_BPM 40

static int bpm;
static int beatsSleepMs;
void* beatsThreadFunction(void* arg);
static pthread_t beatsThread;

static BeatsModeEnum modeNum;
static bool (*currentBeats)[SOUND_COUNT];
static int beatIndex;

#define BEAT_COUNT 8

static char soundFile[SOUND_COUNT][FILENAME_MAX] = {
    "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav",
    "beatbox-wav-files/100053__menegass__gui-drum-cc.wav",
    "beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav"
};

// Pattern for beats1
//   Base, Hi-hat, Snare
static bool beats1[BEAT_COUNT][SOUND_COUNT] = {
    {true,  true, false},
    {false, true, false},
    {false, true, true},
    {false, true, false},
    {true,  true, false},
    {false, true, false},
    {false, true, true},
    {false, true, false}
};

// Pattern for beats2
//   Base, Hi-hat, Snare
static bool beats2[BEAT_COUNT][SOUND_COUNT] = {
    {true, false, true},
    {true, false, true},
    {true, true,  false},
    {true, false, false},
    {true, false, false},
    {true, false, true},
    {true, false, true},
    {true, true,  false}
};

void BeatsMaker_startMaking(void)
{
    BeatsMaker_setBpm(DEFAULT_BPM);
    BeatsMaker_changeMode(BEATS_MODE_0);
    pthread_create(&beatsThread, NULL, beatsThreadFunction, NULL);
}

void BeatsMaker_stopMaking(void)
{
    pthread_join(beatsThread, NULL);
}

int BeatsMaker_getBpm(void)
{
    return bpm;
}

void BeatsMaker_addBpm(int amount)
{
    int newBpm = bpm + amount;
    if (newBpm >= MIN_BPM && newBpm <= MAX_BPM) {
        BeatsMaker_setBpm(newBpm);
    }
}

void BeatsMaker_setBpm(int newBpm)
{
    bpm = newBpm;
    beatsSleepMs = ((60*1000)/ bpm) / 2;
}

int BeatsMaker_getMode(void) {
    return modeNum;
}

void BeatsMaker_cycleMode(void) {
    int mode = modeNum + 1;
    if (mode >= BEATS_MODE_COUNT) {
        mode = 0;
    }
    BeatsMaker_changeMode(mode);
}

void BeatsMaker_changeMode(BeatsModeEnum mode) {
    if (mode >= BEATS_MODE_COUNT) {
        return;
    }

    modeNum = mode;
    if (mode == BEATS_MODE_0) {
        currentBeats = NULL;
    } else if (mode == BEATS_MODE_1) {
        currentBeats = beats1;
    } else if (mode == BEATS_MODE_2) {
        currentBeats = beats2;
    }
}

void BeatsMaker_playSound(SoundEnum sound) {
    if (sound >= SOUND_COUNT) {
        return;
    }
    wavedata_t* pSound;
    pSound = malloc(sizeof(*pSound));
    AudioMixer_readWaveFileIntoMemory(soundFile[sound], pSound);
    AudioMixer_queueSound(pSound);
}

void* beatsThreadFunction(void* arg)
{
    while(!Shutdown_isShuttingDown()) {
        if (currentBeats == NULL) {
            Timer_sleepForMs(beatsSleepMs);
            continue;
        }

        for (int i = 0; i < SOUND_COUNT; i++) {
            if (currentBeats[beatIndex][i]) {
                BeatsMaker_playSound(i);
            }
        }

        beatIndex++;
        if (beatIndex == BEAT_COUNT) {
            beatIndex = 0;
        }
        Timer_sleepForMs(beatsSleepMs);
    }
    return NULL;
}