#include "endian.h"


// calling example
// memset16_safe(ptrToArr , int16_var , ARRAY_SIZE(ptrToArr) );
void memset16_safe(uint16_t *s, uint16_t c, size_t n)
{	
	while(n--) *s++ = c;
}

void swap(uint8_t *x ,uint8_t *y)
{
    uint8_t temp = *x;
    *x = *y;
    *y = temp;
}
void convert_to_big_endiann(uint16_t *arr , uint16_t length)
{
    uint8_t *ptr = (uint8_t *)arr;
    for(uint16_t i=0; i < length; i++)
    {
          swap(ptr , ptr+1);
          ptr+=2;
    }
}
