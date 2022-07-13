This is V1.0


Warning: Embedded systems is not that easy y 3zezee, So ->
	 readme.txt is intending to make it easy for you

                    --------------------------------------------   
                            -------------------
                           |  pinout & wiring   |
                            ------------------- 
see pinout_definitions.h file in the same directory
 

                    --------------------------------------------   

                            -------------------
                           |  	spi enable     |
                            ------------------- 

1-open config.txt in /boot
	$ sudo nano /boot/config.txt

2-add the following to config.txt
dtoverlay=spi0-0cs
dtoverlay=spi0-1cs
dtoverlay=spi1-0cs
dtoverlay=spi1-1cs

3-then save the file using 
ctrl+shift x
y
enter

4-reboot your system using 
	$ sudo reboot

5-to check out write the following command
	$ sudo ls -l /dev/spi*
the output will be similar to 
crw-rw---- 1 root spi 153, 1 Feb 23 13:17 /dev/spidev0.0
crw-rw---- 1 root spi 153, 2 Feb 23 13:17 /dev/spidev0.1
crw-rw---- 1 root spi 153, 0 Feb 23 13:17 /dev/spidev1.0
crw-rw---- 1 root spi 153, 0 Feb 23 13:17 /dev/spidev1.1

for stpm33 we need only spidev0.0


                    --------------------------------------------                            
           
                         -----------------------
                        |  Building & Running   |
                         -----------------------  

1-open the terminal and follow the path then type
	$ make
	
2-then type to excute
	$ ./metering.out

3-system will run in the background as it's a deamon to check it out use the following commands
	$ ps -eo 'tty,pid,comm' | grep^?

4-you will find metering.out in the list with corresponeding pid number which can be used to control this daemon like termination and pausing 
to terminate the daemon
	$ kill <pid_no>


                    --------------------------------------------                            
thank you 







