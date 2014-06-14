/*
 *
 * Battery discharge formula
 * equivalent_current_mA = (percent_monthly_discharge_rate / 100) * capacity_maH / (24 * 30) 
 */


#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include <small_ssd1306.h>
#include <small_totp.h>

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#define OLD

#ifdef OLD
#define OLED_CS 	10  // AVR pin 19 (SCK)
#define OLED_MOSI 	11  // AVR pin 18 (MISO)
#define OLED_CLOCK 	13  // AVR pin 17 (MOSI)
#define OLED_DC 	12  // AVR pin 16 (SS_)
#define OLED_RESET 	 9  // AVR pin 15 (OC1A)
#define OLED_POWER	 5  // signal to control base of transistor gating OLED's VDD
#define LED              4
#define BUTTON           2
#define INVERT_SCREEN 0  // 0 is normal 1 is inverted color
#define ORIENTATION 1    // 0 is normal 1 is inverted 180 degrees
#define DISABLE_UNUSED_PINS \
  DDRD = B11111111; \
  DDRC = B11111111; \
  DDRB = B11111111; \
  PORTD = B11001011; \
  PORTC = B11111111; \
  PORTB = B11000001; 
#else
#define OLED_CS 	10  // AVR pin 19 (SCK)
#define OLED_MOSI 	11  // AVR pin 18 (MISO)
#define OLED_CLOCK 	13  // AVR pin 17 (MOSI)
#define OLED_DC 	12  // AVR pin 16 (SS_)
#define OLED_RESET 	A2  // AVR pin 15 (OC1A)
#define OLED_POWER	 4  // signal to control base of transistor gating OLED's VDD
#define LED             A1
#define BUTTON           2
#define INVERT_SCREEN 0  // 0 is normal 1 is inverted color
#define ORIENTATION 1   // 0 is normal 1 is inverted 180 degrees
#define DISABLE_UNUSED_PINS \
  DDRD = B11111111; \
  DDRC = B11111111; \
  DDRB = B11111111; \
  PORTD = B11101011; \
  PORTC = B11111001; \
  PORTB = B11000011;
#endif


/*
 * \file
 *
 * \brief Example code for testing crystal stability and robustness. The
 * firmware will output the crystal clock on a I/O pin that can be probed
 * with a standard 10X oscilloscope probe. DO NOT probe the crystal directly
 * if you do not have very high impedance probes indeded for crystal
 * measurements. The output clock will be devided by 2. NOTE: The correct 
 * device must be set by a define. 
 *
 *
 * Copyright (C) 2009 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 *
 * Devices 			Define				
 * supported
 *
 * ATmega8   		ATMEGA8
 * ATmega8A			ATMEGA8
 * ATmega16			ATMEGA16_32
 * ATmega16A		ATMEGA16_32
 * ATmega32			ATMEGA16_32	
 * ATmega32A		ATMEGA16_32
 * ATmega64			ATMEGA64_128
 * ATmega64A		ATMEGA64_128	
 * ATmega128		ATMEGA64_128
 * ATmega128A		ATMEGA64_128
 * ATmega640		ATMEGA640_1280_2560
 * ATmega1280		ATMEGA640_1280_2560
 * ATmega1281		ATMEGA640_1280_2560
 * ATmega2560		ATMEGA640_1280_2560
 * ATmega2561		ATMEGA640_1280_2560
 * ATmega162		ATMEGA162
 * ATmega165A		ATMEGA169
 * ATmega165P		ATMEGA169
 * ATmega165PA		ATMEGA169
 * ATmega169		ATMEGA169
 * ATmega169A		ATMEGA169
 * ATmega169P		ATMEGA169
 * ATmega169PA		ATMEGA169
 * ATmega325A		ATMEGA329_649
 * ATmega325P		ATMEGA329_649
 * ATmega325PA		ATMEGA329_649
 * ATmega3250A		ATMEGA329_649
 * ATmega3250P		ATMEGA329_649
 * ATmega3250PA		ATMEGA329_649
 * ATmega329		ATMEGA329_649
 * ATmega329A		ATMEGA329_649
 * ATmega329P		ATMEGA329_649
 * ATmega329PA		ATMEGA329_649
 * ATmega3290A		ATMEGA329_649
 * ATmega3290P		ATMEGA329_649
 * ATmega3290PA		ATMEGA329_649
 * ATmega645A		ATMEGA329_649
 * ATmega645P		ATMEGA329_649
 * ATmega6450A		ATMEGA329_649
 * ATmega6450P		ATMEGA329_649
 * ATmega649		ATMEGA329_649
 * ATmega6490		ATMEGA329_649
 * ATmega649A		ATMEGA329_649
 * ATmega649P		ATMEGA329_649
 * ATmega6490A		ATMEGA329_649
 * ATmega6490P		ATMEGA329_649
 * ATmega48			ATMEGA48_88_168
 * ATmega48A		ATMEGA48_88_168
 * ATmega88			ATMEGA48_88_168
 * ATmega88A		ATMEGA48_88_168
 * ATmega168		ATMEGA48_88_168
 * ATmega168A		ATMEGA48_88_168
 * ATmega48P		ATMEGA48_88_168
 * ATmega48PA		ATMEGA48_88_168
 * ATmega88P		ATMEGA48_88_168
 * ATmega88PA		ATMEGA48_88_168
 * ATmega168P		ATMEGA48_88_168
 * ATmega168PA		ATMEGA48_88_168
 * ATmega164A		ATMEGA164_324_644_1284
 * ATmega164P		ATMEGA164_324_644_1284
 * ATmega164PA		ATMEGA164_324_644_1284
 * ATmega324A		ATMEGA164_324_644_1284
 * ATmega324P		ATMEGA164_324_644_1284
 * ATmega324PA		ATMEGA164_324_644_1284
 * ATmega644A		ATMEGA164_324_644_1284
 * ATmega644P		ATMEGA164_324_644_1284
 * ATmega644PA		ATMEGA164_324_644_1284
 * ATmega1284P		ATMEGA164_324_644_1284
 */

 
#include <avr/io.h>


int the_main( void )
{
	// Crystal clock will be output on PORTB3 (divided by 2)
	DDRB |= (1<<PORTB3);

	// Enable CTC mode with toggle on compare match. Precale 1 and top at 0
	OCR2A  =  0;
	TCCR2A = (0<<COM2A1)|(1<<COM2A0)|(1<<WGM21)|(0<<WGM20);
	TCCR2B = (0<<WGM22)|(0<<CS22)|(0<<CS21)|(1<<CS20);
	ASSR   = (1<<AS2);

	while(1){}
}

void setup()
{
  the_main();
}

void loop()
{
}



