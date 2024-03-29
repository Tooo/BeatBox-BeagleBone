// joystickReader.h
// Module to read the joystick
#ifndef JOYSTICK_READER_H_
#define JOYSTICK_READER_H_

// Begin/end background thread which reads joystick inputs
void JoystickReader_startReading(void);
void JoystickReader_stopReading(void);

#endif