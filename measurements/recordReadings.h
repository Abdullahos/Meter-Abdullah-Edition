//recordReadings .h

#include <stdio.h> // for printf
#include <stdlib.h> // For exit() function
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define PATH_ML "/home/pi/Desktop/meter/ml/records.txt"

//FILE *filePointer1;
void writeFile(char * FileName , char * DataString);
