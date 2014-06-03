#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

//#include "glcdfont.c"
#include "small_ssd1306.h"

static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] = {//start up screen
0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x1F, 0x1E, 0x3E, 0x40, 0x80, 0x01, 0x01, 0x1E, 0xA1, 0x41, 0x27,
0x3F, 0x3F, 0x3F, 0xFF, 0x7F, 0x7F, 0x7F, 0x7E, 0xFC, 0xF1, 0xE6, 0xC8, 0x10, 0x60, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x1F, 0xC0, 0xFF, 0xFF, 0xDF, 0x00, 0x00, 0x00, 0xFF, 0x00,
0x00, 0x00, 0x06, 0x08, 0x0F, 0x14, 0x20, 0xC0, 0x00, 0x00, 0x01, 0x01, 0x0B, 0x0E, 0x04, 0x06,
0x06, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x06, 0x04, 0x1C, 0x78, 0x30,
0x10, 0x20, 0x20, 0x20, 0xA0, 0xA0, 0xA0, 0x80, 0x80, 0x20, 0x20, 0x20, 0xD0, 0x30, 0x18, 0x00,
0xC0, 0xE0, 0xF0, 0xF0, 0xF0, 0xE0, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0xE0, 0xF0, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x07, 0x20, 0x90, 0x90, 0x90, 0x90, 0xE2, 0x02, 0x06, 0x18, 0x61, 0x01, 0x01, 0x07, 0x06, 0x00,
0xF8, 0xBC, 0x9E, 0x91, 0x82, 0xC4, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x3E, 0xFE, 0xFD,
0xFC, 0xFA, 0xF2, 0xFA, 0xF9, 0xFC, 0x7E, 0xFF, 0x3F, 0x0F, 0x82, 0x40, 0x30, 0x0C, 0x83, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x18, 0x18, 0x30, 0x70, 0xE0, 0xE0, 0xC0, 0x80, 0x00,
0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xC0, 0xE0, 0x70, 0x30, 0x08, 0x04, 0x00,
0x00, 0x00, 0x01, 0x01, 0x00, 0x60, 0x61, 0xC7, 0xC0, 0xE0, 0x38, 0x07, 0x00, 0x80, 0x80, 0xC0,
0xC0, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xC0, 0xC0, 0x80, 0x80, 0x00,
0x07, 0x05, 0x09, 0x07, 0x07, 0x07, 0x08, 0x05, 0x18, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
0x01, 0x03, 0x0F, 0x1F, 0x6F, 0x89, 0x67, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03,
0x0F, 0x1F, 0xF3, 0x91, 0x90, 0x01, 0x08, 0x0C, 0x04, 0x02, 0x01, 0x00, 0x00, 0x63, 0x80, 0x00,
0x0C, 0x1E, 0x1E, 0x0E, 0x00, 0x00, 0x00, 0x70, 0x78, 0x78, 0x30, 0x00, 0x01, 0x03, 0x8F, 0xFF,
0x7C, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF8, 0x3F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x1F,
0x1F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00,
0xC0, 0xE0, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x07,
0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x30, 0xFC, 0x14, 0x02, 0xD2, 0xC2, 0x44, 0xC4, 0x88,
0x08, 0x10, 0x00, 0x20, 0x40, 0x41, 0x02, 0x06, 0x08, 0x08, 0x09, 0x09, 0x18, 0xC2, 0x02, 0x00,
0x04, 0x04, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0C, 0x04, 0x06, 0x03, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x40, 0x30, 0x0E, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x18, 0x3C, 0x7F, 0x7F, 0x7F, 0x3F, 0x1F, 0x1F, 0x3F, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x03, 0x1F, 0x3F, 0x1F, 0x1F, 0x3F, 0x7F, 0x7F, 0x7F, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00
};



static const unsigned char PROGMEM terminus [] = { //13x20 terminus font
0x3F, 0xE0, 0x38, 0x0F, 0xF8, 0x7F, 0xC0, 0x03, 0xFF, 0xFC, 0xFF, 0xDF, 0xFF, 0x3F, 0xE1, 0xFF,
0x00, 0x7F, 0xF0, 0x78, 0x1F, 0xFC, 0xFF, 0xE0, 0x07, 0xFF, 0xFD, 0xFF, 0xDF, 0xFF, 0x7F, 0xF3,
0xFF, 0x80, 0xF0, 0x78, 0xF8, 0x3C, 0x1F, 0xE0, 0xF0, 0x0F, 0xF0, 0x03, 0xC0, 0x1C, 0x07, 0xF0,
0x7F, 0x83, 0xC0, 0xE0, 0x39, 0xF8, 0x38, 0x0F, 0xC0, 0x70, 0x1F, 0xF0, 0x03, 0x80, 0x1C, 0x07,
0xE0, 0x3F, 0x01, 0xC0, 0xE0, 0x39, 0xF8, 0x38, 0x0F, 0xC0, 0x70, 0x3B, 0xF0, 0x03, 0x80, 0x1C,
0x07, 0xE0, 0x3F, 0x01, 0xC0, 0xE0, 0x78, 0x38, 0x38, 0x0E, 0x00, 0x70, 0x73, 0xF0, 0x03, 0x80,
0x1C, 0x0E, 0xE0, 0x3F, 0x01, 0xC0, 0xE0, 0xF8, 0x38, 0x38, 0x0E, 0x00, 0x70, 0xE3, 0xF0, 0x03,
0x80, 0x00, 0x0E, 0xE0, 0x3F, 0x01, 0xC0, 0xE1, 0xF8, 0x38, 0x00, 0x0E, 0x00, 0x71, 0xC3, 0xF0,
0x03, 0x80, 0x00, 0x1C, 0xE0, 0x3F, 0x01, 0xC0, 0xE3, 0xB8, 0x38, 0x00, 0x1C, 0x00, 0xF3, 0x83,
0xFF, 0xF3, 0xFF, 0x80, 0x1C, 0xF0, 0x7F, 0x01, 0xC0, 0xE7, 0x38, 0x38, 0x00, 0x38, 0x3F, 0xE7,
0x03, 0xFF, 0xFB, 0xFF, 0xC0, 0x38, 0x7F, 0xF7, 0x81, 0xC0, 0xEE, 0x38, 0x38, 0x00, 0x70, 0x3F,
0xEE, 0x03, 0x80, 0x3F, 0x81, 0xE0, 0x38, 0x7F, 0xF3, 0xFF, 0xC0, 0xFC, 0x38, 0x38, 0x00, 0xE0,
0x00, 0xFE, 0x03, 0x80, 0x1F, 0x80, 0xE0, 0x70, 0xF0, 0x79, 0xFF, 0xC0, 0xF8, 0x38, 0x38, 0x01,
0xC0, 0x00, 0x7E, 0x03, 0x80, 0x1F, 0x80, 0xE0, 0x70, 0xE0, 0x38, 0x01, 0xC0, 0xF0, 0x38, 0x38,
0x03, 0x80, 0x00, 0x7F, 0xFF, 0x80, 0x1F, 0x80, 0xE0, 0xE0, 0xE0, 0x38, 0x01, 0xC0, 0xE0, 0x38,
0x38, 0x07, 0x00, 0x00, 0x7F, 0xFF, 0x80, 0x1F, 0x80, 0xE0, 0xE0, 0xE0, 0x38, 0x01, 0xC0, 0xE0,
0x38, 0x38, 0x0E, 0x01, 0xC0, 0x70, 0x03, 0x80, 0x1F, 0x80, 0xE0, 0xE0, 0xE0, 0x38, 0x01, 0xC0,
0xE0, 0x38, 0x38, 0x1C, 0x01, 0xC0, 0x70, 0x03, 0xF0, 0x1F, 0x80, 0xE0, 0xE0, 0xE0, 0x38, 0x01,
0xC0, 0xF0, 0x78, 0x38, 0x38, 0x01, 0xE0, 0xF0, 0x03, 0xF8, 0x1F, 0xC1, 0xE0, 0xE0, 0xF0, 0x78,
0x03, 0xC0, 0x7F, 0xF1, 0xFF, 0x3F, 0xFE, 0xFF, 0xE0, 0x03, 0xBF, 0xF9, 0xFF, 0xC0, 0xE0, 0x7F,
0xF3, 0xFF, 0x80, 0x3F, 0xE1, 0xFF, 0x3F, 0xFE, 0x7F, 0xC0, 0x03, 0x9F, 0xF0, 0xFF, 0x80, 0xE0,
0x3F, 0xE3, 0xFF, 0x00
};

static const unsigned char PROGMEM terminus_font [] = { //10x15 terminus font
0x0C, 0x0C, 0xC3, 0x30, 0x30, 0x00, 0x07, 0x00, 0xC0, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFC, 0x0C, 0x0F, 0xC3, 0xF0, 0x03, 0xFF, 0xCF, 0xEF, 0xFC, 0xFC, 0x3F, 0x00,
0x00, 0x00, 0x06, 0x00, 0x18, 0x03, 0xF0, 0x00, 0x3F, 0x3F, 0xC3, 0xF3, 0xFC, 0xFF, 0xFF, 0xF3,
0xF3, 0x03, 0x3F, 0x01, 0xFC, 0x0F, 0x00, 0x00, 0x30, 0x33, 0xF3, 0xFC, 0x3E, 0x3F, 0xC3, 0xF3,
0xFF, 0xC0, 0xF0, 0x30, 0x03, 0x03, 0xC0, 0xFF, 0xF1, 0xF0, 0x00, 0x1F, 0x03, 0x00, 0x00, 0x60,
0x00, 0x30, 0x00, 0x00, 0x03, 0x00, 0x01, 0xF3, 0xFF, 0x00, 0x0C, 0x00, 0xCC, 0x00, 0xF0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0,
0x00, 0x38, 0x30, 0x38, 0x00, 0x00, 0x0C, 0x0C, 0xC3, 0x30, 0x30, 0x71, 0x8D, 0x80, 0xC0, 0x30,
0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x86, 0x1C, 0x18, 0x66, 0x18, 0x07, 0xC0,
0x18, 0x0C, 0x0D, 0x86, 0x61, 0x80, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x06, 0x18, 0xFC, 0x61, 0xB0,
0x66, 0x1B, 0x06, 0xC0, 0x30, 0x06, 0x1B, 0x03, 0x0C, 0x00, 0x6C, 0x1B, 0x00, 0x80, 0x70, 0x36,
0x1B, 0x06, 0x63, 0x30, 0x66, 0x18, 0x30, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC0, 0xC0, 0x31, 0x81,
0x80, 0x03, 0x07, 0x80, 0x00, 0x30, 0x00, 0x30, 0x00, 0x00, 0x03, 0x00, 0x03, 0x06, 0x0F, 0x00,
0x0C, 0x00, 0x0C, 0x00, 0x30, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x60, 0xC0, 0x00, 0x00, 0x60, 0x00,
0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x60, 0x30, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0xC3, 0x30,
0xFC, 0xD9, 0x98, 0xC0, 0xC0, 0x60, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x03,
0x3C, 0x30, 0x3C, 0x0C, 0x0F, 0xC0, 0x30, 0x0C, 0x0F, 0x03, 0xC0, 0xC0, 0x00, 0x00, 0x18, 0x00,
0x06, 0x0C, 0x0D, 0x86, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC0, 0x30, 0x0C, 0x0F, 0x03, 0x0C, 0x00,
0x6C, 0x33, 0x00, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC1, 0xB0, 0x3C, 0x0C, 0x30, 0xC0, 0xF0, 0x3C,
0x0D, 0x86, 0x61, 0x80, 0x31, 0x81, 0x80, 0x03, 0x0C, 0xC0, 0x00, 0x18, 0x00, 0x30, 0x00, 0x00,
0x03, 0x00, 0x03, 0x0C, 0x0F, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x30, 0x00, 0x00, 0x00, 0x03, 0x0C,
0xC0, 0xC0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0xC0, 0x30, 0x06,
0x00, 0x00, 0x0C, 0x0C, 0xC3, 0x31, 0xB6, 0xDB, 0x18, 0xC0, 0xC0, 0x60, 0x06, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x33, 0x03, 0x6C, 0x30, 0x30, 0x0C, 0x1B, 0xC0, 0x30, 0x00, 0x0F, 0x03,
0xC0, 0xC3, 0x00, 0xC0, 0x30, 0x00, 0x03, 0x0C, 0x0F, 0x03, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC0,
0x30, 0x0C, 0x0F, 0x03, 0x0C, 0x00, 0x6C, 0x63, 0x00, 0xE1, 0xF0, 0x3C, 0x0F, 0x03, 0xC1, 0xB0,
0x3C, 0x00, 0x30, 0xC0, 0xF0, 0x3C, 0x0D, 0x86, 0x61, 0x80, 0x31, 0x80, 0xC0, 0x03, 0x18, 0x60,
0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x03, 0x00, 0x03, 0x0C, 0x0F, 0x00, 0x00, 0x01, 0xCC, 0x00,
0x30, 0x00, 0x00, 0x00, 0x03, 0x06, 0xC0, 0xC0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
0xC0, 0xC0, 0x00, 0xC0, 0x30, 0x06, 0x00, 0x00, 0x0C, 0x0C, 0xCF, 0xFD, 0xB0, 0x73, 0x18, 0xC0,
0xC0, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x07, 0x0C, 0x30, 0x30, 0x0C,
0x33, 0xC0, 0x30, 0x00, 0x1B, 0x03, 0xC0, 0xC3, 0x00, 0xC0, 0x60, 0x00, 0x01, 0x8C, 0x0F, 0x1F,
0xC0, 0xF0, 0x3C, 0x03, 0x03, 0xC0, 0x30, 0x0C, 0x03, 0x03, 0x0C, 0x00, 0x6C, 0xC3, 0x00, 0xF3,
0xF8, 0x3C, 0x0F, 0x03, 0xC1, 0xB0, 0x3C, 0x00, 0x30, 0xC0, 0xD8, 0x6C, 0x0C, 0xCC, 0x33, 0x00,
0x61, 0x80, 0xC0, 0x03, 0x30, 0x30, 0x00, 0x00, 0x7F, 0x3F, 0xC3, 0xF0, 0xFF, 0x3F, 0x1F, 0xEC,
0x0F, 0xFC, 0x3C, 0x00, 0xCC, 0x18, 0x30, 0xFF, 0x3F, 0xC3, 0xF3, 0x06, 0xC0, 0xF3, 0xF7, 0xFB,
0xFC, 0xC0, 0xE0, 0x6C, 0x0F, 0x03, 0xC0, 0xFF, 0xF0, 0xC0, 0x30, 0x06, 0x00, 0x00, 0x0C, 0x00,
0x03, 0x31, 0xB0, 0x06, 0x0D, 0x80, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x63, 0x0F, 0x0C, 0x00, 0x30, 0x0C, 0x63, 0xC0, 0x30, 0x00, 0x1B, 0x03, 0xC0, 0xC3, 0x00, 0xC0,
0xC0, 0xFF, 0xC0, 0xC0, 0x1B, 0x33, 0xC0, 0xF0, 0x6C, 0x03, 0x03, 0xC0, 0x30, 0x0C, 0x03, 0x03,
0x0C, 0x00, 0x6D, 0x83, 0x00, 0xDE, 0xFC, 0x3C, 0x0F, 0x03, 0xC1, 0xB0, 0x3C, 0x00, 0x30, 0xC0,
0xD8, 0x6C, 0x0C, 0xCC, 0x33, 0x00, 0xC1, 0x80, 0x60, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0xB0,
0x66, 0x19, 0x83, 0x61, 0x83, 0x0C, 0x0F, 0x06, 0x0C, 0x00, 0xCC, 0x30, 0x30, 0xCD, 0xB0, 0x66,
0x1B, 0x06, 0xC0, 0xF6, 0x0C, 0x0C, 0x60, 0xC0, 0xE0, 0x6C, 0x0F, 0x03, 0xC0, 0xC0, 0x30, 0xC0,
0x30, 0x06, 0x00, 0x00, 0x0C, 0x00, 0x03, 0x31, 0xB0, 0x06, 0x07, 0x00, 0x00, 0xC0, 0x03, 0x18,
0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x63, 0x1B, 0x0C, 0x00, 0x60, 0x18, 0xC3, 0xFF, 0x3F, 0xC0,
0x31, 0x86, 0xC0, 0xC0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0x33, 0x63, 0xC0, 0xFF, 0xCC, 0x03,
0x03, 0xC0, 0x30, 0x0C, 0x03, 0x03, 0x0C, 0x00, 0x6F, 0x03, 0x00, 0xCC, 0xF6, 0x3C, 0x0F, 0x06,
0xC1, 0xB0, 0x66, 0x00, 0x30, 0xC0, 0xD8, 0x6C, 0x0C, 0x78, 0x1E, 0x01, 0x81, 0x80, 0x60, 0x03,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3C, 0x0F, 0x03, 0xC0, 0xC3, 0x0C, 0x0F, 0x03, 0x0C, 0x00,
0xCC, 0x60, 0x30, 0xCC, 0xF0, 0x3C, 0x0F, 0x06, 0xC0, 0xFC, 0x0C, 0x00, 0x60, 0xC0, 0xE0, 0x6C,
0x0D, 0x86, 0xC0, 0xC0, 0x60, 0xC0, 0x30, 0x06, 0x1E, 0x30, 0x0C, 0x00, 0x03, 0x30, 0xFC, 0x0C,
0x0F, 0x30, 0x00, 0xC0, 0x03, 0x0C, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x33, 0x0C, 0x00,
0xC1, 0xF1, 0x83, 0x01, 0xB0, 0x60, 0x30, 0xFC, 0x60, 0xC0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x30,
0x63, 0x63, 0xC0, 0xF0, 0x6C, 0x03, 0x03, 0xFF, 0x3F, 0xCC, 0x7F, 0xFF, 0x0C, 0x00, 0x6E, 0x03,
0x00, 0xC0, 0xF3, 0x3C, 0x0F, 0xFC, 0xC1, 0xBF, 0xC3, 0xF0, 0x30, 0xC0, 0xD8, 0x6C, 0x0C, 0x30,
0x1E, 0x03, 0x01, 0x80, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3C, 0x03, 0x03, 0xC0,
0xC3, 0x0C, 0x0F, 0x03, 0x0C, 0x00, 0xCC, 0xC0, 0x30, 0xCC, 0xF0, 0x3C, 0x0F, 0x06, 0xC0, 0xF8,
0x0C, 0x00, 0x60, 0xC0, 0xF0, 0xCC, 0x0C, 0xCC, 0xC0, 0xC0, 0xC3, 0x80, 0x30, 0x03, 0xB3, 0x30,
0x0C, 0x00, 0x03, 0x30, 0x36, 0x0C, 0x19, 0xB0, 0x00, 0xC0, 0x03, 0x06, 0xC0, 0xC0, 0x00, 0xFF,
0xC0, 0x00, 0xC3, 0x63, 0x0C, 0x01, 0x80, 0x1B, 0x03, 0x00, 0xF0, 0x30, 0x61, 0x86, 0x3F, 0xC0,
0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0xC3, 0x63, 0xFF, 0xF0, 0x3C, 0x03, 0x03, 0xC0, 0x30, 0x0C,
0x0F, 0x03, 0x0C, 0x00, 0x6F, 0x03, 0x00, 0xC0, 0xF1, 0xBC, 0x0F, 0x00, 0xC1, 0xBC, 0x00, 0x18,
0x30, 0xC0, 0xCC, 0xCC, 0xCC, 0x78, 0x0C, 0x06, 0x01, 0x80, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00,
0x3F, 0xF0, 0x3C, 0x03, 0x03, 0xC0, 0xC3, 0x0C, 0x0F, 0x03, 0x0C, 0x00, 0xCD, 0x80, 0x30, 0xCC,
0xF0, 0x3C, 0x0F, 0x06, 0xC0, 0xF0, 0x0C, 0x00, 0x60, 0xC0, 0xF0, 0xCC, 0xCC, 0x78, 0xC0, 0xC1,
0x80, 0xC0, 0x30, 0x06, 0x33, 0x30, 0x0C, 0x00, 0x03, 0x30, 0x36, 0x18, 0x30, 0xE0, 0x00, 0xC0,
0x03, 0x03, 0x80, 0xC0, 0x00, 0x00, 0x00, 0x01, 0x83, 0xC3, 0x0C, 0x03, 0x00, 0x0F, 0x03, 0x00,
0xF0, 0x30, 0x63, 0x03, 0x00, 0xC0, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xC0, 0xC3, 0x63, 0xC0, 0xF0,
0x3C, 0x03, 0x03, 0xC0, 0x30, 0x0C, 0x0F, 0x03, 0x0C, 0x00, 0x6D, 0x83, 0x00, 0xC0, 0xF0, 0xFC,
0x0F, 0x00, 0xC1, 0xB6, 0x00, 0x0C, 0x30, 0xC0, 0xCC, 0xCC, 0xCC, 0xCC, 0x0C, 0x0C, 0x01, 0x80,
0x18, 0x03, 0x00, 0x00, 0x00, 0x00, 0x60, 0xF0, 0x3C, 0x03, 0x03, 0xFF, 0xC3, 0x06, 0x1F, 0x03,
0x0C, 0x00, 0xCF, 0x00, 0x30, 0xCC, 0xF0, 0x3C, 0x0F, 0x0C, 0x60, 0xF0, 0x07, 0xF8, 0x60, 0xC0,
0xD9, 0x8C, 0xCC, 0x30, 0x61, 0xC3, 0x00, 0xC0, 0x30, 0x06, 0x31, 0xE0, 0x00, 0x00, 0x0F, 0xFC,
0x36, 0x18, 0x30, 0x60, 0x00, 0xC0, 0x03, 0x3F, 0xFF, 0xFC, 0x18, 0x00, 0x00, 0x01, 0x83, 0x83,
0x0C, 0x06, 0x00, 0x0F, 0x03, 0x00, 0xF0, 0x30, 0xC3, 0x03, 0x00, 0xC3, 0x00, 0xC0, 0x60, 0xFF,
0xC1, 0x80, 0x03, 0x33, 0xC0, 0xF0, 0x3C, 0x03, 0x03, 0xC0, 0x30, 0x0C, 0x0F, 0x03, 0x0C, 0x30,
0x6C, 0xC3, 0x00, 0xC0, 0xF0, 0x7C, 0x0F, 0x00, 0xC1, 0xB3, 0x00, 0x0C, 0x30, 0xC0, 0xCC, 0xCD,
0xEC, 0xCC, 0x0C, 0x18, 0x01, 0x80, 0x18, 0x03, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0x3C, 0x03,
0x03, 0xC0, 0x03, 0x03, 0xFF, 0x03, 0x0C, 0x00, 0xCD, 0x80, 0x30, 0xCC, 0xF0, 0x3C, 0x0F, 0xF8,
0x3F, 0xF0, 0x00, 0x0C, 0x60, 0xC0, 0xD9, 0x8C, 0xCC, 0x78, 0x3F, 0xC6, 0x00, 0xC0, 0x30, 0x06,
0x00, 0x00, 0x00, 0x00, 0x03, 0x31, 0xB6, 0x33, 0xB0, 0x60, 0x00, 0x60, 0x06, 0x03, 0x80, 0xC0,
0x18, 0x00, 0x00, 0x03, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x0F, 0xFF, 0x00, 0xF0, 0x30, 0xC3, 0x03,
0x00, 0xC3, 0x00, 0xC0, 0x30, 0x00, 0x03, 0x00, 0x03, 0x1F, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC0,
0x30, 0x0C, 0x0F, 0x03, 0x0C, 0x30, 0x6C, 0x63, 0x00, 0xC0, 0xF0, 0x3C, 0x0F, 0x00, 0xC1, 0xB1,
0x80, 0x0C, 0x30, 0xC0, 0xC7, 0x8F, 0x3D, 0x86, 0x0C, 0x30, 0x01, 0x80, 0x0C, 0x03, 0x00, 0x00,
0x00, 0x00, 0xC0, 0xF0, 0x3C, 0x03, 0x03, 0xC0, 0x03, 0x00, 0x0F, 0x03, 0x0C, 0x00, 0xCC, 0xC0,
0x30, 0xCC, 0xF0, 0x3C, 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x0C, 0x60, 0xC0, 0xCF, 0x0C, 0xCC, 0xCC,
0x00, 0xCC, 0x00, 0xC0, 0x30, 0x06, 0x00, 0x00, 0x0C, 0x00, 0x03, 0x30, 0xFC, 0x36, 0xF0, 0xE0,
0x00, 0x60, 0x06, 0x06, 0xC0, 0xC0, 0x18, 0x00, 0x03, 0x03, 0x03, 0x03, 0x0C, 0x18, 0x0C, 0x0C,
0x03, 0xC0, 0xF0, 0x30, 0xC3, 0x03, 0x00, 0xC3, 0x00, 0xC0, 0x18, 0x00, 0x06, 0x00, 0xC3, 0x00,
0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC0, 0x30, 0x0C, 0x0F, 0x03, 0x0C, 0x30, 0x6C, 0x33, 0x00, 0xC0,
0xF0, 0x3C, 0x0F, 0x00, 0x6F, 0x30, 0xCC, 0x0C, 0x30, 0xC0, 0xC7, 0x8E, 0x1D, 0x86, 0x0C, 0x30,
0x01, 0x80, 0x0C, 0x03, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC0, 0x03, 0x00,
0x0F, 0x03, 0x0C, 0x0C, 0xCC, 0x60, 0x30, 0xCC, 0xF0, 0x3C, 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x0C,
0x60, 0xC0, 0xCF, 0x0C, 0xCD, 0x86, 0x00, 0xD8, 0x00, 0xC0, 0x30, 0x06, 0x00, 0x00, 0x0C, 0x00,
0x03, 0x30, 0x30, 0x66, 0xD9, 0xB0, 0x00, 0x30, 0x0C, 0x0C, 0x60, 0xC0, 0x18, 0x00, 0x03, 0x06,
0x01, 0x86, 0x0C, 0x30, 0x06, 0x18, 0x03, 0x61, 0x98, 0x60, 0xC1, 0x86, 0x01, 0x80, 0x00, 0xC0,
0x0C, 0x00, 0x0C, 0x00, 0xC1, 0x80, 0xC0, 0xF0, 0x66, 0x1B, 0x06, 0xC0, 0x30, 0x06, 0x1B, 0x03,
0x0C, 0x18, 0xCC, 0x1B, 0x00, 0xC0, 0xF0, 0x36, 0x1B, 0x00, 0x3E, 0x30, 0x66, 0x18, 0x30, 0x61,
0x83, 0x0C, 0x0F, 0x03, 0x0C, 0x30, 0x01, 0x80, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x60, 0xF0,
0x66, 0x19, 0x83, 0x60, 0xC3, 0x00, 0x1B, 0x03, 0x0C, 0x0C, 0xCC, 0x30, 0x30, 0xCC, 0xF0, 0x36,
0x1B, 0x00, 0x00, 0xF0, 0x0C, 0x0C, 0x60, 0x60, 0xC6, 0x0C, 0xCF, 0x03, 0x01, 0xB0, 0x00, 0x60,
0x30, 0x0C, 0x00, 0x00, 0x0C, 0x00, 0x03, 0x30, 0x30, 0x63, 0x8F, 0x30, 0x00, 0x18, 0x18, 0x18,
0x30, 0xC0, 0x30, 0x00, 0x03, 0x06, 0x00, 0xFC, 0x7F, 0xBF, 0xF3, 0xF0, 0x03, 0x3F, 0x0F, 0xC0,
0xC0, 0xFC, 0x7F, 0x00, 0x01, 0x80, 0x06, 0x00, 0x18, 0x00, 0xC0, 0xFF, 0xC0, 0xFF, 0xC3, 0xF3,
0xFC, 0xFF, 0xF0, 0x03, 0xF3, 0x03, 0x3F, 0x0F, 0x8C, 0x0F, 0xFF, 0xC0, 0xF0, 0x33, 0xF3, 0x00,
0x03, 0x30, 0x33, 0xF0, 0x30, 0x3F, 0x03, 0x08, 0x07, 0x03, 0x0C, 0x3F, 0xF1, 0xF0, 0x06, 0x1F,
0x00, 0x0F, 0xFC, 0x00, 0x3F, 0xFF, 0xC3, 0xF0, 0xFF, 0x3F, 0x83, 0x07, 0xF3, 0x03, 0x3F, 0x07,
0x8C, 0x18, 0xFC, 0xCC, 0xF0, 0x33, 0xF3, 0x00, 0x00, 0xF0, 0x07, 0xF8, 0x3E, 0x3F, 0xC6, 0x07,
0xFB, 0x03, 0x7F, 0x3F, 0xF0, 0x38, 0x30, 0x38, 0x00, 0x00
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
  cursor_x = 0;
  cursor_y = 0;
  font_size = 0;
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
  spi_write(SSD1306_DISPLAYOFF);                    // 0xAE
  spi_write(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  spi_write(0x80);                                  // the suggested ratio 0x80
  spi_write(SSD1306_SETMULTIPLEX);                  // 0xA8
  spi_write(0x1F);
  spi_write(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  spi_write(0x0);                                   // no offset
  spi_write(SSD1306_SETSTARTLINE | 0x0);            // line #0
  spi_write(SSD1306_CHARGEPUMP);                    // 0x8D
  spi_write(0x14); //switchcap
  spi_write(SSD1306_MEMORYMODE);                    // 0x20
  spi_write(0x00);                                  // 0x0 act like ks0108
  spi_write(SSD1306_SEGREMAP | 0x1);
  spi_write(SSD1306_COMSCANDEC);
  spi_write(SSD1306_SETCOMPINS);                    // 0xDA
  spi_write(0x02);
  spi_write(SSD1306_SETCONTRAST);                   // 0x81
  spi_write(0xCF);
  spi_write(SSD1306_SETPRECHARGE);                  // 0xd9
  spi_write(0xF1); //switch cap
  spi_write(SSD1306_SETVCOMDETECT);                 // 0xDB
  spi_write(0x40);
  spi_write(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  if(invert_screen) {
    spi_write(SSD1306_INVERTDISPLAY);
  } else {
    spi_write(SSD1306_NORMALDISPLAY);
  }
  if(orientation) {
    spi_write(SSD1306_SEGREMAP);
    spi_write(SSD1306_COMSCANINC);
  }
  spi_write(SSD1306_DISPLAYON); //--turn on oled panel

  //yuck?
}
void small_ssd1306::off(void) {
  spi_write(SSD1306_DISPLAYOFF);	// put the OLED display in sleep mode
  spi_write(0x8D);  // disable charge pump
  spi_write(0x10);  // disable charge pump

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
    spi_write(SSD1306_NORMALDISPLAY);
  } else {
    invert_screen=1;
    spi_write(SSD1306_INVERTDISPLAY);
  }
}

void small_ssd1306::set_font(int8_t value) {
  font_size=value;
}

void small_ssd1306::clear(void) {
  memset(buffer, 0, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
}

void small_ssd1306::set_pixel(uint8_t x, uint8_t y, uint8_t on) {
  if ((x < 0) || (x >= SSD1306_LCDWIDTH) || (y < 0) || (y >= SSD1306_LCDHEIGHT))
    return;
  // x is which column
  if (on) 
    buffer[x+ (y/8)*SSD1306_LCDWIDTH] |= (1 << (y&7));  
  else
    buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); 
}

void small_ssd1306::draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap,
				uint16_t w, uint16_t h, uint16_t w2, uint16_t h2,
				uint16_t w_size, uint16_t h_size, uint8_t on) {
  int16_t i, j, byteWidth = (w_size + 7) / 8;

  for(j=h; j<h_size; j++) {
    if(j >= h2) {
      break;
    }
    for(i=w; i<w_size; i++ ) {
      if( i >= w2) {
	break;
      }
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
	  set_pixel(x+i-w, y+j-h, on);
      }
    }
  }
}

size_t small_ssd1306::write(uint8_t c) {
  uint16_t character;
  if(font_size == 1) {
    if(c >= 0x30 && c < 0x40) {
      character = (c - 0x30)*13;
      draw_bitmap(cursor_x, cursor_y, terminus, character, 0, character+13, 20, 136, 20, 1);
      cursor_x+=15;
      return 1;
    }
  } 
  if (c == '\n') {
    cursor_y += 16;
    cursor_x  = 0;
  } else if (c == ' ') {
    cursor_x  += 11;
  } else if (c >= 0x21 && ( c <= 0x7E)) {
    character = (c-0x21)*10;
      draw_bitmap(cursor_x, cursor_y, terminus_font, character, 0, character+10, 15, 944, 15, 1);
      cursor_x+=11;

    if ((cursor_x > (SSD1306_LCDWIDTH - 11))) {
      cursor_y += 16;
      cursor_x = 0;
    }
  }
return 1;

}

void small_ssd1306::set_cursor(int8_t x, int8_t y) {
  cursor_x = x;
  cursor_y = y;
}

void small_ssd1306::update(void) {
  spi_write(SSD1306_COLUMNADDR);
  spi_write(0);   // Column start address (0 = reset)
  spi_write(127); // Column end address (127 = reset)

  spi_write(SSD1306_PAGEADDR);
  spi_write(0); // Page start address (0 = reset)
  spi_write(3); // Page end address

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

void small_ssd1306::spi_write(uint8_t data) {
  *csport |= cspinmask;
  *dcport &= ~dcpinmask;
  *csport &= ~cspinmask;

  for(uint8_t bit = 0x80; bit; bit >>= 1) {
    *clkport &= ~clkpinmask;
    if(data & bit) {
      *mosiport |=  mosipinmask;
    } else {
      *mosiport &= ~mosipinmask;
    }
    *clkport |=  clkpinmask;
  } 
  *csport |= cspinmask;
}
