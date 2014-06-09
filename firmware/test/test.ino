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

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println("I am test code");
  delay(1000);
}
