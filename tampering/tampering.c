
#include <stdio.h> // for printf
#include <time.h>
#include <stdlib.h> // For exit() function
#include <string.h>

#include "tamper_pinout.h"
#include "gpio.h"
#include "../ipc/msg_queue.h"
#include "../config.h"

//db
#include <sqlite3.h>
#include <stdio.h>
//b #define FILE_PATH "~/Desktop/MeterData-John/meter/server/database/events.txt"
//b FILE *filePointer;

sqlite3 *db;
int db_rc = 0;
char *err_msg = 0;

int connect_to_db()
{
    db_rc = sqlite3_open("/home/pi/Desktop/meter/localdB.litedb", &db);
    if (db_rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    
}
int write_to_db()
{
	//establish connection with db
	int status_no = connect_to_db();
	if(status_no == 0)
	{
		printf("connection to local db established successfully\n");
	}
	else
	{
		printf("Cann't establish connection to db\n");
	}
	//form sql
	char *sql = "INSERT INTO EVENTS (type, time_stamp) VALUES (\"tampering\",datetime());" ;
	db_rc = sqlite3_exec(db, sql, 0, 0, &err_msg);    
   	//error while writting to db
	if (db_rc != SQLITE_OK ) {
    	    fprintf(stderr, "SQL error: %s\n", err_msg);
       	    sqlite3_free(err_msg);        
            sqlite3_close(db);
      
       	    return 1;
   	} 
    
    sqlite3_close(db);
    
    return 0;
}
void Tampring(void)
{
	static int event = 0;
	char message[200];
	char* ctime_no_newline;
	time_t timeStamp = time(NULL);	//Create container to hold time value
	event++;
    ctime_no_newline = strtok(ctime(&timeStamp), "\n");
    
    gpio_write(TAMPER_LED , 1);
    sleep(2);
    gpio_write(TAMPER_LED , 0);

	//DataBase
	//write event to LOCAL db
	int status_code = write_to_db();
	//success
	if(status_code == 0)
	{
		printf("saved to LOCAL database\n");
	}
	else
	{
		printf("error while saving to LOCAL database\n");
	}
	//END DATABASE

	/**** this is the file that will send to server if there is tampring ***/
//b	filePointer = fopen(FILE_PATH,"a");	// W -> For create text file
														// a -> For upadte in exciting file
/**** this is the Events that will be sent to the HES *****/
	snprintf(message , sizeof(message) , "{\"meterId\": 1, \"event\": \"%d\",\"type\":\"tampering\",\"date\":\"%s\"}\n",event , ctime_no_newline);
/*b*/	sendMsg(message,sizeof(message),90);
//b	fprintf(filePointer,message);
//b	fclose(filePointer);
	
	/*********** for debugging ************/
//	printf(message);
}
void skeleton_daemon(void)
{
    pid_t pid, sid;

   //Fork the Parent Process
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }

    //We got a good pid, Close the Parent Process
    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
void MainProcess(void)
{
	char TamperingStatus = '1',TamperingStatusLastVal = '1';
	int c = 0;
	gpio_init(TAMPER_LIMIT_SWITCH,"in");
	gpio_init(TAMPER_LED , "out" );
	gpio_write(TAMPER_LED , 0);
	
		while(1)
		{
			TamperingStatus = gpio_read(TAMPER_LIMIT_SWITCH);
			if(TamperingStatus == '0')
			{
				usleep(10000);		// delay for debouncing
				if(TamperingStatus == '0') //check again if the button still pressed
				{
					if(TamperingStatus != TamperingStatusLastVal) 
					{
						Tampring();
						TamperingStatusLastVal = TamperingStatus;
					}
				}
			}
			else
			{
				TamperingStatusLastVal = TamperingStatus;

			}

		}
	
}

int main()
{
#ifdef DAEMON_ON
    skeleton_daemon();
#endif
    MainProcess();		
    //exit(EXIT_SUCCESS);
	return 0;
}
