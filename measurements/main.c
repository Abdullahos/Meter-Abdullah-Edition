#include <stdio.h> // for printf
#include <time.h>
#include <stdlib.h> // For exit() function
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include "config.h"
#include "pinout_definitions.h"
//#include "st_types.h"
#include "gpio.h"
#include "spi.h"
#include "metroTask.h"
#include "metrology.h"
#include "metrology_hal_spi.h"

#include "../ipc/msg_queue.h"
#include "../config.h"
#include "recordReadings.h"

//db
#include <sqlite3.h>
#include <stdio.h>
//time
#include <sys/time.h>

//b #define DISPLAY_PATH "/home/pi/Desktop/MeterData-John/meter/server/database/DisplayReadings.txt"

#define RX_BUFFER_SIZE 6
#define TX_BUFFER_SIZE 13

#define INSERT_QUEREY "INSERT INTO READINGS (volt,current,energy,power,time_stamp) VALUES"
uint32_t credit_recived = 0;
uint32_t Credit = 0;
uint32_t serial_comm_activate = 0;


volatile uint8_t RxData[RX_BUFFER_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t TxData[TX_BUFFER_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, '\n' };

uint8_t TxData_RelayON[2] = { 0x5a, 0x5a };
uint8_t TxData_RelayOFF[2] = { 0x7a, 0x7a };
uint8_t TxData_CreditReceived[2] = { 0x1a, 0x1a };

/**
 * @brief  The application entry point.
 * @retval int
 */
double Voltage = 0, Current = 0, ActivePower=0, ActiveEnergy=0;
long meterId = 40;
/**
 *write readings to local db
 *arg (volt, current, energy, power)
 * return  0 if ok, 1 if not
*/
int  write_Readings_to_dB(double volt, double current, double energy , double power)
{	
	 sqlite3 *db;
   	 char *err_msg = 0;
   	 int rc = sqlite3_open("/home/pi/Desktop/meter/localdB.litedb", &db);
    
   	//connection failed to db
	 if (rc != SQLITE_OK) {
        	fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        	sqlite3_close(db);
        return 1;
    	}
	//conncetion succeeded
	char sql_buf[600];
	char *sql = sql_buf;
	//form querey
	snprintf(sql_buf, sizeof(sql_buf),"INSERT INTO READINGS (volt,current,energy,power,time_stamp) VALUES(%lf,%lf,%lf,%lf,datetime());",volt, current, energy, power);
	//char *sql = "INSERT INTO READINGS (volt,current,energy,power,time_stamp) VALUES(10,10,10,10,\"\");";
	//debugging
	printf("%s",sql);
	printf("\n");
	
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
        if (rc != SQLITE_OK ) {
        
    	    fprintf(stderr, "SQL error: %s\n", err_msg);
        
       	    sqlite3_free(err_msg);        
            sqlite3_close(db);
        
        return 1;
    } 
    
    sqlite3_close(db);
    return 0;

}
void RecordReadings(void)
{
	char buf[120] , buf2[100];
	time_t now = time(NULL);
	char* ctime_no_newline;
	ctime_no_newline = strtok(ctime(&now), "\n"); //string to the timestamp
	//abdulla
	//save readings to localdB into table Readings
	int op_res = write_Readings_to_dB(Voltage, Current, ActiveEnergy, ActivePower);
	if(op_res == 0)
	{
		printf("Readings saved to localdB\n");
	}
	else{
		printf("error while writting to the localdB\n");	
	}
	
	//send Message to senderToServer
	//snprintf(buf , sizeof(buf) , "{\"meterId\":\"%d\",\"date\":\"%s\",\"volt\" : %lf,\"electric_current\" : %lf,\"energy\" : %lf,\"activePower\": %lf}\n",meterId,ctime_no_newline,Voltage,Current,ActiveEnergy,ActivePower);
	//sendMsg(buf,sizeof(buf),65);
	//send Message to Display
	snprintf(buf2 , sizeof(buf2) , "\"meterId,volt,current,energy,power:%d,%lf,%lf,%lf,%lf\"\n",meterId,Voltage,Current,ActiveEnergy,ActivePower);
	sendMsg(buf2,sizeof(buf2),75);

//////////////Save To ML file///////////////////
/*b*/ // 	for(int i=0;i<1050;i++)
/*b*/ //	{writeFile( PATH_ML,buf2);}
	/*********** for debugging ************/
/*b*/	printf("Data To Sender to Server: %s \n",buf);
/*b*/	printf("Data To Display: %s \n",buf2);

}


void MainProcess(void)
{
	char buf[100];
	gpio_init(STPM33_CS_Pin , "out" );
	gpio_init(STPM33_SYN_Pin , "out" );
	gpio_init(STPM33_EN_Pin , "out" );
	//gpio_init(RELAY_OFF_Pin , "out" );
	//gpio_init(RELAY_ON_Pin , "out" );
		
	METRO_Init();
	printf("init done \n");
	sleep(2);


//	while (1) {
	for(int i=0;i<1050;i++)
	{
		Metro_HAL_Set_Latch_device_type(LATCH_SW);
		Metro_HAL_Get_Data_device();
		METRO_Update_Measures();

		Voltage = (double) metroData.rmsvoltage / 1000.0;
		Current = (double) metroData.rmscurrent / 1000.0;
		ActivePower = metroData.powerActive / 1000.0;
		ActiveEnergy = metroData.energyActive;
		
		
		// write the readings into text file
		RecordReadings(); 
		
		sleep(1);
	}
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


int main(void)
{
#ifdef DAEMON_ON
    skeleton_daemon();
#endif
    MainProcess();
    exit(EXIT_SUCCESS);
}



