#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include <sha1.h>

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


//#define OLED_CS 	10  // AVR pin 19 (SCK)
//#define OLED_MOSI 	11  // AVR pin 18 (MISO)
//#define OLED_CLK 	13  // AVR pin 17 (MOSI)
//#define OLED_DC 	12  // AVR pin 16 (SS_)
//#define OLED_RESET 	9   // AVR pin 15 (OC1A)
//#define VDD_DISABLE	5   // signal to control base of transistor gating OLED's VDD


int Count;
/* this is the default secret that gets flashed to all tokens */
//byte secret[10] = { 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42 };
/*nice test site http://blog.tinisles.com/2011/10/google-authenticator-one-time-password-algorithm-in-javascript/ */
byte secret[10] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0xde, 0xad, 0xbe, 0xef };
const unsigned long Time = 1399966933;
volatile int state = LOW;

void reboot();
void setup_mode();
bool first_boot();
void google_totp();

void setup(){
  Serial.begin(9600);
  if(first_boot()) {
    setup_mode();
  } // else we run like normal.
  pinMode(4, OUTPUT);

  Sha1.init();


  digitalWrite(4, HIGH);
  attachInterrupt(0, blink, CHANGE);
  delay(500);
  digitalWrite(4, LOW);
  Count = 0;
}

void loop(){
  //  delay(3000);
  //  sleep_now();
  int value = 0;
  byte Time_array[4] = {0,0,0,0};

  Time_array[0] = (int)((Time >> 24) & 0xFF) ;
  Time_array[1] = (int)((Time >> 16) & 0xFF) ;
  Time_array[2] = (int)((Time >> 8) & 0XFF);
  Time_array[3] = (int)((Time & 0XFF));
  digitalWrite(4, state);

  Serial.print("hello there ");
  Serial.print(Time, HEX);
  Serial.print(" ");
  Serial.print(Time_array[0], HEX);
  Serial.print(Time_array[1], HEX);
  Serial.print(Time_array[2], HEX);
  Serial.print(Time_array[3], HEX);
  Serial.print(" ");
  Serial.print(Count);
  Serial.print(" configured ");
  Serial.print(first_boot());
  value = EEPROM.read(0);
  Serial.print(" 0x00 ");
  Serial.print(value);
  value = EEPROM.read(1);
  Serial.print(" 0x01 ");
  Serial.print(value);
  value = EEPROM.read(2);
  Serial.print(" 0x02 ");
  Serial.print(value);
  value = EEPROM.read(3);
  Serial.print(" 0x03 ");
  Serial.print(value);
  value = EEPROM.read(4);
  Serial.print(" 0x04 ");
  Serial.print(value);
  Serial.println("");
  delay(300);
  if (Count == 8) {
    delay(300);
    reboot(); 
    Serial.println("THIS IS IMPOSSIBLE");
  }
  Count++;
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
  if(secret[0] == 0x42 &&
     secret[1] == 0x42 &&
     secret[2] == 0x42 &&
     secret[3] == 0x42 &&
     secret[4] == 0x42 &&
     secret[5] == 0x42 &&
     secret[6] == 0x42 &&
     secret[7] == 0x42 &&
     secret[8] == 0x42 &&
     secret[9] == 0x42) {
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
  return true;
}

void setup_mode()  {
  /* to implement.... i need to have the display explain to the user what steps they need
     to take to initialize their token */

  Serial.println("i am not gonna take another step.");
  google_totp();
  while ( 1 );
  return;
}

void google_totp() {
  byte *Big_hash;
  int Offset;
  long Truncated_hash;
  unsigned long Google_time = Time / 30;
  char Message[7];
  byte Google_time_array[8] = { 0x00, 0x00, 0x00, 0x00,
				(byte)((Google_time >> 24) & 0xFF),
				(byte)((Google_time >> 16) & 0xFF),
				(byte)((Google_time >> 8) & 0xFF),
				(byte)(Google_time & 0xFF) };
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
  Serial.print("Message: ");
  Serial.println(Message);
}
