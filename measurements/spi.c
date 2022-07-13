/**
pinout for RPI
SCLK    11
MOSI    10
MISO    9
SS      8 but you can change it as you prefer in this code we will connect SS PIN to ground
**/

#include "spi.h"

int spi_fd;
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


