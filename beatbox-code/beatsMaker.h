// beatsMaker.h
// Module to make beats
#ifndef BEATS_MAKER_H_
#define BEATS_MAKER_H_

typedef enum {
    SOUND_BASE = 0,
    SOUND_HI_HAT,
    SOUND_SNARE,
    SOUND_COUNT
} SoundEnum;

typedef enum {
    BEATS_MODE_0 = 0,
    BEATS_MODE_1,
    BEATS_MODE_2,
    BEATS_MODE_COUNT
} BeatsModeEnum;

// Begin/end background thread which make beats
void BeatsMaker_startMaking(void);
void BeatsMaker_stopMaking(void);

// Change BPM
int  BeatsMaker_getBpm(void);
void BeatsMaker_addBpm(int amount);
void BeatsMaker_setBpm(int newBpm);

void BeatsMaker_playSound(SoundEnum sound);

int BeatsMaker_getMode(void);
void BeatsMaker_cycleMode(void);
void BeatsMaker_changeMode(BeatsModeEnum mode);

#endif