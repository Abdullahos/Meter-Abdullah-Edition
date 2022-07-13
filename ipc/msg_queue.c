//|||||||||||||||||code||||||||||||||||||||||||//
//////////  (1) MSGQ For Send Data//////////////

#include "msg_queue.h"

struct msg_buffer
{
	long msg_type;
	char msg_txt[sizeOfMessage];
}message;
struct msqid_ds buf;
int num_messages , rc;

void sendMsg(char *msg,int len,int id)
{	for(int i=0 ; i<len;i++)
		message.msg_txt[i]=msg[i] ;
	key_t key=id;
	int msg_id;
	
//	key = ftok("progfile",65);//k=65
	msg_id = msgget(key, 0666 | IPC_CREAT);
	message.msg_type=1;
	
	//printf("please write data to send: \n");
	//fgets(message.msg_txt,500,stdin );


  rc = msgctl(msg_id, IPC_STAT, &buf);
  num_messages = buf.msg_qnum;
  printf("num_messages = %d \n" , num_messages);
  if(num_messages >= 32)
  {
	msgrcv(msg_id,&message,sizeof(message),1,0);  
  }
  	msgsnd(msg_id,&message,sizeof(message),0);
	printf("we have sent : %s \n",message.msg_txt);


}
//|||||||||||||||||code||||||||||||||||||||||||//
//////////  (2)MSGQ For reciving Data///////////////////////
char *receive(int id)
{
	key_t key=id;
	int msg_id;
	static char str[sizeOfMessage];
//	key = ftok("progfile",id);//65
	msg_id = msgget(key, 0666 | IPC_CREAT);

	msgrcv(msg_id,&message,sizeof(message),1,0);
	//printf("the msg or data that we have recieve is : %s \n ",message.msg_txt );
	 strcpy(str,message.msg_txt);
	
	// if you want destroy mqueu
	msgctl(msg_id,IPC_RMID,NULL);
	
	return str;
}



void receiveMsg(char * str , int id)
{
	key_t key=id;
	int msg_id;
//	key = ftok("progfile",id);//65
	msg_id = msgget(key, 0666 | IPC_CREAT);
	msgrcv(msg_id,&message,sizeof(message),1,0);
	//printf("the msg or data that we have recieve is : %s \n ",message.msg_txt );
	 strcpy(str,message.msg_txt);
	// if you want destroy msg queue
	msgctl(msg_id,IPC_RMID,NULL);
	
}

