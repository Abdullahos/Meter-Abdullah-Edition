#ifndef _PINOUT_DEFINITION_H_
#define _PINOUT_DEFINITION_H_

/*
  we will use spidev1.0 for TFT screen
					--------------------------------------------                            

                         -----------------------
                        |  RaspberryPi pinout   |
                         -----------------------                                                     

Note that 
	we have to use spi0.x (for example: spidev0.0) for stpm33 as spi0.x can only select SPI_MODE_3 
	which is needed for stpm33 unlike spi1.x which can't apply SPI_MODE_3 

spi0.0 pinout in RaspberryPi
 * SCLK    11	    
 * MOSI    10        
 * MISO    9	   


spi1.1 pinout in RaspberryPi
 * SCLK -> gpio21  
 * MOSI -> gpio20 
 * MISO -> gpio19  
 
*/

// SPI TFT module connections

// *** If software SPI module is used, comment this line OUT ***

#define TFT_SPI_HARDWARE

// *******************************************
// please select neither RPI_ZERO or MANGO_PI
//#define MANGO_PI
  #define RPI_ZERO
/**************** GPIO ***************/
#define RIGHT_BUTTON	  5
#define LEFT_BUTTON	    15

#define SPI_BUS		"/dev/spidev1.0"

#ifdef RPI_ZERO
//for raspberrypi
#define DC_TFT		24
#define RST_TFT 	25
#define CS_TFT		18
#ifndef TFT_SPI_HARDWARE  
//for raspberrypi		
#define SDIN_TFT	20
#define SCLK_TFT  	21
#endif
#endif

#ifdef MANGO_PI
//for mangopi
#define DC_TFT		132	//PE4
#define RST_TFT 	133	//PE5 
#define CS_TFT		129 	//PE1
#ifndef TFT_SPI_HARDWARE  
//software spi for mangopi
#define SDIN_TFT	130  //PE2
#define SCLK_TFT  	131 //PE3
#endif
#endif


#endif
