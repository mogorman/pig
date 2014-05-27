#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#define OLD

#ifdef OLD
#define OLED_CS 	10  // AVR pin 19 (SCK)
#define OLED_MOSI 	11  // AVR pin 18 (MISO)
#define OLED_CLK 	13  // AVR pin 17 (MOSI)
#define OLED_DC 	12  // AVR pin 16 (SS_)
#define OLED_RESET 	 9  // AVR pin 15 (OC1A)
#define VDD_DISABLE	 5  // signal to control base of transistor gating OLED's VDD
#define LED              4
#define BUTTON           2
#else
#define OLED_CS 	10  // AVR pin 19 (SCK)
#define OLED_MOSI 	11  // AVR pin 18 (MISO)
#define OLED_CLK 	13  // AVR pin 17 (MOSI)
#define OLED_DC 	12  // AVR pin 16 (SS_)
#define OLED_RESET 	A2  // AVR pin 15 (OC1A)
#define VDD_DISABLE	 4  // signal to control base of transistor gating OLED's VDD
#define LED             A1
#define BUTTON           2
#endif

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


/* this is the default secret that gets flashed to all tokens */
/*nice test site http://blog.tinisles.com/2011/10/google-authenticator-one-time-password-algorithm-in-javascript/ */
//PROGMEM const uint8_t secret_time [] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0xde, 0xad, 0xbe, 0xef,
//   0x53, 0x71, 0xDF, 0x05 };
// 10 byte secret, 4 byte unix time stamp.
PROGMEM const uint8_t secret_time [] = { 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
					 0x42, 0x42, 0x42, 0x42 };

unsigned long Time = 0;
volatile int state = LOW;
volatile unsigned long sound_check = 0;

void reboot();
void setup_mode();
bool first_boot();
void google_totp();
void init_token();
void display_init();
void display_reinit();
void display_off();

/* //SHA example code. */


/* This code is public-domain - it is based on libcrypt
 * placed in the public domain by Wei Dai and other contributors.
 */
// gcc -Wall -DSHA1TEST -o sha1test sha1.c && ./sha1test



#define HASH_LENGTH 20
#define BLOCK_LENGTH 64

typedef struct sha1nfo {
	uint32_t buffer[BLOCK_LENGTH/4];
	uint32_t state[HASH_LENGTH/4];
	uint32_t byteCount;
	uint8_t bufferOffset;
	uint8_t keyBuffer[BLOCK_LENGTH];
	uint8_t innerHash[HASH_LENGTH];
} sha1nfo;

/* public API - prototypes - TODO: doxygen*/

/**
 */
void sha1_init(struct sha1nfo *s);
/**
 */
void sha1_writebyte(struct sha1nfo *s, uint8_t data);
/**
 */
void sha1_write(struct sha1nfo *s, const char *data, size_t len);
/**
 */
uint8_t* sha1_result(struct sha1nfo *s);
/**
 */
void sha1_initHmac(struct sha1nfo *s, const uint8_t* key, int keyLength);
/**
 */
uint8_t* sha1_resultHmac(struct sha1nfo *s);




void setup()
{
  if(first_boot()) {
    setup_mode();
  } // else we run like normal.
  init_token();
}

void loop()
{
  sound_check=0;
  if(state == LOW) {
    delay(100);
    sleep_mode();
  } else { 
    display_reinit();
    display.dim(true);
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("awake");
    display.println(Time);
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setCursor(0,0);
    display.display();
    delay(2000);
    google_totp();
    delay(2000);
    display_off();
    state = LOW;
  }
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

  google_totp();
  //  while ( 1 );
  return;
}

void google_totp() {
  byte *Big_hash;
  int Offset;
  long Truncated_hash;
  int i;
  char Message[7] ={0,0,0,0,0,0,0};
  uint8_t secret[10] = {0};
  unsigned long Google_time = Time / 30;
  byte Google_time_array[8] = { 0x00, 0x00, 0x00, 0x00,
				(byte)((Google_time >> 24) & 0xFF),
				(byte)((Google_time >> 16) & 0xFF),
				(byte)((Google_time >> 8) & 0xFF),
				(byte)(Google_time & 0xFF) };
  sha1nfo s;

  for ( i = 0; i < 10; i++) {
    secret[i] = pgm_read_byte(&secret_time[i]);
  }
  sha1_initHmac(&s, secret, 10);
  sha1_write(&s, Google_time_array,8);
  Big_hash = sha1_resultHmac(&s);

  //Sha1.initHmac(secret, 10);
  //  Sha1.write(Google_time_array, 8);
  //  Big_hash = Sha1.resultHmac();
  Offset = Big_hash[20-1] & 0x0F;
  Truncated_hash = 0;
  for ( int j =0; j < 4; ++j) {
    Truncated_hash <<= 8;
    Truncated_hash |= Big_hash[Offset + j];
  }
  Truncated_hash &= 0x7FFFFFFF;
  Truncated_hash %= 1000000;
  sprintf(Message, "%06ld", Truncated_hash);

  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  display.print("    ");
  display.println(Message);
  display.display();
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
  pinMode(BUTTON, INPUT); //This is the main button, tied to INT0
  digitalWrite(BUTTON, HIGH); //Enable internal pull up on button
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

  EICRA |= (1<<ISC11); //Interrupt on falling edge
  EIMSK |= (1<<INT1); //Enable INT0 interrupt

  sei(); //Enable global interrupts
  display_init();
}


SIGNAL(TIMER2_OVF_vect){
  Time +=8;
}

SIGNAL(INT0_vect){
  //When you hit the button, we will need to display the time
  //if(show_the_time == false)
  state = HIGH;
}



SIGNAL(INT1_vect){
  //When you hit the button, we will need to display the time
  //if(show_the_time == false)
  sound_check++;
  if(sound_check == 1000) {
    reboot();
  }
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
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
}

void display_reinit()
{
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
    display.ssd1306_command(SSD1306_DISPLAYON);

    //ssd1306_command(SSD1306_SEGREMAP | 0x1);
    display.ssd1306_command(SSD1306_SEGREMAP);
    //ssd1306_command(SSD1306_COMSCANDEC);
    display.ssd1306_command(SSD1306_COMSCANINC);
}

void display_off()
{
  display.clearDisplay();
  display.display();
  display.ssd1306_command(SSD1306_DISPLAYOFF);	// put the OLED display in sleep mode
  display.ssd1306_command(0x8D);  // disable charge pump
  display.ssd1306_data(0x10);  // disable charge pump

  delay(10);
  digitalWrite(VDD_DISABLE, HIGH);
}





/* //SHA example code. */


/* This code is public-domain - it is based on libcrypt
 * placed in the public domain by Wei Dai and other contributors.
 */
// gcc -Wall -DSHA1TEST -o sha1test sha1.c && ./sha1test





/* code */
#define SHA1_K0  0x5a827999
#define SHA1_K20 0x6ed9eba1
#define SHA1_K40 0x8f1bbcdc
#define SHA1_K60 0xca62c1d6

void sha1_init(struct sha1nfo *s) {
	s->state[0] = 0x67452301;
	s->state[1] = 0xefcdab89;
	s->state[2] = 0x98badcfe;
	s->state[3] = 0x10325476;
	s->state[4] = 0xc3d2e1f0;
	s->byteCount = 0;
	s->bufferOffset = 0;
}

uint32_t sha1_rol32(uint32_t number, uint8_t bits) {
	return ((number << bits) | (number >> (32-bits)));
}

void sha1_hashBlock(struct sha1nfo *s) {
	uint8_t i;
	uint32_t a,b,c,d,e,t;

	a=s->state[0];
	b=s->state[1];
	c=s->state[2];
	d=s->state[3];
	e=s->state[4];
	for (i=0; i<80; i++) {
		if (i>=16) {
			t = s->buffer[(i+13)&15] ^ s->buffer[(i+8)&15] ^ s->buffer[(i+2)&15] ^ s->buffer[i&15];
			s->buffer[i&15] = sha1_rol32(t,1);
		}
		if (i<20) {
			t = (d ^ (b & (c ^ d))) + SHA1_K0;
		} else if (i<40) {
			t = (b ^ c ^ d) + SHA1_K20;
		} else if (i<60) {
			t = ((b & c) | (d & (b | c))) + SHA1_K40;
		} else {
			t = (b ^ c ^ d) + SHA1_K60;
		}
		t+=sha1_rol32(a,5) + e + s->buffer[i&15];
		e=d;
		d=c;
		c=sha1_rol32(b,30);
		b=a;
		a=t;
	}
	s->state[0] += a;
	s->state[1] += b;
	s->state[2] += c;
	s->state[3] += d;
	s->state[4] += e;
}

void sha1_addUncounted(struct sha1nfo *s, uint8_t data) {
	uint8_t * const b = (uint8_t*) s->buffer;

	b[s->bufferOffset ^ 3] = data;

	s->bufferOffset++;
	if (s->bufferOffset == BLOCK_LENGTH) {
		sha1_hashBlock(s);
		s->bufferOffset = 0;
	}
}

void sha1_writebyte(struct sha1nfo *s, uint8_t data) {
	++s->byteCount;
	sha1_addUncounted(s, data);
}

void sha1_write(struct sha1nfo *s, const byte *data, size_t len) {
	for (;len--;) sha1_writebyte(s, (uint8_t) *data++);
}

void sha1_pad(struct sha1nfo *s) {
	// Implement SHA-1 padding (fips180-2 §5.1.1)

	// Pad with 0x80 followed by 0x00 until the end of the block
	sha1_addUncounted(s, 0x80);
	while (s->bufferOffset != 56) sha1_addUncounted(s, 0x00);

	// Append length in the last 8 bytes
	sha1_addUncounted(s, 0); // We're only using 32 bit lengths
	sha1_addUncounted(s, 0); // But SHA-1 supports 64 bit lengths
	sha1_addUncounted(s, 0); // So zero pad the top bits
	sha1_addUncounted(s, s->byteCount >> 29); // Shifting to multiply by 8
	sha1_addUncounted(s, s->byteCount >> 21); // as SHA-1 supports bitstreams as well as
	sha1_addUncounted(s, s->byteCount >> 13); // byte.
	sha1_addUncounted(s, s->byteCount >> 5);
	sha1_addUncounted(s, s->byteCount << 3);
}

uint8_t* sha1_result(struct sha1nfo *s) {
	// Pad to complete the last block
	sha1_pad(s);

	// Swap byte order back
	int i;
	for (i=0; i<5; i++) {
		s->state[i]=
			  (((s->state[i])<<24)& 0xff000000)
			| (((s->state[i])<<8) & 0x00ff0000)
			| (((s->state[i])>>8) & 0x0000ff00)
			| (((s->state[i])>>24)& 0x000000ff);
	}

	// Return pointer to hash (20 characters)
	return (uint8_t*) s->state;
}

#define HMAC_IPAD 0x36
#define HMAC_OPAD 0x5c

void sha1_initHmac(struct sha1nfo *s, const uint8_t* key, int keyLength) {
	uint8_t i;
	memset(s->keyBuffer, 0, BLOCK_LENGTH);
	if (keyLength > BLOCK_LENGTH) {
		// Hash long keys
		sha1_init(s);
		for (;keyLength--;) sha1_writebyte(s, *key++);
		memcpy(s->keyBuffer, sha1_result(s), HASH_LENGTH);
	} else {
		// Block length keys are used as is
		memcpy(s->keyBuffer, key, keyLength);
	}
	// Start inner hash
	sha1_init(s);
	for (i=0; i<BLOCK_LENGTH; i++) {
		sha1_writebyte(s, s->keyBuffer[i] ^ HMAC_IPAD);
	}
}

uint8_t* sha1_resultHmac(struct sha1nfo *s) {
	uint8_t i;
	// Complete inner hash
	memcpy(s->innerHash,sha1_result(s),HASH_LENGTH);
	// Calculate outer hash
	sha1_init(s);
	for (i=0; i<BLOCK_LENGTH; i++) sha1_writebyte(s, s->keyBuffer[i] ^ HMAC_OPAD);
	for (i=0; i<HASH_LENGTH; i++) sha1_writebyte(s, s->innerHash[i]);
	return sha1_result(s);
}
