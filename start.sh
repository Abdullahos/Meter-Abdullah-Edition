#!/bin/bash

./kill.sh

cd ~/Desktop/meter/
./kill.sh
cd ~/Desktop/meter/measurements
make
sudo ./metering.out

cd ~/Desktop/meter/tampering
make
sudo ./tampering.out

cd ~/Desktop/meter/server/events
#gcc events_sender.c IPC_MQ.c -o events.out -lcurl
make
sudo ./events.out

cd ~/Desktop/meter/server/readings
#gcc readings_sender.c IPC_MQ.c -o reads.out -lcurl
make
sudo ./reads.out

cd /home/pi/Desktop/meter/display
make
sudo ./display.out
