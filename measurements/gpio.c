#include "gpio.h"

void gpio_init(int gpio_pin , char * gpio_dir)
{
        int  exportfd, directionfd;
	char buf1[4] , buf2[35];
    	exportfd = open("/sys/class/gpio/export" , O_WRONLY);
    if (exportfd < 0)
    {
        printf("Cannot open GPIO to export it\n");
        exit(1);
    }
    snprintf(buf1 ,sizeof(buf1) ,"%d" ,gpio_pin);
    write(exportfd, buf1, 4);
    close(exportfd);
    printf("GPIO exported successfully\n");

    // Update the direction of the GPIO to be an output
	snprintf(buf2 , sizeof(buf2) ,"/sys/class/gpio/gpio%d/direction", gpio_pin);
    //printf(buf2);
    directionfd = open((const char *)buf2 , O_WRONLY);
    if (directionfd < 0)
    {
        printf("Cannot open GPIO direction it\n");
        exit(1);
    }
    write(directionfd,(const char*) gpio_dir, 4);
    close(directionfd);
    printf("GPIO direction set as output successfully\n");

}

char gpio_read(int gpio_pin)
{
char result;
char buf[35];
 snprintf(buf , sizeof(buf) ,"/sys/class/gpio/gpio%d/value",gpio_pin);
 int valuefd = open((const char*)buf, O_RDONLY);
 if (valuefd < 0)
{
        printf("Cannot open GPIO value\n");
        exit(1);
}
        read(valuefd,&result, 4);
        //printf("buf = %c \n", buf);
        close(valuefd);
return result;
}

void gpio_write(int gpio_pin , unsigned char gpio_value)
{
char result;
char buf[35];
char val[4];
 snprintf(buf , sizeof(buf), "/sys/class/gpio/gpio%d/value",gpio_pin);
 int valuefd = open((const char*)buf, O_RDWR);
 if (valuefd < 0)
{
        printf("Cannot open GPIO value\n");
        exit(1);
}
	snprintf(val ,sizeof(val) , "%d",gpio_value);
        write(valuefd,(const char *)val, 4);
        close(valuefd);
}
