//recordReadings .h

#include <stdio.h> // for printf
#include <stdlib.h> // For exit() function
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define PATH "/home/belal-a-aziz/Desktop/RecordInFile/records.txt"

FILE *filePointer;
void writeFile(char * FileName , char * DataString);
