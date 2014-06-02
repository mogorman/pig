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
#define ORIENTATION 0    // 0 is normal 1 is inverted 180 degrees
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

small_ssd1306 display(OLED_MOSI, OLED_CLOCK, OLED_DC, OLED_RESET, OLED_CS,
		      OLED_POWER, INVERT_SCREEN, ORIENTATION);

/* this is the default secret that gets flashed to all tokens */
/*nice test site http://blog.tinisles.com/2011/10/google-authenticator-one-time-password-algorithm-in-javascript/ */
//PROGMEM const uint8_t secret_time [] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0xde, 0xad, 0xbe, 0xef,
//   0x53, 0x71, 0xDF, 0x05 };
// 10 byte secret, 4 byte unix time stamp.
const uint8_t secret_time [] = { 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
				 0x41, 0x41, 0x41, 0x41 };

small_totp totp(secret_time, 10);

uint32_t Time = 0;
volatile boolean state = LOW;
volatile uint16_t sound_check = 0;

void reboot();
void setup_mode();
bool first_boot();
void init_token();

void setup()
{
  init_token();
  Serial.begin(9600);
  Serial.println("hello world");
  int i;
  if(first_boot()) {
    setup_mode();
  } // else we run like normal.

  display.on();
  display.update();
  for(i = 0; i < 3; i++) {
    sleep_enable();
    sleep_mode();
    sleep_disable();
  }
  display.clear();
  display.set_font(1);
  display.print(0);
  display.set_font(0); 
  display.update();
  for(i = 0; i < 100; i++) {
    sleep_enable();
    sleep_mode();
    sleep_disable();
  }
  display.off();
  Serial.println("hello world2");
  delay(1000);
}

void loop()
{
  int i;

  if(state == LOW) {
    sleep_enable();
    sleep_mode();
    sleep_disable();
  } else {
    EIMSK |= (1<<INT1); //Enable sound interrupt
    EIMSK &= ~(1<<INT0); //Disable button interrupt
    power_spi_enable();
    display.on();
    display.clear();
    display.set_cursor(0,5);
    display.print(Time);
    display.update();
    for(i = 0; i < 3; i++) {
      sleep_enable();
      sleep_mode();
      sleep_disable();
    }
    display.clear();
    display.set_cursor(0,0);
    display.set_font(1);
    pad_print(totp.code(Time));
    display.set_font(0); 
    display.update();
    //    delay(30000);
    for(i = 0; i < 30; i++) {
	sleep_enable();
	sleep_mode();
	sleep_disable();
    }
    display.off();
    power_spi_disable();
    EIMSK &= ~(1<<INT1); //Disable sound interrupt
    EIMSK |= (1<<INT0); //Enable INT0 interrupt
    if(sound_check > 1000) {
      reboot();
    } else { 
      sound_check=0;
    }
    state = LOW;
  }
}


void pad_print( uint32_t number) {
  uint32_t currentMax = 10;
  byte i;
  for (i = 1; i < 6; i++){
    if (number < currentMax) {
      display.print(0);
    }
    currentMax *= 10;
  } 
  display.print(number);
}


void reboot() {
   display.off();
   display.on();
   display.clear();
   display.set_cursor(0,0);
   //   display.invert();
   display.print(F("reboot"));
   display.update();
   cli();                  // Clear interrupts
   wdt_enable(WDTO_2S);      // Set the Watchdog to 1 second
   while(1){
     asm("nop");
   }            // Enter an infinite loop
}


/*
boot.  check first boot flag. if ! 42 we know its first boot
we write the time to next 4 bytes. run normally.
 if it is 42 we know its a reboot check if key is all 0x42

 */


/* functions of token display, catch audio reboot, verify that i should be in clock mode or setup mode. */

bool first_boot() {
  /* if the secret is set to the default value put token in setup mode */
  int i, ret = 0;
  for( i = 0; i < 10; i++) {
    if(secret_time[i] != 0x42) {
      break;
    }
    if(i == 9) //we got to end and all results where 0x42
      return true;
  }
  for(i = 0; i < 4; i++) {
    if(EEPROM.read(i) != 0x42) {
      break;
    }
    if(i == 3) {
      return false;
    }
  }
    return true;
}

void setup_mode()  {
  /* to implement.... i need to have the display explain to the user what steps they need
     to take to initialize their token */

  return;
}

void init_token() {
  int i;
  for ( i = 0; i < 4; i++) {
    Time <<= 8;
    Time |= secret_time[(10 + i)]; //offset by the secret
  }
    DISABLE_UNUSED_PINS;
  //DISABLE ALL THE PINS!

  // DISABLE ALL THE PINS!

  pinMode(BUTTON, INPUT); //This is the main button, tied to INT0
  digitalWrite(BUTTON, HIGH); //Enable internal pull up on button


  ADCSRA &= ~(1<<ADEN); //Disable ADC
  ACSR = (1<<ACD); //Disable the analog comparator
  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
  power_twi_disable();
  power_timer1_disable();
//Setup TIMER2
  TCCR2A = 0x00;
  TCCR2B = (1<<CS22)|(1<<CS20); //Set CLK/128 or overflow interrupt every 1s
  //TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20); //Set CLK/1024 or overflow interrupt every 8s
  ASSR = (1<<AS2); //Enable asynchronous operation
  TIMSK2 = (1<<TOIE2); //Enable the timer 2 interrupt

  EICRA = (1<<ISC01); //Interrupt on falling edge
  EIMSK |= (1<<INT0); //Enable INT0 interrupt
  EICRA |= (1<<ISC11); //Interrupt on falling edge
  //  EIMSK |= (1<<INT1); //Enable INT0 interrupt
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sei(); //Enable global interrupts
}

SIGNAL(TIMER2_OVF_vect){
  Time++; // +=8 if other timer set
}

SIGNAL(INT0_vect){
  state = HIGH; //button pressed
}

SIGNAL(INT1_vect){
  sound_check++; //sound detected
}
