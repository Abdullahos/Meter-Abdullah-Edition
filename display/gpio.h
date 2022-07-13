#ifndef _GPIO_H_
#define _GPIO_H_
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h> //has uint32_t defined


typedef enum
{
	GPIO_PIN_RESET = 0,
	GPIO_PIN_SET
	
}GPIO_PinState;
#define HIGH 1
#define LOW 0

void gpio_init(int gpio_pin , char * gpio_dir);
char gpio_read(int gpio_pin);
void gpio_write(int gpio_pin , unsigned char gpio_value);


#endif
