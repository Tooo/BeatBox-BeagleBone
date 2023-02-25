#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>

#include "udpListener.h"
#include "shutdownManager.h"
#include "audioMixer.h"
#include "beatsMaker.h"

// UDP values
#define MAX_LEN 1024
#define MAX_PACKET_SIZE 20
#define PORT 12345

// UDP variables
static int socketDescriptor;
static struct sockaddr_in sinRemote;

// UDP Thread
static pthread_t udpThread;
static void* Udp_threadFunction(void* args);

#define UDP_VOLUME_INCREMENT 5
#define UDP_BPM_INCREMENT 5

// Intialize Server function
static void Udp_serverInit(void);

// UDP Server Commands
static void Udp_mode();
static void Udp_changeMode(int mode);

static void Udp_volume(void);
static void Udp_volumeUp(void);
static void Udp_volumeDown(void);

static void Udp_tempo(void);
static void Udp_tempoUp(void);
static void Udp_tempDown(void);

static void Udp_playSound(int sound);
static void Udp_stop(void);
static void Udp_unknown(void);

static int Udp_getNumber(char* messageTx);

// Return UDP message
static void Udp_send(char* messageTx);

void Udp_startListening(void)
{
    pthread_create(&udpThread, NULL, &Udp_threadFunction, NULL);
}

void Udp_stopListening(void)
{
    pthread_join(udpThread, NULL);
}

static void* Udp_threadFunction(void* args)
{
    Udp_serverInit();
    char previousMessage[MAX_LEN] = "";
    int num = 0;

    while (!Shutdown_isShuttingDown()){
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MAX_LEN];

        int bytesRx = recvfrom(socketDescriptor, messageRx, MAX_LEN - 1, 0,
                                (struct sockaddr*) &sinRemote, &sin_len);

        messageRx[bytesRx] = '\0';

        // Just "enter"
        if (bytesRx == 1) {
            strcpy(messageRx, previousMessage);
        }

        // Menu options
        if (strncmp(messageRx, "mode ", 5) == 0) {
            if (bytesRx != 1) {
                num = Udp_getNumber(messageRx);
            } 
            Udp_changeMode(num);
         } else if (strncmp(messageRx, "mode", 4) == 0) {
            Udp_mode();
        } else if (strncmp(messageRx, "volumeup", 8) == 0) {
            Udp_volumeUp();
        } else if (strncmp(messageRx, "volumedown", 10) == 0) {
            Udp_volumeDown();
        } else if (strncmp(messageRx, "volume", 6) == 0) {
            Udp_volume();
        } else if (strncmp(messageRx, "tempoup", 7) == 0) {
            Udp_tempoUp();
        } else if (strncmp(messageRx, "tempodown", 9) == 0) {
            Udp_tempDown();
        } else if (strncmp(messageRx, "tempo", 5) == 0) {
            Udp_tempo();
        } else if (strncmp(messageRx, "sound", 5) == 0) {
            if (bytesRx != 1) {
                num = Udp_getNumber(messageRx);
            } 
            Udp_playSound(num);
        } else if (strncmp(messageRx, "stop", 4) == 0) {
            Udp_stop();
        } else {
            Udp_unknown();
        }     

        strcpy(previousMessage, messageRx);

    }
    close (socketDescriptor);
    return NULL;
}

static void Udp_serverInit(void) {
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
}

static void Udp_mode()
{
    char messageTx[MAX_LEN];
    int newMode = BeatsMaker_getMode();
    snprintf(messageTx, MAX_LEN, "mode %d \n", newMode);
    Udp_send(messageTx);
}

static void Udp_changeMode(int mode)
{
    BeatsMaker_changeMode(mode);
    Udp_mode();
}

static void Udp_volume(void)
{
    char messageTx[MAX_LEN];
    int volume = AudioMixer_getVolume();
    snprintf(messageTx, MAX_LEN, "volume %d \n", volume);
    Udp_send(messageTx);
}

static void Udp_volumeUp(void)
{
    AudioMixer_addVolume(UDP_VOLUME_INCREMENT);
    Udp_volume();
}

static void Udp_volumeDown(void)
{
    AudioMixer_addVolume(-UDP_VOLUME_INCREMENT);
    Udp_volume();
}

static void Udp_tempo(void)
{
    char messageTx[MAX_LEN];
    int tempo = BeatsMaker_getBpm();
    snprintf(messageTx, MAX_LEN, "tempo %d \n", tempo);
    Udp_send(messageTx);
}

static void Udp_tempoUp(void)
{
    BeatsMaker_addBpm(UDP_BPM_INCREMENT);
    Udp_tempo();
}

static void Udp_tempDown(void)
{
    BeatsMaker_addBpm(-UDP_BPM_INCREMENT);
    Udp_tempo();
}

static void Udp_playSound(int sound)
{
    BeatsMaker_playSound(sound);
    char* messageTx = "played \n";
    Udp_send(messageTx);
}

static void Udp_stop(void)
{
    char* messageTx = "terminating \n";
    Udp_send(messageTx);
    Shutdown_trigger();
}

static void Udp_unknown(void)
{
    char* messageTx = "Unknown Command. Please type help for command examples.\n";
    Udp_send(messageTx);
}

static int Udp_getNumber(char* messageTx)
{
    char* token = strtok(messageTx, " ");
    token = strtok(NULL, " ");
    return atoi(token);
}

static void Udp_send(char* messageTx) 
{
    unsigned int sin_len = sizeof(sinRemote);
    sendto(socketDescriptor, messageTx, strlen(messageTx), 0,
            (struct sockaddr *) &sinRemote, sin_len);
}