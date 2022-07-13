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
//#include "../../ipc/msg_queue.h"
#include "../../config.h"
//server
#define URL_PATH  "https://rooot.azurewebsites.net/reading/post"
//local db
#include <sqlite3.h>
#include <stdio.h>


/**
function to read the readings from text file line by line and send each reading to the backend
json formate:
{
   "meterId":1,
   "date":"2014-02-11",
   "volt" : 10,
   "electric_current" : 10, 
   "energy" : 100
}
*/

sqlite3 *db;
char *err_msg = 0;
int db_rc;
#define meterId 40
#define no_of_rows_to_keep 5
long total_rows_no;

int connect_to_db()
{
    db_rc = sqlite3_open("/home/pi/Desktop/meter/localdB.litedb", &db);
   
     if (db_rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }	
    return 0;
}
int delete_readings_from_db()
{	
	//connect to db
	int db_code = connect_to_db();
	if(db_code == 0)
	{
		printf("Deletion:\nconnection to db succeeded\n");
	}
	else{
		printf("Deletion:\nconnection to db failed\n");
		return 1;
	}
	if(total_rows_no>no_of_rows_to_keep)
	{
		//prepare deletion 
		char *delete_last_nrow_sql = "DELETE FROM READINGS WHERE ROWID IN (SELECT ROWID FROM READINGS ORDER BY ROWID DESC LIMIT?)";
		sqlite3_stmt*delete_stmt;
		sqlite3_prepare_v2(db,delete_last_nrow_sql, -1, &delete_stmt, 0);
		sqlite3_bind_int(delete_stmt, 1,total_rows_no-no_of_rows_to_keep);
		//perform deletion
		sqlite3_step(delete_stmt);	
		//error
	   	if (db_rc != SQLITE_OK ) {
	        	printf("error while deleting\n");
		}
		printf("row deleted successfully\n");
	}//end if
	else
	{
		printf("no of readings < min no to delete\n");
	}
	//close connection to db
	sqlite3_close(db);
}	
int send_readings_to_server()
{
	int db_code = connect_to_db();
	if(db_code == 0)
	{
		printf("connection to db succeeded\n");
	}
	else{
		printf("connection to db failed\n");
		return 1;
	}
	//get the total no of readings
	char *select_stmt_sql = "SELECT COUNT(*) FROM READINGS";
	sqlite3_stmt*select_stmt;
	sqlite3_prepare_v2(db, select_stmt_sql, -1, &select_stmt, 0);
	sqlite3_step(select_stmt);
	total_rows_no = sqlite3_column_int64(select_stmt,0);
	//must finalize to prevent database locking
	sqlite3_finalize(select_stmt);
	if(total_rows_no>no_of_rows_to_keep)
	{
		//read from sqlite local db
		//SELECT * FROM READINGS ORDER BY ROWID ASC LIMIT 1
		char *select_last_row_sql = "SELECT * FROM READINGS WHERE ROWID IN (SELECT ROWID FROM READINGS ORDER BY ROWID DESC LIMIT?)";
		sqlite3_stmt* stmt;
		sqlite3_prepare_v2(db,select_last_row_sql, -1, &stmt, 0);
		sqlite3_bind_int64(stmt, 1,total_rows_no-no_of_rows_to_keep);
		//get last (total_rows_no - no_of_rows_to_keep) rows from db
		while (sqlite3_step(stmt) == SQLITE_ROW) {
		//sqlite3_step(stmt);
			int id  = sqlite3_column_int(stmt,0);		//id
			double volt = sqlite3_column_double(stmt,1);	//volt
			double current = sqlite3_column_double(stmt,2);	//current
			double energy = sqlite3_column_double(stmt,3);	//energy
			double power = sqlite3_column_double(stmt,4);	//power
			const unsigned char* time_stamp = sqlite3_column_text(stmt,5);	//time_stamp	
		//send readings to the server
		char buf[600];
		CURL *curl;
		CURLcode res;
		const char*  apiSendReadingUrl = URL_PATH;
	 	curl_global_init(CURL_GLOBAL_ALL);

		curl = curl_easy_init();

		if(curl) {

		      snprintf(buf, sizeof(buf), "{\"meterId\":%d,\"date\":\"%s\",\"volt\" : %lf,\"electric_current\" :%lf, \"energy\" : %lf,\"activePower\":%lf}\n",meterId, "k", volt, current, energy, power);
		      printf("\nRequest to server: %s\n",buf);
		      //form Request
		      const char* jsonObj = buf;
		      struct curl_slist *headers = NULL;
		      //form header
		      headers = curl_slist_append(headers, "Accept: application/json");
		      headers = curl_slist_append(headers, "Content-Type: application/json");
		      headers = curl_slist_append(headers, "charset: utf-8");

		      curl_easy_setopt(curl, CURLOPT_URL, apiSendReadingUrl);

		      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);

		      curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");
		      //send request
		      res = curl_easy_perform(curl);
		      //check response
		      if(res != CURLE_OK) {
			  fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
		      }
		      //delete_from_db();
		}//end if curl
		curl_global_cleanup();

	   	if (db_rc != SQLITE_OK ) {
	        
		        fprintf(stderr, "SQL error: %s\n", err_msg);
		        
		        sqlite3_free(err_msg);        
		        sqlite3_close(db);
	        
	        //return 1;
	   	} //end if
		printf("response: \n");
	    }//end while
	}
	else
	{
		printf("no of readings < the min no to send to cloud\n");
	}
	//close db connection
	sqlite3_close(db);
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
    //openlog ("firstdaemon", LOG_PID, LOG_DAEMON);

}

int main()
{
#ifdef DAEMON_ON
    skeleton_daemon();
#endif

    while (1)
    {
        //syslog (LOG_NOTICE, "Sending readings daemon started.");
				send_readings_to_server();
				delete_readings_from_db();
				printf("go to sleep\n");
				sleep(30);
				printf("After sleep : saba7 el 5eer\n");
	//			break;
    }

    //syslog (LOG_NOTICE, "Sending readings daemon terminated.");
    //closelog();

    return EXIT_SUCCESS;
}
