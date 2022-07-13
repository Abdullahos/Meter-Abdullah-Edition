/*

 * Project Name: TFT Display  

 * File: STT7735_TFT.h

 * Description: library test file  

 * Created By : /Root.

 */

 



#ifndef _st7735_tft_lcd_h

#define _st7735_tft_lcd_h



//#include <bcm2835.h>

#include <stdio.h>

#include <stdint.h>

#include <stdbool.h>

#include <string.h>



#include "ST7735_TFT_Font.h"

#include "pinout_definition.h"

#include "spi.h"
#include "gpio.h"
#include "endian.h"



// ******** USER OPTION 1 PCB_TYPE *********

// PCB type , pick one and one ONLY

#define TFT_PCB_RED

//#define TFT_PCB_GREEN

//#define TFT_PCB_BLACK

//#define  TFT_PCB_GENERIC

// ****************************************



// ******** USER OPTION 2 SPI TYPE ***********

// SPI TFT module connections

// *** If software SPI module is used, comment this line OUT ***

//#define TFT_SPI_HARDWARE

// *******************************************



// *********** DEFINES ****************

// ******** ST7735 registers ********

#define ST7735_NOP     0x00 // Non operation

#define ST7735_SWRESET 0x01 // Soft Reset

#define ST7735_RDDID   0x04

#define ST7735_RDDST   0x09



#define ST7735_SLPIN   0x10 // Sleep ON

#define ST7735_SLPOUT  0x11 // Sleep OFF

#define ST7735_PTLON   0x12 // Partial mode

#define ST7735_NORON   0x13 // Normal Display

#define ST7735_INVOFF  0x20 // Display invert off

#define ST7735_INVON   0x21 // Display Invert on

#define ST7735_DISPOFF 0x28 // Display off

#define ST7735_DISPON  0x29 // Display on



#define ST7735_IDLEON 0x39 //Idle Mode ON

#define ST7735_IDLEOF  0x38 //Idle Mode OFF



#define ST7735_CASET   0x2A // Column address set

#define ST7735_RASET   0x2B // Page address set

#define ST7735_RAMWR   0x2C // Memory write

#define ST7735_RAMRD   0x2E // Memory read

#define ST7735_PTLAR   0x30 // Partial Area

#define ST7735_VSCRDEF 0x33 // Vertical scroll def

#define ST7735_COLMOD  0x3A // Interface Pixel Format

#define ST7735_MADCTL  0x36 // Memory Access Control

#define ST7735_VSCRSADD 0x37 // Vertical Access Control



// Frame Rate Control

#define ST7735_FRMCTR1 0xB1 // Normal

#define ST7735_FRMCTR2 0xB2 // idle

#define ST7735_FRMCTR3 0xB3 // Partial



#define ST7735_INVCTR  0xB4 // Display Inversion control

#define ST7735_DISSET5 0xB6 // Display Function set



// Power_Control

#define ST7735_PWCTR1  0xC0

#define ST7735_PWCTR2  0xC1

#define ST7735_PWCTR3  0xC2

#define ST7735_PWCTR4  0xC3

#define ST7735_PWCTR5  0xC4

#define ST7735_PWCTR6  0xFC



#define ST7735_VMCTR1  0xC5 // VCOM_Control 1



#define ST7735_RDID1   0xDA

#define ST7735_RDID2   0xDB

#define ST7735_RDID3   0xDC

#define ST7735_RDID4   0xDD



#define ST7735_GMCTRP1 0xE0 // Positive Gamma Correction Setting

#define ST7735_GMCTRN1 0xE1 // Negative Gamma Correction Setting



#define ST7735_MADCTL_MY 0x80

#define ST7735_MADCTL_MX 0x40

#define ST7735_MADCTL_MV 0x20

#define ST7735_MADCTL_ML 0x10

#define ST7735_MADCTL_RGB 0x00 //0x08

#define ST7735_MADCTL_BGR 0x08 //0x00

#define ST7735_MADCTL_MH 0x04



// Color definitions 16-Bit Color Values R5G6B5

#define   ST7735_BLACK   0x0000

#define   ST7735_BLUE    0x1F00

#define   ST7735_RED     0x00F8

#define   ST7735_GREEN   0xE007

#define   ST7735_CYAN    0xFF07

#define   ST7735_MAGENTA 0x1FF8

#define   ST7735_YELLOW  0xE0FF

#define   ST7735_WHITE   0xFFFF

#define   ST7735_TAN     0x01ED

#define   ST7735_GREY    0xD19C

#define   ST7735_BROWN   0x0162



// GPIO
#define TFT_DC_SetHigh  gpio_write(_TFT_DC, HIGH)

#define TFT_DC_SetLow  gpio_write(_TFT_DC, LOW)

#define TFT_RST_SetHigh  gpio_write(_TFT_RST, 1)

#define TFT_RST_SetLow  gpio_write(_TFT_RST, 0)

#define TFT_CS_SetHigh gpio_write(_TFT_CS, HIGH) // SW SPI last 6 lines

#define TFT_CS_SetLow gpio_write(_TFT_CS, LOW)

#define TFT_SCLK_SetHigh gpio_write(_TFT_SCLK, HIGH)

#define TFT_SCLK_SetLow  gpio_write(_TFT_SCLK, LOW)

#define TFT_SDATA_SetHigh gpio_write(_TFT_SDATA, HIGH)

#define TFT_SDATA_SetLow  gpio_write(_TFT_SDATA,LOW)



#define TFT_DC_SetDigitalOutput gpio_init(_TFT_DC, "out")

#define TFT_RST_SetDigitalOutput gpio_init(_TFT_RST, "out")

#define TFT_SCLK_SetDigitalOutput gpio_init(_TFT_SCLK, "out") // SW SPI last 3 lines

#define TFT_SDATA_SetDigitalOutput gpio_init(_TFT_SDATA, "out")

#define TFT_CS_SetDigitalOutput gpio_init(_TFT_CS, "out")

//
//#define TFT_DC_SetHigh  bcm2835_gpio_write(_TFT_DC, HIGH)
//
//#define TFT_DC_SetLow  bcm2835_gpio_write(_TFT_DC, LOW)
//
//#define TFT_RST_SetHigh  bcm2835_gpio_write(_TFT_RST, HIGH)
//
//#define TFT_RST_SetLow  bcm2835_gpio_write(_TFT_RST, LOW)
//
//#define TFT_CS_SetHigh bcm2835_gpio_write(_TFT_CS, HIGH) // SW SPI last 6 lines
//
//#define TFT_CS_SetLow bcm2835_gpio_write(_TFT_CS, LOW)
//
//#define TFT_SCLK_SetHigh bcm2835_gpio_write(_TFT_SCLK, HIGH)
//
//#define TFT_SCLK_SetLow  bcm2835_gpio_write(_TFT_SCLK, LOW)
//
//#define TFT_SDATA_SetHigh bcm2835_gpio_write(_TFT_SDATA, HIGH)
//
//#define TFT_SDATA_SetLow  bcm2835_gpio_write(_TFT_SDATA,LOW)
//
//
//
//#define TFT_DC_SetDigitalOutput bcm2835_gpio_fsel(_TFT_DC, BCM2835_GPIO_FSEL_OUTP)
//
//#define TFT_RST_SetDigitalOutput bcm2835_gpio_fsel(_TFT_RST, BCM2835_GPIO_FSEL_OUTP)
//
//#define TFT_SCLK_SetDigitalOutput bcm2835_gpio_fsel(_TFT_SCLK, BCM2835_GPIO_FSEL_OUTP) // SW SPI last 3 lines
//
//#define TFT_SDATA_SetDigitalOutput bcm2835_gpio_fsel(_TFT_SDATA, BCM2835_GPIO_FSEL_OUTP)
//
//#define TFT_CS_SetDigitalOutput bcm2835_gpio_fsel(_TFT_CS, BCM2835_GPIO_FSEL_OUTP)



// Offsets + screen size + Misc

#define TFT_HIGHFREQ_DELAY 1 
//0 //Software SPI delay

#define TFT_ASCII_OFFSET 0x00

#define TFT_ASCII_OFFSET_SP 0x20 //ASCII character for Space, The font table(1-4) starts here

#define TFT_ASCII_OFFSET_NUM 0x2E //offset for  font 5-6

#define _swap(a, b) { int16_t t; t = a; a = b; b = t;}

// ***  USER OPTION 3 SCREEN SECTION ***

#define OFFSET_COL 0  // 2 These offsets can be adjusted for any issues

#define OFFSET_ROW 0  // 3 with manufacture tolerance/defects

#define _width_TFT _width // Screen width in pixels

#define _height_TFT _height // Screen height in pixels

// **********************************************



#ifdef __cplusplus

extern "C" {

#endif



enum ST7735_modes {

    NORMAL = 0, PARTIAL, IDLE, SLEEP, INVERT, DISP_ON, DISP_OFF

}; //0,1,2,3,4,5,6



// ******** FUNCTION  PROTOTYPES ************



// SPI

void TFT_SPI_Initialize(void);

//inline void spiwrite(uint8_t spidata);

void spiwrite(uint8_t);

void write_command(uint8_t);

void write_data(uint8_t);

//void TFT_SPI_Initialize(void);

void TFT_SPIoff(void);

void TFT_PowerDown(void);



// Init routines

void TFT_Setup(int16_t, int16_t, int16_t, int16_t, int16_t);

void TFT_ResetPIN(void);

void Rcmd1(void);

void Rcmd2red(void);

void Rcmd3(void);



#ifdef TFT_PCB_GENERIC

void TFT_ST7735B_Initialize(void);

void Bcmd();

#endif



#ifdef TFT_PCB_GREEN

void TFT_GreenTab_Initialize(void);

void Rcmd2green();

#endif



#ifdef TFT_PCB_BLACK

void TFT_BlackTab_Initialize(void);

#endif



#ifdef TFT_PCB_RED

void TFT_RedTab_Initialize(void);

#endif



// Misc + Screen related

void TFTfillScreen(uint16_t color);

void TFTsetRotation(uint8_t);

void TFTchangeMode(const enum ST7735_modes m);

void setAddrWindow(uint8_t, uint8_t, uint8_t, uint8_t);

void setScrollDefinition(uint8_t top_fix_height_TFT, uint8_t bottom_fix_height_TFT, bool _scroll_direction);

void VerticalScroll(uint8_t _vsp);



// Shapes and lines

void TFTdrawPixel(uint8_t, uint8_t, uint16_t);

void TFTdrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

void TFTdrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color);

void TFTdrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color);



void drawRectWH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);

void fillRectangle(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);



void drawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);

void fillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);



void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);



void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);



// Text

void TFTsetTextWrap(bool w);

void TFTFontNum(uint8_t FontNumber);

void TFTdrawChar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);

void TFTdrawText(uint8_t x, uint8_t y, char *_text, uint16_t color, uint16_t bg, uint8_t size);

void TFTdrawCharNumFont(uint8_t x, uint8_t y, uint8_t c, uint16_t color ,uint16_t bg);

void TFTdrawTextNumFont(uint8_t x, uint8_t y, char *pText, uint16_t color, uint16_t bg);



// Bitmap & Icon

void TFTdrawIcon(uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint16_t bgcolor, const unsigned char character[]);

void TFTdrawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, const unsigned char bitmap[]);

void TFTdrawBitmap24(uint8_t x, uint8_t y, uint8_t *pBmp, char w, char h);

void TFTdrawBitmap16(uint8_t x, uint8_t y, uint8_t *pBmp, char w, char h);

void pushColor(uint16_t color);

int16_t Color565(int16_t ,int16_t , int16_t );



#ifdef __cplusplus

}

#endif



#endif // file header guard



// FROM GITHUB ROTATE FUNC

#ifndef TFT_WIDTH

  #define TFT_WIDTH  128

#endif

#ifndef TFT_HEIGHT

  #define TFT_HEIGHT 160

#endif





// Enumerate the different configurations

#define INITR_GREENTAB       0x0

#define INITR_REDTAB         0x1

#define INITR_BLACKTAB       0x2 // Display with no offsets

#define INITR_GREENTAB2      0x3 // Use if you get random pixels on two edges of green tab display

#define INITR_GREENTAB3      0x4 // Use if you get random pixels on edge(s) of 128x128 screen

#define INITR_GREENTAB128    0x5 // Use if you only get part of 128x128 screen in rotation 0 & 1

#define INITR_GREENTAB160x80 0x6 // Use if you only get part of 128x128 screen in rotation 0 & 1

#define INITR_REDTAB160x80   0x7 // Added for https://www.aliexpress.com/item/ShengYang-1pcs-IPS-0-96-inch-7P-SPI-HD-65K-Full-Color-OLED-Module-ST7735-Drive/32918394604.html

#define INITB                0xB





// Setup the tab color that will be used by the library setRotation() and setup command list

#if defined (ST7735_INITB)

  #define TAB_COLOUR INITB

  

#elif defined (ST7735_GREENTAB)

  #define TAB_COLOUR INITR_GREENTAB

  #define CGRAM_OFFSET

  

#elif defined (ST7735_GREENTAB2)

  #define TAB_COLOUR INITR_GREENTAB2

  #define CGRAM_OFFSET



#elif defined (ST7735_GREENTAB3)

  #define TAB_COLOUR INITR_GREENTAB3

  #define CGRAM_OFFSET



#elif defined (ST7735_GREENTAB128)

  #define TAB_COLOUR INITR_GREENTAB128

  #define CGRAM_OFFSET

 

#elif defined (ST7735_GREENTAB160x80)

  #define TAB_COLOUR INITR_GREENTAB160x80

  #define CGRAM_OFFSET



#elif defined (ST7735_REDTAB160x80)

  #define TAB_COLOUR INITR_REDTAB160x80

  #define CGRAM_OFFSET

 

#elif defined (ST7735_REDTAB)

  #define TAB_COLOUR INITR_REDTAB

  

#elif defined (ST7735_BLACKTAB)

  #define TAB_COLOUR INITR_BLACKTAB



#else // Make sure it is not undefined

  #define TAB_COLOUR INITR_BLACKTAB

#endif





// Color definitions for backwards compatibility with old sketches

// use colour definitions like TFT_BLACK to make sketches more portable

#define ST7735_BLACK       0x0000      /*   0,   0,   0 */

#define ST7735_NAVY        0x0F00      /*   0,   0, 128 */

#define ST7735_DARKGREEN   0x03E0      /*   0, 128,   0 */

#define ST7735_DARKCYAN    0xEF03      /*   0, 128, 128 */

#define ST7735_MAROON      0x0078      /* 128,   0,   0 */

#define ST7735_PURPLE      0x0F78      /* 128,   0, 128 */

#define ST7735_OLIVE       0xE07B      /* 128, 128,   0 */

#define ST7735_LIGHTGREY   0x18C6      /* 192, 192, 192 */

#define ST7735_DARKGREY    0xEF7B      /* 128, 128, 128 */

#define ST7735_BLUE        0x1F00      /*   0,   0, 255 */

#define ST7735_GREEN       0xE007      /*   0, 255,   0 */

#define ST7735_CYAN        0xFF07      /*   0, 255, 255 */

#define ST7735_RED         0x00F8      /* 255,   0,   0 */

#define ST7735_MAGENTA     0x1FF8      /* 255,   0, 255 */

#define ST7735_YELLOW      0xE0FF      /* 255, 255,   0 */

#define ST7735_WHITE       0xFFFF      /* 255, 255, 255 */

#define ST7735_ORANGE      0x20FD      /* 255, 165,   0 */

#define ST7735_GREENYELLOW 0xE5AF      /* 173, 255,  47 */

#define ST7735_PINK        0x1FF8





// Delay between some initialisation commands

#define TFT_INIT_DELAY 0x80





// Generic commands used by TFT_eSPI.cpp

#define TFT_NOP     0x00

#define TFT_SWRST   0x01



#define TFT_CASET   0x2A

#define TFT_PASET   0x2B

#define TFT_RAMWR   0x2C



#define TFT_RAMRD   0x2E

#define TFT_IDXRD   0x00 //0xDD // ILI9341 only, indexed control register read



#define TFT_MADCTL  0x36

#define TFT_MAD_MY  0x80

#define TFT_MAD_MX  0x40

#define TFT_MAD_MV  0x20

#define TFT_MAD_ML  0x10

#define TFT_MAD_BGR 0x08

#define TFT_MAD_MH  0x04

#define TFT_MAD_RGB 0x00



#ifndef TFT_RGB_ORDER

  #if defined(INITR_BLACKTAB) || defined(INITR_GREENTAB2) || defined(INITB)

    #define TFT_MAD_COLOR_ORDER TFT_MAD_RGB

  #else

    #define TFT_MAD_COLOR_ORDER TFT_MAD_BGR

  #endif

#else

  #if (TFT_RGB_ORDER == 1)

    #define TFT_MAD_COLOR_ORDER TFT_MAD_RGB

  #else

    #define TFT_MAD_COLOR_ORDER TFT_MAD_BGR

  #endif

#endif



#define TFT_INVOFF  0x20

#define TFT_INVON   0x21



// ST7735 specific commands used in init

#define ST7735_NOP     0x00

#define ST7735_SWRESET 0x01

#define ST7735_RDDID   0x04

#define ST7735_RDDST   0x09



#define ST7735_SLPIN   0x10

#define ST7735_SLPOUT  0x11

#define ST7735_PTLON   0x12

#define ST7735_NORON   0x13



#define ST7735_INVOFF  0x20

#define ST7735_INVON   0x21

#define ST7735_DISPOFF 0x28

#define ST7735_DISPON  0x29

#define ST7735_CASET   0x2A

#define ST7735_RASET   0x2B // PASET

#define ST7735_RAMWR   0x2C

#define ST7735_RAMRD   0x2E



#define ST7735_PTLAR   0x30

#define ST7735_COLMOD  0x3A

#define ST7735_MADCTL  0x36



#define ST7735_FRMCTR1 0xB1

#define ST7735_FRMCTR2 0xB2

#define ST7735_FRMCTR3 0xB3

#define ST7735_INVCTR  0xB4

#define ST7735_DISSET5 0xB6



#define ST7735_PWCTR1  0xC0

#define ST7735_PWCTR2  0xC1

#define ST7735_PWCTR3  0xC2

#define ST7735_PWCTR4  0xC3

#define ST7735_PWCTR5  0xC4

#define ST7735_VMCTR1  0xC5



#define ST7735_RDID1   0xDA

#define ST7735_RDID2   0xDB

#define ST7735_RDID3   0xDC

#define ST7735_RDID4   0xDD



#define ST7735_PWCTR6  0xFC



#define ST7735_GMCTRP1 0xE0

#define ST7735_GMCTRN1 0xE1



//************************************************ EOF *************************************************

void tft_drawimage(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *image );
void tft_fillrec(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, uint16_t color);
void tft_drawchar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size) ;
void tft_drawtext(uint8_t x, uint8_t y, uint8_t *text, uint16_t color, uint16_t bg, uint8_t size);
void tft_fillscreen(uint16_t color);