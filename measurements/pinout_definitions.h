
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PINOUT_DEF_H
#define __PINOUT_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif
 
/*
pinout & wiring
Warning: Embedded systems is not that easy y 3zezee, So ->
        john is carefull to make you comfortable while wiring

                            -------------------
                           |  jumpers colors   |
                            ------------------- 
         
They're in order from gpio17 to gpio11 no need to trace  the pins & the wires

	white	|	grey	|	purple	|	 3v3	|	blue	|	green	|	orange
	gpio17	|	gpio27	|	gpio22 	| 	none	| 	gpio10	|	gpio9	|	gpio11	

 
                             ------------------
                            |  STPM33 pinout   |
                             ------------------                                                     

    SPI                                                             
                1 NC     3 Gnd   5 SCS   7 NC    9 NC                                        
                2 MOSI   4 MISO  6 SCL   8 SYN   10 Vcc                                  

    Digital IO
                1 Int1      3 Led2      5 Led1      7 Ckin      
                2 Int2      4 En/Rst    6 Gnd       8 Ckout/ZCR
                  

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

   
/* Includes ------------------------------------------------------------------*/
#include "config.h"

//#define SPI_STPM_SPEED   SPI_BAUDRATEPRESCALER_64
#define SPI_STPM_SPEED   1000000


/* GPIOs */
#define STPM33_CS_Pin 17
//#define STPM33_CS_GPIO_Port GPIOE

#define STPM33_SYN_Pin 27
//#define STPM33_SYN_GPIO_Port GPIOE

#define STPM33_EN_Pin 22
//#define STPM33_EN_GPIO_Port GPIOE

//#define RELAY_OFF_Pin 6
//#define RELAY_OFF_GPIO_Port GPIOE

//#define RELAY_ON_Pin 1
//#define RELAY_ON_GPIO_Port GPIOF



   

#ifdef __cplusplus
}
#endif

#endif /* __ST_DEVICE_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
