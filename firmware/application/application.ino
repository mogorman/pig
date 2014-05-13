#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include <sha1.h>

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>


//#define OLED_CS 	10  // AVR pin 19 (SCK)
//#define OLED_MOSI 	11  // AVR pin 18 (MISO)
//#define OLED_CLK 	13  // AVR pin 17 (MOSI)
//#define OLED_DC 	12  // AVR pin 16 (SS_)
//#define OLED_RESET 	9   // AVR pin 15 (OC1A)
//#define VDD_DISABLE	5   // signal to control base of transistor gating OLED's VDD


/* this is the default secret that gets flashed to all tokens */
/*nice test site http://blog.tinisles.com/2011/10/google-authenticator-one-time-password-algorithm-in-javascript/ */
PROGMEM const uint8_t secret_time [] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0xde, 0xad, 0xbe, 0xef,
   0x53, 0x71, 0xDF, 0x05 };
/*PROGMEM const uint8_t secret_time [] = { 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
					 0x42, 0x42, 0x42, 0x42 };
*/
unsigned long Time = 0;
volatile int state = LOW;

void reboot();
void setup_mode();
bool first_boot();
void google_totp();
void init_token();

void setup(){
  Serial.begin(9600);
  if(first_boot()) {
    setup_mode();
  } // else we run like normal.
  init_token();
}

void loop(){
  if(state == LOW) {
    Serial.println("going to sleep");
    delay(1000);
    sleep_mode();
  }
  Serial.print("time = ");
  Serial.println(Time);
  state = LOW;
}


void reboot() {
  cli();                  // Clear interrupts
  wdt_enable(WDTO_1S);      // Set the Watchdog to 1 second
  while(1){
    asm("nop");
  }            // Enter an infinite loop
}


/*
boot.  check first boot flag. if ! 42 we know its first boot 
we write the time to next 4 bytes. run normally.
 if it is 42 we know its a reboot check if key is all 0x42

 */

void blink()
{
  state = !state;
}



/* functions of token display, catch audio reboot, verify that i should be in clock mode or setup mode. */

bool first_boot() {
  /* if the secret is set to the default value put token in setup mode */
  if(pgm_read_byte(&secret_time[0]) == 0x42 &&
    pgm_read_byte(&secret_time[1]) == 0x42 &&
    pgm_read_byte(&secret_time[2]) == 0x42 &&
    pgm_read_byte(&secret_time[3]) == 0x42 &&
    pgm_read_byte(&secret_time[4]) == 0x42 &&
    pgm_read_byte(&secret_time[5]) == 0x42 &&
    pgm_read_byte(&secret_time[6]) == 0x42 &&
    pgm_read_byte(&secret_time[7]) == 0x42 &&
    pgm_read_byte(&secret_time[8]) == 0x42 &&
    pgm_read_byte(&secret_time[9]) == 0x42) {
    return true;
  }
  /* if eeprom is set up by reset interrupt and hasn't been reset that means we got reflashed hopefully
     and should run normally.
   */
  if (EEPROM.read(0) == 0x42 &&
      EEPROM.read(1) == 0x42 &&
      EEPROM.read(2) == 0x42 &&
      EEPROM.read(3) == 0x42) {
    EEPROM.write(0,0);
    EEPROM.write(1,0);
    EEPROM.write(2,0);
    EEPROM.write(3,0);
    return false;
  }
  /* otherwise we probably have just changed batteries and we are gonna need a new secret
     because clocks are no longer synced */
  //  return false; // not checkint to see reboot because we are testing.
    return true;
}

void setup_mode()  {
  /* to implement.... i need to have the display explain to the user what steps they need
     to take to initialize their token */

  Serial.println("i am not gonna take another step.");
  google_totp();
  //  while ( 1 );
  return;
}

void google_totp() {
  byte *Big_hash;
  int Offset;
  long Truncated_hash;
  int i;
  char Message[7];
  uint8_t secret[10] = {0};
  unsigned long Google_time = Time / 30;
  byte Google_time_array[8] = { 0x00, 0x00, 0x00, 0x00,
				(byte)((Google_time >> 24) & 0xFF),
				(byte)((Google_time >> 16) & 0xFF),
				(byte)((Google_time >> 8) & 0xFF),
				(byte)(Google_time & 0xFF) };
  //  strncpy_P((char *) secret, (char *) pgm_read_word( &(secret_time) ), 10);
  for ( i = 0; i < 10; i++) {
    secret[i] = pgm_read_byte(&secret_time[i]);
  }
  Sha1.initHmac(secret, 10);
  Sha1.write(Google_time_array, 8);
  Big_hash = Sha1.resultHmac();
  Offset = Big_hash[20-1] & 0x0F;
  Truncated_hash = 0;
  for ( int j =0; j < 4; ++j) {
    Truncated_hash <<= 8;
    Truncated_hash |= Big_hash[Offset + j];
  }
  Truncated_hash &= 0x7FFFFFFF;
  Truncated_hash %= 1000000;
  sprintf(Message, "%06ld", Truncated_hash);
  Serial.print("Time: ");
  Serial.println(Google_time, HEX);
  Serial.println(Time, HEX);
  Serial.print("Secret: ");
  Serial.print(secret[0], HEX);
  Serial.print(":");
  Serial.print(secret[1], HEX);
  Serial.print(":");
  Serial.print(secret[2], HEX);
  Serial.print(":");
  Serial.print(secret[3], HEX);
  Serial.print(":");
  Serial.print(secret[4], HEX);
  Serial.print(":");
  Serial.print(secret[5], HEX);
  Serial.print(":");
  Serial.print(secret[6], HEX);
  Serial.print(":");
  Serial.print(secret[7], HEX);
  Serial.print(":");
  Serial.print(secret[8], HEX);
  Serial.print(":");
  Serial.println(secret[9], HEX);


  Serial.print("Message: ");
  Serial.println(Message);
}

void init_token() {
  int i;
  for ( i = 0; i < 4; i++) {
    Time <<= 8;
    Time |= pgm_read_byte(&secret_time[ (10 + i) ]); //offset by the secret 
  }
  /* for(i = 1 ; i < 18 ; i++){ */
  /*   pinMode(i, INPUT); */
  /*   digitalWrite(i, LOW); */
  /* } */
  pinMode(2, INPUT); //This is the main button, tied to INT0
  digitalWrite(2, HIGH); //Enable internal pull up on button
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();

  ADCSRA &= ~(1<<ADEN); //Disable ADC
  ACSR = (1<<ACD); //Disable the analog comparator
  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
  power_twi_disable();
  //  power_timer1_disable();
//Setup TIMER2
  TCCR2A = 0x00;
  //TCCR2B = (1<<CS22)|(1<<CS20); //Set CLK/128 or overflow interrupt every 1s
  TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20); //Set CLK/1024 or overflow interrupt every 8s
  ASSR = (1<<AS2); //Enable asynchronous operation
  TIMSK2 = (1<<TOIE2); //Enable the timer 2 interrupt

  //Setup external INT0 interrupt
  EICRA = (1<<ISC01); //Interrupt on falling edge
  EIMSK = (1<<INT0); //Enable INT0 interrupt

  Serial.println("BigTime Testing:");
  sei(); //Enable global interrupts

}


SIGNAL(TIMER2_OVF_vect){
  Time +=8;
}

SIGNAL(INT0_vect){
  //When you hit the button, we will need to display the time
  //if(show_the_time == false) 
  state = HIGH;
}
