

#ifndef _SPI_H_
#define _SPI_H_

/**
pinout for RPI
SCLK    11	//gmle
MOSI    10  //purple -> 15
MISO    9	 //yellow ->  17
* 
* #define STPM33_CS_Pin 2 //blue -> 7
//#define STPM33_CS_GPIO_Port GPIOE

#define STPM33_SYN_Pin 4 //green -> 5
//#define STPM33_SYN_GPIO_Port GPIOE

#define STPM33_EN_Pin 5 //white 3
//#define STPM33_EN_GPIO_Port GPIOE

white enable
* 
**/

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

//#include <linux/time.h>
//#include <linux/delay.h>
//#include <wiringPi.h>


/**********************************************************
Declare Global Variables
***********************************************************/

//#define cs 8
extern int spi_fd;
typedef struct{
 const char *device ;
 uint8_t mode;
 uint8_t bits_per_word;
 uint32_t speed;
//set lsb_msb to 0 to set the first bit t MSB
 uint8_t lsb_msb;
 uint16_t delay;
}spidev_t;

void sunxi_spi_init(char filename[40]);
int spi_init(spidev_t* spix);
int spi_txrx(unsigned char txData);
int spi_read_config(void);
uint8_t spi_txrx_len(const uint8_t *txData ,uint32_t length);

#endif
