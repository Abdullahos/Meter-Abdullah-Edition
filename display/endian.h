#ifndef _ENDIAN_H_
#define _ENDIAN_H_
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define ARRAY_SIZE(ARR)		(sizeof(ARR)/sizeof(ARR[0]))

// calling example
// memset16_safe(ptrToArr , int16_var , ARRAY_SIZE(ptrToArr) );
void memset16_safe(uint16_t *s, uint16_t c, size_t n);

void swap(uint8_t *x , uint8_t *y);
// calling example
// convert_to_big_endiann(arr , ARRAY_SIZE(arr));
void convert_to_big_endiann(uint16_t *arr , uint16_t length);


#endif
