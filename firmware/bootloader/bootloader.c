//OPTIBOOT
#include "boot.h"
//OPTIBOOT

//AUDIOBOOT
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include <stdlib.h>
//AUDIOBOOT

//OPTIBOOT
#define OLD
#ifdef OLD
#define LED D4
#else
#define LED C1
#endif
#include "pin_defs.h"
#include "stk500.h"

//AUDIOBOOT
void a_main();
uint8_t receiveFrame();

/* some variables */
union address_union {
    uint16_t word;
    uint8_t  byte[2];
} address;

union length_union {
    uint16_t word;
    uint8_t  byte[2];
} length;

struct flags_struct {
    unsigned eeprom : 1;
    unsigned rampz  : 1;
} flags;

uint8_t buff[256];
uint8_t address_high;

uint8_t pagesz=0x80;

uint8_t i;
uint8_t bootuart = 0;

uint8_t error_count = 0;

#define TIMER TCNT2 // we use timer2 for measuring time

// frame format definition
#define COMMAND         0
#define PAGEINDEXLOW 	1  // page address lower part
#define PAGEINDEXHIGH 	2  // page address higher part
#define CRCLOW          3  // checksum lower part 
#define CRCHIGH 		4  // checksum higher part 
#define DATAPAGESTART   5  // start of data
#define PAGESIZE 		128
#define FRAMESIZE       (PAGESIZE+DATAPAGESTART)// size of the data block to be received

// bootloader commands
#define NOCOMMAND       0
#define TESTCOMMAND     1
#define PROGCOMMAND     2
#define RUNCOMMAND      3

uint8_t FrameData[FRAMESIZE];





//OPTIBOOT

//AUDIOBOOT
#ifdef OLD
#define LEDPORT (1<<PD4);
#define INITLED {DDRD|=LEDPORT;}
#define LEDON {PORTD|=LEDPORT;}
#define LEDOFF {PORTD&=~LEDPORT;}
#define TOGGLELED {PORTD^=LEDPORT;}
#else
#define LEDPORT (1<<PC1);
#define INITLED {DDRC|=LEDPORT;}
#define LEDON {PORTC|=LEDPORT;}
#define LEDOFF {PORTC&=~LEDPORT;}
#define TOGGLELED {PORTC^=LEDPORT;}
#endif

// use TxD ( PD1 ) as audio input	
#define INPUTAUDIOPIN (1<<PD3)
#define PINVALUE (PIND&INPUTAUDIOPIN)
#define INITPORT {PORTD|=INPUTAUDIOPIN; } //turn on pull up 

#define PINLOW (PINVALUE==0)
#define PINHIGH (!PINLOW)

#define true (1==1)
#define false !true
//AUDIOBOOT

//OPTIBOOT
#define OPTIBOOT_MAJVER 5
#define OPTIBOOT_MINVER 0

#define MAKESTR(a) #a
#define MAKEVER(a, b) MAKESTR(a*256+b)

asm("  .section .version\n"
    "optiboot_version:  .word " MAKEVER(OPTIBOOT_MAJVER, OPTIBOOT_MINVER) "\n"
    "  .section .text\n");

#define BAUD_RATE 9600L
#define LED_START_FLASHES 2
#define F_CPU 8000000L
#define VIRTUAL_BOOT 1

#ifndef UART
#define UART 0
#endif

#define WATCHDOG_OFF    (0)
#define WATCHDOG_16MS   (_BV(WDE))
#define WATCHDOG_32MS   (_BV(WDP0) | _BV(WDE))
#define WATCHDOG_64MS   (_BV(WDP1) | _BV(WDE))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_250MS  (_BV(WDP2) | _BV(WDE))
#define WATCHDOG_500MS  (_BV(WDP2) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_1S     (_BV(WDP2) | _BV(WDP1) | _BV(WDE))
#define WATCHDOG_2S     (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE))
#ifndef __AVR_ATmega8__
#define WATCHDOG_4S     (_BV(WDP3) | _BV(WDE))
#define WATCHDOG_8S     (_BV(WDP3) | _BV(WDP0) | _BV(WDE))
#endif


/* Function Prototypes */
/* The main function is in init9, which removes the interrupt vector table */
/* we don't need. It is also 'naked', which means the compiler does not    */
/* generate any entry or exit code itself. */
int main(void) __attribute__ ((OS_main)) __attribute__ ((section (".init9")));
void putch(char);
uint8_t getch(void);
static inline void getNch(uint8_t); /* "static inline" is a compiler hint to reduce code size */
void verifySpace();
static inline void flash_led(uint8_t);
uint8_t getLen();
static inline void watchdogReset();
void watchdogConfig(uint8_t x);
#ifdef SOFT_UART
void uartDelay() __attribute__ ((naked));
#endif
void appStart(uint8_t rstFlags) __attribute__ ((naked));

/*
 * NRWW memory
 * Addresses below NRWW (Non-Read-While-Write) can be programmed while
 * continuing to run code from flash, slightly speeding up programming
 * time.  Beware that Atmel data sheets specify this as a WORD address,
 * while optiboot will be comparing against a 16-bit byte address.  This
 * means that on a part with 128kB of memory, the upper part of the lower
 * 64k will get NRWW processing as well, even though it doesn't need it.
 * That's OK.  In fact, you can disable the overlapping processing for
 * a part entirely by setting NRWWSTART to zero.  This reduces code
 * space a bit, at the expense of being slightly slower, overall.
 *
 * RAMSTART should be self-explanatory.  It's bigger on parts with a
 * lot of peripheral registers.
 */
#if defined(__AVR_ATmega168__)
#define RAMSTART (0x100)
#define NRWWSTART (0x3800)
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32__)
#define RAMSTART (0x100)
#define NRWWSTART (0x7000)
#elif defined (__AVR_ATmega644P__)
#define RAMSTART (0x100)
#define NRWWSTART (0xE000)
// correct for a bug in avr-libc
#undef SIGNATURE_2
#define SIGNATURE_2 0x0A
#elif defined (__AVR_ATmega1284P__)
#define RAMSTART (0x100)
#define NRWWSTART (0xE000)
#elif defined(__AVR_ATtiny84__)
#define RAMSTART (0x100)
#define NRWWSTART (0x0000)
#elif defined(__AVR_ATmega1280__)
#define RAMSTART (0x200)
#define NRWWSTART (0xE000)
#elif defined(__AVR_ATmega8__) || defined(__AVR_ATmega88__)
#define RAMSTART (0x100)
#define NRWWSTART (0x1800)
#endif

/* C zero initialises all global variables. However, that requires */
/* These definitions are NOT zero initialised, but that doesn't matter */
/* This allows us to drop the zero init code, saving us memory */
#define buff    ((uint8_t*)(RAMSTART))
#ifdef VIRTUAL_BOOT_PARTITION
#define rstVect (*(uint16_t*)(RAMSTART+SPM_PAGESIZE*2+4))
#define wdtVect (*(uint16_t*)(RAMSTART+SPM_PAGESIZE*2+6))
#endif

/*
 * Handle devices with up to 4 uarts (eg m1280.)  Rather inelegantly.
 * Note that mega8/m32 still needs special handling, because ubrr is handled
 * differently.
 */
#if UART == 0
# define UART_SRA UCSR0A
# define UART_SRB UCSR0B
# define UART_SRC UCSR0C
# define UART_SRL UBRR0L
# define UART_UDR UDR0
#elif UART == 1
#if !defined(UDR1)
#error UART == 1, but no UART1 on device
#endif
# define UART_SRA UCSR1A
# define UART_SRB UCSR1B
# define UART_SRC UCSR1C
# define UART_SRL UBRR1L
# define UART_UDR UDR1
#elif UART == 2
#if !defined(UDR2)
#error UART == 2, but no UART2 on device
#endif
# define UART_SRA UCSR2A
# define UART_SRB UCSR2B
# define UART_SRC UCSR2C
# define UART_SRL UBRR2L
# define UART_UDR UDR2
#elif UART == 3
#if !defined(UDR1)
#error UART == 3, but no UART3 on device
#endif
# define UART_SRA UCSR3A
# define UART_SRB UCSR3B
# define UART_SRC UCSR3C
# define UART_SRL UBRR3L
# define UART_UDR UDR3
#endif


//OPTIBOOT


int main(void)
{
  uint8_t ch;

  /*
   * Making these local and in registers prevents the need for initializing
   * them, and also saves space because code no longer stores to memory.
   * (initializing address keeps the compiler happy, but isn't really
   *  necessary, and uses 4 bytes of flash.)
   */
  register uint16_t address = 0;
  register uint8_t  length;

  // After the zero init loop, this is the first code to run.
  //
  // This code makes the following assumptions:
  //  No interrupts will execute
  //  SP points to RAMEND
  //  r1 contains zero
  //
  // If not, uncomment the following instructions:
  // cli();
  asm volatile ("clr __zero_reg__");
#if defined(__AVR_ATmega8__) || defined (__AVR_ATmega32__)
  SP=RAMEND;  // This is done by hardware reset
#endif


  // Adaboot no-wait mod
  ch = MCUSR;
  MCUSR = 0;
  //WAIT
  if ((ch & _BV(WDRF))) appStart(ch);
  //NOWAIT
  //if (!(ch & _BV(EXTRF))) appStart(ch);


#if LED_START_FLASHES > 0
  // Set up Timer 1 for timeout counter
  TCCR1B = _BV(CS12) | _BV(CS10); // div 1024
#endif
#ifndef SOFT_UART
#if defined(__AVR_ATmega8__) || defined (__AVR_ATmega32__)
  UCSRA = _BV(U2X); //Double speed mode USART
  UCSRB = _BV(RXEN) | _BV(TXEN);  // enable Rx & Tx
  UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);  // config USART; 8N1
  UBRRL = (uint8_t)( (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 );
#else
  UART_SRA = _BV(U2X0); //Double speed mode USART0
  UART_SRB = _BV(RXEN0) | _BV(TXEN0);
  UART_SRC = _BV(UCSZ00) | _BV(UCSZ01);
  UART_SRL = (uint8_t)( (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 );
#endif
#endif


  // Set up watchdog to trigger after 500ms
  //this says 500ms changing it back
  //MOG
   DDRD &= ~(1 << PD2);        // see comment #1
   if (PIND & (1<<PD2))    // see comment #2
     {
#ifdef OLD
       DDRD|=(1<<PD4);
       PORTD |= (1<<PD4);          // turn on the LED to indicate receiving data
#else
       DDRC|=(1<<PC1);
       PORTC |= (1<<PC1);          // turn on the LED to indicate receiving data
#endif
       a_main();
       watchdogConfig(WATCHDOG_500MS);
     }
             watchdogConfig(WATCHDOG_500MS);

#if (LED_START_FLASHES > 0) || defined(LED_DATA_FLASH)
  /* Set LED pin as output */
  LED_DDR |= _BV(LED);
#endif

#ifdef SOFT_UART
  /* Set TX pin as output */
  UART_DDR |= _BV(UART_TX_BIT);
#endif

#if LED_START_FLASHES > 0
  /* Flash onboard LED to signal entering of bootloader */
  flash_led(LED_START_FLASHES * 2);

#endif

  /* Forever loop */
  for (;;) {
    /* get character from UART */

    ch = getch();

    if(ch == STK_GET_PARAMETER) {
      unsigned char which = getch();
      verifySpace();
      if (which == 0x82) {
	/*
	 * Send optiboot version as "minor SW version"
	 */
	putch(OPTIBOOT_MINVER);
      } else if (which == 0x81) {
	  putch(OPTIBOOT_MAJVER);
      } else {
	/*
	 * GET PARAMETER returns a generic 0x03 reply for
         * other parameters - enough to keep Avrdude happy
	 */
	putch(0x03);
      }
    }
    else if(ch == STK_SET_DEVICE) {
      // SET DEVICE is ignored
      getNch(20);
    }
    else if(ch == STK_SET_DEVICE_EXT) {
      // SET DEVICE EXT is ignored
      getNch(5);
    }
    else if(ch == STK_LOAD_ADDRESS) {
      // LOAD ADDRESS
      uint16_t newAddress;
      newAddress = getch();
      newAddress = (newAddress & 0xff) | (getch() << 8);
#ifdef RAMPZ
      // Transfer top bit to RAMPZ
      RAMPZ = (newAddress & 0x8000) ? 1 : 0;
#endif
      newAddress += newAddress; // Convert from word address to byte address
      address = newAddress;
      verifySpace();
    }
    else if(ch == STK_UNIVERSAL) {
      // UNIVERSAL command is ignored
      getNch(4);
      putch(0x00);
    }
    /* Write memory, length is big endian and is in bytes */
    else if(ch == STK_PROG_PAGE) {
      // PROGRAM PAGE - we support flash programming only, not EEPROM
      uint8_t *bufPtr;
      uint16_t addrPtr;

      getch();			/* getlen() */
      length = getch();
      getch();

      // If we are in RWW section, immediately start page erase
      if (address < NRWWSTART) __boot_page_erase_short((uint16_t)(void*)address);

      // While that is going on, read in page contents
      bufPtr = buff;
      do *bufPtr++ = getch();
      while (--length);

      // If we are in NRWW section, page erase has to be delayed until now.
      // Todo: Take RAMPZ into account (not doing so just means that we will
      //  treat the top of both "pages" of flash as NRWW, for a slight speed
      //  decrease, so fixing this is not urgent.)
      if (address >= NRWWSTART) __boot_page_erase_short((uint16_t)(void*)address);

      // Read command terminator, start reply
      verifySpace();

      // If only a partial page is to be programmed, the erase might not be complete.
      // So check that here
      boot_spm_busy_wait();

#ifdef VIRTUAL_BOOT_PARTITION
      if ((uint16_t)(void*)address == 0) {
        // This is the reset vector page. We need to live-patch the code so the
        // bootloader runs.
        //
        // Move RESET vector to WDT vector
        uint16_t vect = buff[0] | (buff[1]<<8);
        rstVect = vect;
        wdtVect = buff[8] | (buff[9]<<8);
        vect -= 4; // Instruction is a relative jump (rjmp), so recalculate.
        buff[8] = vect & 0xff;
        buff[9] = vect >> 8;

        // Add jump to bootloader at RESET vector
        buff[0] = 0x7f;
        buff[1] = 0xce; // rjmp 0x1d00 instruction
      }
#endif

      // Copy buffer into programming buffer
      bufPtr = buff;
      addrPtr = (uint16_t)(void*)address;
      ch = SPM_PAGESIZE / 2;
      do {
        uint16_t a;
        a = *bufPtr++;
        a |= (*bufPtr++) << 8;
        __boot_page_fill_short((uint16_t)(void*)addrPtr,a);
        addrPtr += 2;
      } while (--ch);

      // Write from programming buffer
      __boot_page_write_short((uint16_t)(void*)address);
      boot_spm_busy_wait();

#if defined(RWWSRE)
      // Reenable read access to flash
      boot_rww_enable();
#endif

    }
    /* Read memory block mode, length is big endian.  */
    else if(ch == STK_READ_PAGE) {
      // READ PAGE - we only read flash
      getch();			/* getlen() */
      length = getch();
      getch();

      verifySpace();
      do {
#ifdef VIRTUAL_BOOT_PARTITION
        // Undo vector patch in bottom page so verify passes
        if (address == 0)       ch=rstVect & 0xff;
        else if (address == 1)  ch=rstVect >> 8;
        else if (address == 8)  ch=wdtVect & 0xff;
        else if (address == 9) ch=wdtVect >> 8;
        else ch = pgm_read_byte_near(address);
        address++;
#elif defined(RAMPZ)
        // Since RAMPZ should already be set, we need to use EPLM directly.
        // Also, we can use the autoincrement version of lpm to update "address"
        //      do putch(pgm_read_byte_near(address++));
        //      while (--length);
        // read a Flash and increment the address (may increment RAMPZ)
        __asm__ ("elpm %0,Z+\n" : "=r" (ch), "=z" (address): "1" (address));
#else
        // read a Flash byte and increment the address
        __asm__ ("lpm %0,Z+\n" : "=r" (ch), "=z" (address): "1" (address));
#endif
        putch(ch);
      } while (--length);
    }

    /* Get device signature bytes  */
    else if(ch == STK_READ_SIGN) {
      // READ SIGN - return what Avrdude wants to hear
      verifySpace();
      putch(SIGNATURE_0);
      putch(SIGNATURE_1);
      putch(SIGNATURE_2);
    }
    else if (ch == STK_LEAVE_PROGMODE) { /* 'Q' */
      // Adaboot no-wait mod
      // MOG TURN OFF WATCHDOG
      //           watchdogConfig(WATCHDOG_64MS);
      verifySpace();
    }
    else {
      // This covers the response to commands like STK_ENTER_PROGMODE
      verifySpace();
    }
    putch(STK_OK);
  }
}

//***************************************************************************************
// receiveFrame()
//
// This routine receives a differential manchester coded signal at the input pin.
// The routine waits for a toggling voltage level. 
// It automatically detects the transmission speed.
//
// output: 		uint8_t flag: true: checksum ok
//				Data // global variable
// 
//***************************************************************************************
uint8_t receiveFrame()
{
  uint16_t counter=0;
  volatile uint16_t time=0;
  volatile uint16_t delayTime;
  uint8_t p,t;
  uint8_t k=8;
  uint8_t dataPointer=0;
  uint16_t n;

  //*** synchronisation and bit rate estimation **************************
  time=0;
  // wait for edge
  p=PINVALUE;
  while(p==PINVALUE);

  p=PINVALUE;
  
  TIMER=0; // reset timer
  for(n=0;n<16;n++)
  {
    // wait for edge
	while(p==PINVALUE);
	t=TIMER;
	TIMER=0; // reset timer
    p=PINVALUE;

    if(n>=8)time+=t; // time accumulator for mean period calculation only the last 8 times are used 
  }
  
  delayTime=time*3/4/8;
  // delay 3/4 bit
  while(TIMER<delayTime);

  //p=1;
  
  //****************** wait for start bit ***************************
  while(p==PINVALUE) // while not startbit ( no change of pinValue means 0 bit )
  {
    // wait for edge
	while(p==PINVALUE);
    p=PINVALUE;
	TIMER=0;

    // delay 3/4 bit
    while(TIMER<delayTime);
	TIMER=0;

    counter++;
  }
  p=PINVALUE;
  //****************************************************************
  //receive data bits
  k=8;
  for(n=0;n<(FRAMESIZE*8);n++)
  {
	// wait for edge
	while(p==PINVALUE);
    TIMER=0;
	p=PINVALUE;
    
    // delay 3/4 bit
    while(TIMER<delayTime);
	
	t=PINVALUE;

    counter++;
    
    FrameData[dataPointer]=FrameData[dataPointer]<<1;
    if(p!=t) FrameData[dataPointer]|=1;
	p=t;
    k--;
    if(k==0){dataPointer++;k=8;};
  }
    uint16_t crc=(uint16_t)FrameData[CRCLOW]+FrameData[CRCHIGH]*256;

  if(crc==0x55AA) return true;
  else return false;
}

//***************************************************************************************
//	void boot_program_page (uint32_t page, uint8_t *buf)
//
//  Erase and flash one page.
//
//  inputt: 		page address and data to be programmed
// 
//***************************************************************************************
void boot_program_page (uint32_t page, uint8_t *buf)
{
	uint16_t i;
  
    cli(); // disable interrupts

    boot_page_erase (page);
    boot_spm_busy_wait ();      // Wait until the memory is erased.

    for (i=0; i<SPM_PAGESIZE; i+=2)
    {
        uint16_t w = *buf++;
        w += (*buf++) << 8;
        
        boot_page_fill (page + i, w);
    	boot_spm_busy_wait();       // Wait until the memory is written.
	}

    boot_page_write (page);     // Store buffer in flash page.
    boot_spm_busy_wait();       // Wait until the memory is written.
	
    boot_rww_enable ();
}
//***************************************************************************************
void initstart()
{
   // Timer 2 normal mode, clk/8, count up from 0 to 255  
   // ==> frequency @16MHz= 16MHz/8/256=7812.5Hz

   TCCR2B= _BV(CS21);
}
//***************************************************************************************
void runProgramm(void)
{
	// reintialize registers to default
	DDRB=0;
	DDRC=0;
	DDRD=0;
	cli();

	TCCR2B=0; // turn off timer2

	// start user programm
	asm volatile(		
	"clr r30	\n\t"
	"clr r31	\n\t"	// z Register mit Adresse laden
	"ijmp		\n\t"	// z Register mit Adresse laden
	);
}

//***************************************************************************************
// main loop
//***************************************************************************************
void a_main()
{
  initstart();


  uint8_t p = -1;
  #define WAITBLINKTIME 10000
  uint16_t time=WAITBLINKTIME;
  uint8_t timeout=6;
 
  //*************** wait for toggling input pin or timeout ******************************
  uint8_t exitcounter=3;
  while(1)
  {  

    if(TIMER>100)  // timedelay ==> frequency @16MHz= 16MHz/8/100=20kHz
    {
       TIMER=0;
       time--;
       if(time==0)
       {
         TOGGLELED;

		 time=WAITBLINKTIME;
         timeout--;
         if(timeout==0)
         {

           LEDOFF; // timeout,
           // leave bootloader and run program
		   runProgramm();
         }
       }
    }
    if(p!=PINVALUE)
    {
	//setSpalte(3);
      p=PINVALUE;
      exitcounter--;
    }
    if(exitcounter==0) break; // signal received, leave this loop and go on
  }
  //*************** start command interpreter *************************************  
  LEDON;
  while(1)
  {
    if(!receiveFrame())
    {
      //*****  error: blink fast, press reset to restart *******************
      while(1)
      {         
        if(TIMER>100)  // timerstop ==> frequency @16MHz= 16MHz/8/100=20kHz
        {
           TIMER=0;
           time--;
           if(time==0)
           {
             TOGGLELED;
             time=1000;
           }
        }
      }
    }
    else // succeed
    {
      TOGGLELED;
      switch(FrameData[COMMAND])
      {
        case TESTCOMMAND: // not used yet
        {

        }
        break;
        case RUNCOMMAND:
        {
		//setExternalPort(2);

          // leave bootloader and run program
		runProgramm();

        }
        break;
        case PROGCOMMAND:
        { 
  			// Atmega168 Pagesize=64 Worte=128 Byte
			uint16_t k;
			k=(((uint16_t)FrameData[PAGEINDEXHIGH])<<8)+FrameData[PAGEINDEXLOW];
			boot_program_page (SPM_PAGESIZE*k, FrameData+DATAPAGESTART);	// erase and programm page
        }
        break;
      }
      FrameData[COMMAND]=NOCOMMAND; // delete command
    }
  }
}
//AUDIOBOOT

//optiboot

void putch(char ch) {
#ifndef SOFT_UART
  while (!(UART_SRA & _BV(UDRE0)));
  UART_UDR = ch;
#else
  __asm__ __volatile__ (
    "   com %[ch]\n" // ones complement, carry set
    "   sec\n"
    "1: brcc 2f\n"
    "   cbi %[uartPort],%[uartBit]\n"
    "   rjmp 3f\n"
    "2: sbi %[uartPort],%[uartBit]\n"
    "   nop\n"
    "3: rcall uartDelay\n"
    "   rcall uartDelay\n"
    "   lsr %[ch]\n"
    "   dec %[bitcnt]\n"
    "   brne 1b\n"
    :
    :
      [bitcnt] "d" (10),
      [ch] "r" (ch),
      [uartPort] "I" (_SFR_IO_ADDR(UART_PORT)),
      [uartBit] "I" (UART_TX_BIT)
    :
      "r25"
  );
#endif
}

uint8_t getch(void) {
  uint8_t ch;

#ifdef LED_DATA_FLASH
#if defined(__AVR_ATmega8__) || defined (__AVR_ATmega32__)
  LED_PORT ^= _BV(LED);
#else
  LED_PIN |= _BV(LED);
#endif
#endif


  while(!(UART_SRA & _BV(RXC0))) {
    ;
  }
  if (!(UART_SRA & _BV(FE0))) {
      /*
       * A Framing Error indicates (probably) that something is talking
       * to us at the wrong bit rate.  Assume that this is because it
       * expects to be talking to the application, and DON'T reset the
       * watchdog.  This should cause the bootloader to abort and run
       * the application "soon", if it keeps happening.  (Note that we
       * don't care that an invalid char is returned...)
       */
    watchdogReset();
  }
  
  ch = UART_UDR;

#ifdef LED_DATA_FLASH
#if defined(__AVR_ATmega8__) || defined (__AVR_ATmega32__)
  LED_PORT ^= _BV(LED);
#else
  LED_PIN |= _BV(LED);
#endif
#endif

  return ch;
}

#ifdef SOFT_UART
// AVR305 equation: #define UART_B_VALUE (((F_CPU/BAUD_RATE)-23)/6)
// Adding 3 to numerator simulates nearest rounding for more accurate baud rates
#define UART_B_VALUE (((F_CPU/BAUD_RATE)-20)/6)
#if UART_B_VALUE > 255
#error Baud rate too slow for soft UART
#endif

void uartDelay() {
  __asm__ __volatile__ (
    "ldi r25,%[count]\n"
    "1:dec r25\n"
    "brne 1b\n"
    "ret\n"
    ::[count] "M" (UART_B_VALUE)
  );
}
#endif

void getNch(uint8_t count) {
  do getch(); while (--count);
  verifySpace();
}

void wait_timeout(void) {
#ifdef OLD
  DDRD|=(1<<PD4);
  PORTD |= (1<<PD4);          // turn on the LED to indicate receiving data
#else
  DDRC|=(1<<PC1);
  PORTC |= (1<<PC1);          // turn on the LED to indicate receiving data
#endif
  //  watchdogConfig(WATCHDOG_64MS);    // shorten WD timeout
  while (1)			      // and busy-loop so that WD causes
    ;				      //  a reset and app start.
}

void verifySpace() {
  if (getch() != CRC_EOP) {
    wait_timeout();
  }
  putch(STK_INSYNC);
}

#if LED_START_FLASHES > 0
void flash_led(uint8_t count) {
  do {
    TCNT1 = -(F_CPU/(1024*16));
    TIFR1 = _BV(TOV1);
    while(!(TIFR1 & _BV(TOV1)));
#if defined(__AVR_ATmega8__)  || defined (__AVR_ATmega32__)
    LED_PORT ^= _BV(LED);
#else
    LED_PIN |= _BV(LED);
#endif
    watchdogReset();
  } while (--count);
}
#endif

// Watchdog functions. These are only safe with interrupts turned off.
void watchdogReset() {
  __asm__ __volatile__ (
    "wdr\n"
  );
}

void watchdogConfig(uint8_t x) {
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = x;
}

void appStart(uint8_t rstFlags) {
  // save the reset flags in the designated register
  //  This can be saved in a main program by putting code in .init0 (which
  //  executes before normal c init code) to save R2 to a global variable.
  __asm__ __volatile__ ("mov r2, %0\n" :: "r" (rstFlags));

  watchdogConfig(WATCHDOG_OFF);
  __asm__ __volatile__ (
#ifdef VIRTUAL_BOOT_PARTITION
    // Jump to WDT vector
    "ldi r30,4\n"
    "clr r31\n"
#else
    // Jump to RST vector
    "clr r30\n"
    "clr r31\n"
#endif
    "ijmp\n"
  );
}

//optiboot
