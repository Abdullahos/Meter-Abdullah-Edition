#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <curl/curl.h>
#include <string.h>
//#include "MQ.h"
#include "../../ipc/msg_queue.h"
#include "../../config.h"
//b #define FILE_SIZE 60000
//b #define FILE_PATH "~/Desktop/meter/server/database/events.txt"
#define URL_PATH "https://rooot.azurewebsites.net/event/post"

/**
function to read the readings from text file line by line and send each reading to the backend
json formate:
{
   "meterId":1,
   "event":5,
   "type":"tampering",
   "date":"2014-02-11"
}
*/
void readReadings(void){
	const char*  apiSendReadingUrl = URL_PATH;
    	char line[256];
	//receiveMsg(line , 90 );
//	strcpy( line , receive(90) );
/*b*/
//	while (receive(90)!=NULL)
//	{
/*b*/
	strcpy(line,receive(90));
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
          printf("%s\n", line);
	  CURL *curl;
	  CURLcode res;
	  curl_global_init(CURL_GLOBAL_ALL);
	  curl = curl_easy_init();
	  if(curl)
	 {
	      const char* jsonObj = line;
	      struct curl_slist *headers = NULL;
	      headers = curl_slist_append(headers, "Accept: application/json");
	      headers = curl_slist_append(headers, "Content-Type: application/json");
	      headers = curl_slist_append(headers, "charset: utf-8");

	      curl_easy_setopt(curl, CURLOPT_URL, apiSendReadingUrl);

	      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);

	      curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");

	      res = curl_easy_perform(curl);

	      if(res != CURLE_OK) 
		{ printf("Request failed: %s\n", curl_easy_strerror(res));	} 
		//else { printf(res);	}
    	}
	curl_global_cleanup();

/*john*/
printf("\n");
//fflush(stdout);
//printf("%s\n" , line);
//printf("%s\n", line);	
/*john*/	
  // 	# }
    	/* may check feof here to make a difference between eof and io failure -- network
       	timeout for instance */
}

static void skeleton_daemon()
{

    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0){
    		  fprintf(stderr, "errrorrrr");
        exit(EXIT_FAILURE);

     }
    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    //chdir("/");

    //Close all open file descriptors
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    /* Open the log file */
   // openlog ("firstdaemon", LOG_PID, LOG_DAEMON);

}

int main()
{

#ifdef DAEMON_ON
    skeleton_daemon();
#endif

  while (1)
   {
      //  syslog (LOG_NOTICE, "Sending readings daemon started.");
				readReadings();
	//			break;
    }

    //syslog (LOG_NOTICE, "Sending readings daemon terminated.");
    //closelog();

    return EXIT_SUCCESS;
}
