#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include <sha1.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


#define OLED_CS 	10  // AVR pin 19 (SCK)
#define OLED_MOSI 	11  // AVR pin 18 (MISO)
#define OLED_CLK 	13  // AVR pin 17 (MOSI)
#define OLED_DC 	12  // AVR pin 16 (SS_)
#define OLED_RESET 	9   // AVR pin 15 (OC1A)
#define VDD_DISABLE	5   // signal to control base of transistor gating OLED's VDD


#ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 
int counter = 1;


Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

int Count;

byte secret[10] = { 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42 };
const unsigned long Time = 1399348238;
volatile int state = LOW;

void reboot();
void setup_mode();
bool first_boot();

void setup(){
  if(first_boot()) {
    setup_mode();
  } // else we run like normal.
  pinMode(4, OUTPUT);

  Sha1.init();

  Serial.begin(9600);
  digitalWrite(4, HIGH);
  attachInterrupt(0, blink, CHANGE);
  delay(4000);
  digitalWrite(4, LOW);
  display_init();
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
    configured();
    delay(300);
    reboot(); 
    Serial.println("THIS IS IMPOSSIBLE");
  }
  Count++;
}

void display_init()
{
  pinMode(VDD_DISABLE, OUTPUT);
  digitalWrite(VDD_DISABLE, LOW);  
  // by default, generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  //display.begin(SSD1306_EXTERNALVCC);

  display.display();  // show splash screen
  delay(4000);
  // init done
  display.clearDisplay();	// clear the splash screen
  display.display();

  // print some characters
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  for (uint8_t i=65; i < 65+168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
  delay(2000);
  display.clearDisplay();  
}

void sleep_now() {
  display.clearDisplay();
  display.display();
  display.ssd1306_command(SSD1306_DISPLAYOFF);	// put the OLED display in sleep mode
  display.ssd1306_command(0x8D);  // disable charge pump
  display.ssd1306_data(0x10);  // disable charge pump

  delay(10);
  digitalWrite(VDD_DISABLE, HIGH);
  
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode - lowest power sleep mode
//  sleep_enable();               // enables sleep bit in MCUCR register
  
 // sbi(PRR, PRTWI);		// disable the clock to the two-wire interface - need this for the EEPROM
 // sbi(PRR, PRTIM0);		// disable the clock to TIMER0
 // sbi(PRR, PRTIM1);		// disable the clock to TIMER1
  //sbi(PRR, PRUSART0);		// disable the clock to USART

  //sleep_cpu();
  // After waking up, program continues HERE (AFTER running interrupt service function)
  //Serial.println("just woke up");
 // sleep_disable();							// first thing after waking from sleep

 // cbi(PRR, PRTIM0);		// enable the clock to TIMER0
 // cbi(PRR, PRTIM1);		// enable the clock to TIMER1
 // cbi(PRR, PRUSART0);		// enable the clock to USART
 // cbi(PRR, PRTWI);		// enable the clock to the two-wire interface - need this for the EEPROM

  digitalWrite(VDD_DISABLE, LOW);
  delay(200);
  digitalWrite(OLED_RESET, HIGH);
  //VDD (3.3V) goes high at start, lets just chill for a ms
  delay(10);
  // bring reset low
  digitalWrite(OLED_RESET, LOW);
  // wait 10ms
  delay(10);
  // bring out of reset
  digitalWrite(OLED_RESET, HIGH);
  // turn on VCC (9V?)

    // Init sequence for 128x32 OLED module
    display.ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
    display.ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    display.ssd1306_command(0x80);                                  // the suggested ratio 0x80
    display.ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    display.ssd1306_command(0x1F);
    display.ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    display.ssd1306_command(0x0);                                   // no offset
    display.ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
    display.ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
    display.ssd1306_command(0x14);
    display.ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
    display.ssd1306_command(0x00);                                  // 0x0 act like ks0108
    display.ssd1306_command(SSD1306_SEGREMAP | 0x1);
    display.ssd1306_command(SSD1306_COMSCANDEC);
    display.ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
    display.ssd1306_command(0x02);
    display.ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
    display.ssd1306_command(0x8F);
    display.ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
    display.ssd1306_command(0xF1);
    display.ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    display.ssd1306_command(0x40);
    display.ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    display.ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
    display.ssd1306_command(SSD1306_DISPLAYON);						//--turn on oled panel

  display.println("out of sleep");
  display.invertDisplay(counter);
  if(counter) { counter = 0;} else {counter = 1;}
  //display.clearDisplay();
  display.display();
} // void sleepNow()


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
  return false;
}

void setup_mode()  {
  return;
}
