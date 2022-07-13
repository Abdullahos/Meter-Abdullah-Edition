////////HeaderFile////////////////

/////Includes Lib ///////////////
#ifndef _MSG_QUEUE_
#define _MSG_QUEUE_
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#define sizeOfMessage 500

///////Functions //////////////

void sendMsg(char *msg,int len,int id);
char* receive(int id);
void receiveMsg(char * str , int id);

#endif 

