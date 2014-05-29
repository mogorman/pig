#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>


#include "small_ssd1306.h"

static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,
0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xFF,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00,
0x80, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x8C, 0x8E, 0x84, 0x00, 0x00, 0x80, 0xF8,
0xF8, 0xF8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80,
0x00, 0xE0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xC7, 0x01, 0x01,
0x01, 0x01, 0x83, 0xFF, 0xFF, 0x00, 0x00, 0x7C, 0xFE, 0xC7, 0x01, 0x01, 0x01, 0x01, 0x83, 0xFF,
0xFF, 0xFF, 0x00, 0x38, 0xFE, 0xC7, 0x83, 0x01, 0x01, 0x01, 0x83, 0xC7, 0xFF, 0xFF, 0x00, 0x00,
0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0xFF, 0xFF, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7F, 0xFF,
0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xFF,
0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x8F,
0x8F, 0x9F, 0xBF, 0xFF, 0xFF, 0xC3, 0xC0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x01, 0x03, 0x03, 0x03,
0x03, 0x03, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01,
0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00,
0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x03,
0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

small_ssd1306::small_ssd1306(int8_t MOSI, int8_t CLOCK, int8_t DC, int8_t RESET,
			     int8_t CS, int8_t POWER, int8_t INVERT_SCREEN, int8_t ORIENTATION){
  mosi = MOSI;
  clock = CLOCK;
  dc = DC;
  reset = RESET;
  cs = CS;
  power = POWER;
  invert_screen = INVERT_SCREEN;
  orientation = ORIENTATION;

  pinMode(dc, OUTPUT);
  pinMode(cs, OUTPUT);
  csport      = portOutputRegister(digitalPinToPort(cs));
  cspinmask   = digitalPinToBitMask(cs);
  dcport      = portOutputRegister(digitalPinToPort(dc));
  dcpinmask   = digitalPinToBitMask(dc);

  pinMode(mosi, OUTPUT);
  pinMode(clock, OUTPUT);
  clkport     = portOutputRegister(digitalPinToPort(clock));
  clkpinmask  = digitalPinToBitMask(clock);
  mosiport    = portOutputRegister(digitalPinToPort(mosi));
  mosipinmask = digitalPinToBitMask(mosi);

  pinMode(power, OUTPUT);
}

void small_ssd1306::on(void) {

  pinMode(mosi, OUTPUT);
  pinMode(dc, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(reset, OUTPUT);
 
  digitalWrite(power, LOW); // turn oled on
  delay(20);
  // Setup reset pin direction (used by both SPI and I2C)  

  digitalWrite(reset, HIGH);
  // VDD (3.3V) goes high at start, lets just chill for a ms
  delay(1);
  // bring reset low
  digitalWrite(reset, LOW);
  // wait 10ms
  delay(10);
  // bring out of reset
  digitalWrite(reset, HIGH);
  // turn on VCC (9V?)
    // Init sequence for 128x32 OLED module
  spi_write_command(SSD1306_DISPLAYOFF);                    // 0xAE
  spi_write_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  spi_write_command(0x80);                                  // the suggested ratio 0x80
  spi_write_command(SSD1306_SETMULTIPLEX);                  // 0xA8
  spi_write_command(0x1F);
  spi_write_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  spi_write_command(0x0);                                   // no offset
  spi_write_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
  spi_write_command(SSD1306_CHARGEPUMP);                    // 0x8D
  spi_write_command(0x14); //switchcap
  spi_write_command(SSD1306_MEMORYMODE);                    // 0x20
  spi_write_command(0x00);                                  // 0x0 act like ks0108
  spi_write_command(SSD1306_SEGREMAP | 0x1);
  spi_write_command(SSD1306_COMSCANDEC);
  spi_write_command(SSD1306_SETCOMPINS);                    // 0xDA
  spi_write_command(0x02);
  spi_write_command(SSD1306_SETCONTRAST);                   // 0x81
  spi_write_command(0xCF);
  spi_write_command(SSD1306_SETPRECHARGE);                  // 0xd9
  spi_write_command(0xF1); //switch cap
  spi_write_command(SSD1306_SETVCOMDETECT);                 // 0xDB
  spi_write_command(0x40);
  spi_write_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  if(invert_screen) {
    spi_write_command(SSD1306_INVERTDISPLAY);
  } else {
    spi_write_command(SSD1306_NORMALDISPLAY);
  }
  if(orientation) {
    spi_write_command(SSD1306_SEGREMAP);
    spi_write_command(SSD1306_COMSCANINC);
  }
  spi_write_command(SSD1306_DISPLAYON); //--turn on oled panel

  //yuck?
}
void small_ssd1306::off(void) {
  spi_write_command(SSD1306_DISPLAYOFF);	// put the OLED display in sleep mode
  spi_write_command(0x8D);  // disable charge pump
  spi_write_command(0x10);  // disable charge pump

  delay(10);
  digitalWrite(power, HIGH);

  pinMode(mosi, INPUT);
  pinMode(dc, INPUT);
  pinMode(cs, INPUT);
  pinMode(clock, INPUT);
  pinMode(reset, INPUT);

  digitalWrite(reset, LOW);
  digitalWrite(mosi, LOW);
  digitalWrite(dc, LOW);
  digitalWrite(cs, LOW);
  digitalWrite(clock, LOW);
}

void small_ssd1306::invert(void) {
  if(invert_screen) {
    invert_screen=0;
    spi_write_command(SSD1306_NORMALDISPLAY);
  } else {
    invert_screen=1;
    spi_write_command(SSD1306_INVERTDISPLAY);
  }
}
void small_ssd1306::clear(void) {
  memset(buffer, 0, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
}
void small_ssd1306::update(void) {
  spi_write_command(SSD1306_COLUMNADDR);
  spi_write_command(0);   // Column start address (0 = reset)
  spi_write_command(127); // Column end address (127 = reset)

  spi_write_command(SSD1306_PAGEADDR);
  spi_write_command(0); // Page start address (0 = reset)
  spi_write_command(3); // Page end address

  for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++) {
    *csport |= cspinmask;
    *dcport |= dcpinmask;
    *csport &= ~cspinmask;
      for(uint8_t bit = 0x80; bit; bit >>= 1) {
    	*clkport &= ~clkpinmask;
    	if(buffer[i] & bit) *mosiport |=  mosipinmask;
    	else        *mosiport &= ~mosipinmask;
    	*clkport |=  clkpinmask;
    } 
    *csport |= cspinmask;
  }


}

void small_ssd1306::spi_write_command(uint8_t data) {
    *csport |= cspinmask;
    *dcport &= ~dcpinmask;
    *csport &= ~cspinmask;

    for(uint8_t bit = 0x80; bit; bit >>= 1) {
       *clkport &= ~clkpinmask;
           if(data & bit) *mosiport |=  mosipinmask;
           else        *mosiport &= ~mosipinmask;
           *clkport |=  clkpinmask;
         } 
    *csport |= cspinmask;
}

void small_ssd1306::spi_write_data(uint8_t data) {
    *csport |= cspinmask;
    *dcport |= dcpinmask;
    *csport &= ~cspinmask;

     for(uint8_t bit = 0x80; bit; bit >>= 1) {
       *clkport &= ~clkpinmask;
       if(data & bit) *mosiport |=  mosipinmask;
       else        *mosiport &= ~mosipinmask;
       *clkport |=  clkpinmask;
    }
    *csport |= cspinmask;
}
