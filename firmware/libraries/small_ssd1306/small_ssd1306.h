#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <SPI.h>

#define BLACK 0
#define WHITE 1

#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 32



#define SSD1306_I2C_ADDRESS   0x3C	// 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)

/*=========================================================================
    SSD1306 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1306_128_64  128x64 pixel display

    SSD1306_128_32  128x32 pixel display

    -----------------------------------------------------------------------*/
#define SSD1306_128_32
/*=========================================================================*/

#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 32

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

class small_ssd1306 {

 public:
  small_ssd1306(int8_t MOSI, int8_t CLOCK, int8_t DC, int8_t RESET,
		int8_t CS, int8_t POWER, int8_t INVERT_SCREEN, int8_t ORIENTATION);
  void update();
  void clear();
  void on();
  void off();
  void set_pixel(uint8_t x, uint8_t y, uint8_t color);
  void draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);
  void print(uint8_t x, uint8_t y, char *string);
  void invert();

 private:
  int8_t mosi, clock, dc, reset, cs, power, invert_screen, orientation;
  void spi_write_command(uint8_t data);
  void spi_write_data(uint8_t data);
};
