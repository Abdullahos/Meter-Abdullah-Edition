
#!/bin/bash

str1=$(ps -eo 'tty,pid,comm' | grep tampering.out )
echo $str1
num1=${str1//[!0-9]/}
echo "killing process with pid = $num1..."
sudo kill $num1

str2=$(ps -eo 'tty,pid,comm' | grep metering.out )
echo $str2
num2=${str2//[!0-9]/}
echo "killing process with pid = $num2..."
sudo kill $num2

str3=$(ps -eo 'tty,pid,comm' | grep events.out )
echo $str3
num3=${str3//[!0-9]/}
echo "killing process with pid = $num3..."
sudo kill $num3

str4=$(ps -eo 'tty,pid,comm' | grep reads.out )
echo $str4
num4=${str4//[!0-9]/}
echo "killing process with pid = $num4..."
sudo kill $num4


str5=$(ps -eo 'tty,pid,comm' | grep display.out )
echo $str5
num5=${str5//[!0-9]/}
echo "killing process with pid = $num5..."
sudo kill $num5


