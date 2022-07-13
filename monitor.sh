#!/bin/bash

ps -eo 'tty,pid,comm' | grep tampering.out 

ps -eo 'tty,pid,comm' | grep metering.out 

ps -eo 'tty,pid,comm' | grep events.out 

ps -eo 'tty,pid,comm' | grep reads.out 

ps -eo 'tty,pid,comm' | grep display.out

database=$(ls ~/Desktop/meter/server/database)
#echo $database
echo "database content is:"
echo $database
