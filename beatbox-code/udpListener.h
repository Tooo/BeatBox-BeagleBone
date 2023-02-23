// udpListener.h
// Module to manage UDP server thread
// Thread looping to receive commands
#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

// Begin/end the background thread which listens to the UDP server
void Udp_startListening(void);
void Udp_stopListening(void);

#endif