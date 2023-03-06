# BeatBox BeagleBone
Listen to multiple beats.
Control the mode, volume and beats with joystick.
Shake BeagleBone to play as an air drum.
Display stats of audio and accelerometer every second.
Node.js Web Interface with buttons to control beatbox.
Reports to Webserver when C Program or Nodejs crashes.

## Installation
1. Clone the repository
```bash
(host)$ git clone https://github.com/Tooo/BeatBox-BeagleBone.git
```

2. Make excutable file on the host
```bash
(host)$ make
```

3. Run excutable on the BeagleBone
```bash
(bbg)$ ./beatbox
```

4. Run nodejs web server
```bash
(bbg)$ cd /beatbox-server-copy/ && node server.js
```

## Web Interface
(Screenshots TBA)

## Threads
| Threads | Purpose | 
| :------ | :------ |
| Main | Start other threads and waits for shutdown to cleanup |
| Audio Mixing | Mix audio and play them to them playback buffer |
| Beats Maker | Play the sound clips in the beats order with the user selected tempo |
| Joystick Reader | Read joystick inputs every 10ms |
| Accelerometer Reader | Read accelerometer inputs every 10ms |
| UDP Listener | Listen on the UDP server and respond to commands |
| Terminal Output | Display terminal output of stats and timing jittering every 1s |
