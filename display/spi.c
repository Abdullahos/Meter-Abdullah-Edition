/**
pinout for RPI
SCLK    11
MOSI    10
MISO    9
SS      8 but you can change it as you prefer in this code we will connect SS PIN to ground
**/
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
 
#include "spi.h"

int spi_fd;


void sunxi_spi_init(char filename[40])
    {
        int file;
    __u8    mode = SPI_MODE_0 , lsb = 0, bits;
    __u32 speed=2500000;
 
        if ((file = open(filename,O_RDWR)) < 0)
        {
            printf("Failed to open the bus.");
            /* ERROR HANDLING; you can check errno to see what went wrong */
            exit(1);
            }
 
        ///////////////
        // Verifications
        ///////////////
        //possible modes: 
        //mode |= SPI_LOOP; 
       // mode |= SPI_CPHA; 
     //   mode |= SPI_CPOL; 
     //   mode |= SPI_LSB_FIRST; 
        //mode |= SPI_CS_HIGH; 
        //mode |= SPI_3WIRE; 
     //   mode |= SPI_NO_CS; 
   //     mode |= SPI_READY;
        //multiple possibilities using |
        
            if (ioctl(file, SPI_IOC_WR_MODE, &mode)<0)   {
                perror("can't set spi mode");
                return;
                }
        
 
            if (ioctl(file, SPI_IOC_RD_MODE, &mode) < 0)
                {
                perror("SPI rd_mode");
                return;
                }
            if (ioctl(file, SPI_IOC_RD_LSB_FIRST, &lsb) < 0)
                {
                perror("SPI rd_lsb_fist");
                return;
                }
        //sunxi supports only 8 bits
        /*
            if (ioctl(file, SPI_IOC_WR_BITS_PER_WORD, (__u8[1]){8})<0)   
                {
                perror("can't set bits per word");
                return;
                }
        */
            if (ioctl(file, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) 
                {
                perror("SPI bits_per_word");
                return;
                }
        /*
            if (ioctl(file, SPI_IOC_WR_MAX_SPEED_HZ, &speed)<0)  
                {
                perror("can't set max speed hz");
                return;
                }
        */
            if (ioctl(file, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) 
                {
                perror("SPI max_speed_hz");
                return;
                }
     
 
    printf("%s: spi mode %d, %d bits %sper word, %d Hz max\n",filename, mode, bits, lsb ? "(lsb first) " : "", speed);
 
 
    spi_fd =  file;
 }
 
 
int spi_init(spidev_t* spix)
{
	int ret = 0;
	spi_fd = open(spix->device, O_RDWR);
	if (spi_fd < 0)
		printf("can't open device");
	/*
	 * spi mode
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &spix->mode);
	if (ret == -1)
		printf("can't set spi mode");
	  /**
  	    * spi max speed hz
            */
 	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spix->speed);
	if (ret == -1)
		printf("can't set max speed hz");
	/*
	 * bits per word
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spix->bits_per_word);
	if (ret == -1)
		printf("can't set bits per word");
	/*
	 *first bit is MSB when lsb_msb = 0
	 */
	ret = ioctl(spi_fd , SPI_IOC_WR_LSB_FIRST , &spix->lsb_msb);
	if(ret == -1)
		printf("can't set First BIT to MSB or LSB");
 
	return ret;

}


int spi_txrx(unsigned char txData)
{
  unsigned char rxData;
  int status;
  struct spi_ioc_transfer spi;
  memset (&spi, 0, sizeof (spi));
  spi.tx_buf        = (unsigned long)&txData;
  spi.rx_buf        = (unsigned long)&rxData;
  spi.len           = 1;
//  spi.bits_per_word  = 8;
//enabel cs
  status = ioctl (spi_fd, SPI_IOC_MESSAGE(spi.len), &spi);
//disable cs
  return rxData;
}

uint8_t spi_txrx_len(const uint8_t *txData ,uint32_t length)
{
  uint8_t rxData[length];
  int status;
  struct spi_ioc_transfer spi = {
	.tx_buf        = (unsigned long)txData,
  	.rx_buf        = (unsigned long)rxData,
  	.len           = length,
	//.bits_per_word = 8,
//       	.delay_usecs = 10,
    	//.speed_hz = 8000000,
  };
  
//printf("spi.len = %d\n",spi.len);
  status = ioctl (spi_fd, SPI_IOC_MESSAGE(1), &spi);
//disable cs
	if(status < 1 )     printf("Can't send SPI message \n");
  return status;
}

 int spi_read_config(void)
 {
  int8_t rd_mode;
  uint8_t rd_bits;
  uint32_t rd_speed;
  uint16_t delay;
  uint8_t rd_lsb_msb;  //MSB first
	int ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &rd_mode);
	if (ret == -1)
		printf("can't get spi mode");
	ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &rd_bits);
	if (ret == -1)
		printf("can't get bits per word");
  	ret = ioctl(spi_fd , SPI_IOC_RD_LSB_FIRST , &rd_lsb_msb );	
	ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &rd_speed);
	if (ret == -1)
	printf("can't get max speed hz");
	printf("spi mode: %d\n", rd_mode);
	printf("bits per word: %d\n", rd_bits);
	printf("max speed: %d Hz (%d KHz)\n", rd_speed, rd_speed/1000);
return ret; 
}


