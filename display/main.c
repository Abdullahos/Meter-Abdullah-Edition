
/*
 * Project Name: TFT Display
 * File: main.c
 * Description:
 * Created By : /Root.
 by john
 */

// ************ libraries **************
//#define _width_TFT 128 // Screen width in pixels
//#define _height_TFT 160 // Screen height in pixels
#include <stdio.h> // for printf
#include <time.h> // for test 11
#include <stdlib.h> // For exit() function
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
// ***********include Files**************

#include "gpio.h"
#include "tftArrImage.h"
#include "ST7735_TFT.h"

#include "Bi_Color_Bitmap.h" // Data for test 11 and 12.
#include "endian.h"
#include "../ipc/msg_queue.h"
#include "pthread.h"
#include "../config.h"
#define RELEASED 	0
#define PRESSED		1

//************GlobalVarible*************************/

//forDisplaying
double volt = 0, current = 0, energy = 0, power = 0;
//ForSwitches
#define SCREEN_NO 4
char bottom1, bottom2;
int8_t f_ptr = 0;
double ptrArr[4] = {1 , 220.2 , 45.5 , 100.0};
pthread_mutex_t  screen_mutex_lock = PTHREAD_MUTEX_INITIALIZER;

void meter_showScreen1(void)
{	
	printf("image_Hello \n");
	tft_drawimage(0,0,127,159,   image_Hello );
}

void meter_showScreen4(void)
{
	printf("Iskraemeco\n");
	tft_drawimage(0,0,127,159 , image_Iskra );
}

void meter_showScreen2(double local_volt , double local_current)
{
	tft_fillscreen(ST7735_BLACK);
	printf("text1 \n");
	TFTsetRotation(3);
	tft_drawtext(4, 5 , 	"Current:" , ST7735_GREEN , ST7735_BLACK , 2);
	float myInt1 = local_current;
	char myStr1[8];
	TFTFontNum(3);
	sprintf(myStr1, "%0.3f", myInt1);
	tft_drawtext(12, 35 , myStr1, ST7735_WHITE, ST7735_BLACK, 2);
	tft_drawtext(4, 65 , 	"Voltage:" , ST7735_GREEN , ST7735_BLACK , 2);
	float  myInt2 = local_volt;
	char myStr2[8];
	sprintf(myStr2, "%0.3f", myInt2);
	tft_drawtext(12, 95, myStr2, ST7735_WHITE, ST7735_BLACK, 2);
	TFTsetRotation(2);
}

void meter_updateScreen2(double local_volt , double local_current)
{
//	TFTFontNum(1);
	double myIntX1=local_current;
	char myStrX1[8];
	sprintf(myStrX1, "%0.3f", myIntX1);
	TFTsetRotation(3);
	tft_fillrec(12, 32,160,25, ST7735_BLACK);
	tft_drawtext(12, 35 , myStrX1,  ST7735_WHITE, ST7735_BLACK, 2);
	float  myIntY1= local_volt;
	char myStrY1[8];
	sprintf(myStrY1, "%0.3f", myIntY1);
	tft_fillrec(12, 92,160,25, ST7735_BLACK);
	tft_drawtext(12, 95, myStrY1, ST7735_WHITE, ST7735_BLACK, 2);
	TFTsetRotation(2);

}

void meter_showScreen3(double local_energy , double local_power)
{
 	tft_fillscreen(ST7735_BLACK);
 	TFTsetRotation(3);
	TFTFontNum(1);
	tft_drawtext(4, 5, "Energy : ",  ST7735_GREEN, ST7735_BLACK, 2);
	float  myInt3 = local_energy;
	char myStr3[8];
	TFTFontNum(3);
	sprintf(myStr3, "%0.3f", myInt3);
	tft_drawtext(12, 35 , myStr3,  ST7735_WHITE, ST7735_BLACK, 2);
	tft_drawtext(4, 65, "Power : ",  ST7735_GREEN, ST7735_BLACK, 2);
	float  myInt4 = local_power;
	char myStr4[8];
	sprintf(myStr4, "%0.3f", myInt4);
	tft_drawtext(12, 95, myStr4, ST7735_WHITE, ST7735_BLACK, 2);
 	TFTsetRotation(2);
}

void meter_updateScreen3(double local_energy , double local_power)
{
//	TFTFontNum(1);
	double myIntX1=local_energy;
	char myStrX1[8];
	sprintf(myStrX1, "%0.3f", myIntX1);
	TFTsetRotation(3);
	tft_fillrec(12, 32,160,25, ST7735_BLACK);
	tft_drawtext(12, 35 , myStrX1, ST7735_WHITE, ST7735_BLACK, 2);
	float  myIntY1= local_power;
	char myStrY1[8];
	sprintf(myStrY1, "%0.3f", myIntY1);
	tft_fillrec(12, 92,160,25, ST7735_BLACK);
	tft_drawtext(12, 95, myStrY1, ST7735_WHITE, ST7735_BLACK, 2);
	TFTsetRotation(2);

}

void Setup(void)
{
 	//bcm2835_delay(TEST_DELAY2);
	printf("TFT Setup \n");
	gpio_init(CS_TFT , "out");
#ifndef TFT_SPI_HARDWARE
	printf("SPI SOFTWARE \n");
	TFT_Setup(RST_TFT, DC_TFT ,CS_TFT , SCLK_TFT, SDIN_TFT);
#else
	printf("SPI HARDWARE \n");
	TFT_Setup(RST_TFT, DC_TFT ,CS_TFT , -1, -1);
#endif
	printf("TFT_RedTab_Initialize() \n");
	TFT_RedTab_Initialize();
	printf("init done successfully \n");
	TFTsetRotation(2);
	convert_to_big_endiann(image_Hello , ARRAY_SIZE(image_Hello));
	convert_to_big_endiann(image_Root1 , ARRAY_SIZE(image_Root1));
	convert_to_big_endiann(image_Root2 , ARRAY_SIZE(image_Root2));
	convert_to_big_endiann(image_Iskra , ARRAY_SIZE(image_Iskra));
}
 


void Start(void)
{
		printf("image_Hello \n");
		tft_drawimage(0,0,127,159,   image_Hello );
		sleep(1);
		printf("image_Iskra \n");
		tft_drawimage(0,0,127,159,   image_Iskra );
		sleep(1);
		printf("imageRoot2 \n");
		tft_drawimage(0,0,127,159,   image_Root2 );
		sleep(1);
		printf("black \n");
		tft_fillscreen(ST7735_BLACK);
	//	sleep(1);
	//	printf("Screen2\n");
	//	meter_showScreen2(100.2,10.55);
	//	sleep(1);
	//	printf("Screen3\n");
	//	meter_showScreen3(55.5,1000);
	//	sleep(1);

}

void parseMetering(double *arr)
{
	/********************OpenAndReadFirstLine***************/
	printf("start parsing data\n");
	char aSingleLine[200];
	//char *received_data = receive(75);
//***************RECIEVING***************************/
//	if(received_data != NULL)
	receiveMsg(aSingleLine , 75);	
	printf("the msg or data that we have recieve is : %s \n ",aSingleLine );
	/****************ConvertAndParseAlgorithm*******************/
		char* lastColon = strrchr(aSingleLine,':');
		if (lastColon != NULL)
		{
			printf("Value of lastcolon = %c\n" , *lastColon);
			char* numbers = lastColon+1; // one after the last colon;
			int i=0;
			char *numStr = strtok(numbers,",");
		//	printf("numStr = %s\n" , numStr);
			printf("int value = %d \n" , atof(numStr) );
			while(numStr != NULL)
			{
				printf("numstr = %s\n" , numStr);
				arr[i++]=atof(numStr);
						numStr = strtok(NULL,",");
			}
		}
		printf("In Parse: meterID: %d , volt: %0.3f, current: %0.3f, Energy: %0.3f, Power: %0.3f\n", (int)arr[0], arr[1], arr[2], arr[3], arr[4]);
	printf("end parsing data\n");

}


void *screen_update_thread(void *args)
{
	while(1)
	{
		parseMetering(ptrArr);
		volt 		=	ptrArr[1];
		current 	= 	ptrArr[2];
		energy		= 	ptrArr[3];
		power 		= 	ptrArr[4];
		pthread_mutex_lock(&screen_mutex_lock);
		switch(f_ptr)
		{
			case 2:
				meter_updateScreen2(volt , current);
			break;
			case 3:
				meter_updateScreen3(energy, power);
			break;
		}
		pthread_mutex_unlock(&screen_mutex_lock);
		usleep(100000);
	}

}



void *screen_show_thread(void *args)
{
	while(1)
	{
		if(bottom1 == PRESSED)
		{
			f_ptr++;
			if(f_ptr > SCREEN_NO)	{f_ptr = SCREEN_NO;}
			pthread_mutex_lock(&screen_mutex_lock);
			switch(f_ptr)
			{
				case 1:
					meter_showScreen1();
				break;
				case 2:
					meter_showScreen2(volt , current);
				break;
				case 3:
					meter_showScreen3(energy , power);
				break;
				case 4:
					meter_showScreen4();
				break;
			}
			pthread_mutex_unlock(&screen_mutex_lock);
			bottom1 = RELEASED;
		}


		if(bottom2 == PRESSED)
		{
			f_ptr--;
			if(f_ptr < 1 )			{f_ptr =1;}
			pthread_mutex_lock(&screen_mutex_lock);
			switch(f_ptr)
			{
				case 1:
					meter_showScreen1();
				break;
				case 2:
					meter_showScreen2(volt , current);
				break;
				case 3:
					meter_showScreen3(energy , power);
				break;
	                   	case 4:
                                        meter_showScreen4();
                                break;

			}
			pthread_mutex_unlock(&screen_mutex_lock);
			bottom2 = RELEASED;
		}
	}
}

void *button_states_thread(void *args)
{
	static char right_button_current_state , right_button_last_state;
	static char left_button_current_state , left_button_last_state;

	while(1)
	{
		right_button_current_state 	= gpio_read(	RIGHT_BUTTON	);
		left_button_current_state 	= gpio_read(	LEFT_BUTTON		);

		if( (right_button_current_state != right_button_last_state) && (right_button_last_state == '1') )
		{
		//	pthread_mutex_lock(&mutex_lock);
			bottom1 = PRESSED;
		//	pthread_mutex_unlock(&mutex_lock);
			right_button_last_state =  right_button_current_state;
		}
		else	{ right_button_last_state = right_button_current_state; }

		if( (left_button_current_state != left_button_last_state) && (left_button_last_state == '1') )
		{
		//	pthread_mutex_lock(&mutex_lock);
			bottom2 = PRESSED;
		//	pthread_mutex_unlock(&mutex_lock);
			left_button_last_state =  left_button_current_state;
		}
		else	{ left_button_last_state = left_button_current_state; }		
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
	pthread_t button_states_thread_id , screen_show_thread_id  , screen_update_thread_id;
#ifdef DAEMON_ON
	skeleton_daemon();
#endif
	gpio_init( LEFT_BUTTON  , "in" );
	gpio_init( RIGHT_BUTTON , "in" );
	Setup();
	Start();

///////////////////////////////////////////TESTDisplayByButton///////////////////////////////////////////

	pthread_create(	&button_states_thread_id 	, NULL , button_states_thread 	, NULL);
	pthread_create(	&screen_show_thread_id 		, NULL , screen_show_thread 	, NULL);
	pthread_create(	&screen_update_thread_id 	, NULL , screen_update_thread 	, NULL);

	pthread_join(	button_states_thread_id 	, 	NULL	);
	pthread_join(	screen_show_thread_id 		, 	NULL	);
	pthread_join(	screen_update_thread_id 	, 	NULL	);


}


