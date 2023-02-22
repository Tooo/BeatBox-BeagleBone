// beatsMaker.h
// Module to make beats
#ifndef BEATS_MAKER_H_
#define BEATS_MAKER_H_

// Begin/end background thread which make beats
void BeatsMaker_startMaking(void);
void BeatsMaker_stopMaking(void);

// Change BPM
int  BeatsMaker_getBpm(void);
void BeatsMaker_addBpm(int amount);
void BeatsMaker_setBpm(int newBpm);

#endif