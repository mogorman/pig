#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include <stdlib.h>
#include <avr/boot.h>

/* Use the F_CPU defined in Makefile */

/* 20060803: hacked by DojoCorp */
/* 20070626: hacked by David A. Mellis to decrease waiting time for auto-reset */
/* set the waiting time for the bootloader */
/* get this from the Makefile instead */
/* #define MAX_TIME_COUNT (F_CPU>>4) */

/* 20070707: hacked by David A. Mellis - after this many errors give up and launch application */
#define MAX_ERROR_COUNT 5

/* set the UART baud rate */
/* 20060803: hacked by DojoCorp */
//#define BAUD_RATE   115200
//#define BAUD_RATE   19200
#define BAUD_RATE   9600

/* SW_MAJOR and MINOR needs to be updated from time to time to avoid warning message from AVR Studio */
/* never allow AVR Studio to do an update !!!! */
#define HW_VER	 0x02
#define SW_MAJOR 0x01
#define SW_MINOR 0x10

#define SIG1	0x1E	// Yep, Atmel is the only manufacturer of AVR micros.  Single source :(

#if defined __AVR_ATmega168__
#define SIG2	0x94
#define SIG3	0x06
#define PAGE_SIZE	0x40U	//64 words

#elif defined __AVR_ATmega328P__
#define SIG2	0x95
#define SIG3	0x0F
#define PAGE_SIZE	0x40U	//64 words
#endif


//MOG
/* onboard LED is used to indicate, that the bootloader was entered (3x flashing) */
/* if monitor functions are included, LED goes on after monitor was entered */

/* Onboard LED is connected to pin PB2 (e.g. Crumb8, Crumb168) */
#define LED_DDR  DDRD
#define LED_PORT PORTD
#define LED_PIN  PIND
/* 20060803: hacked by DojoCorp, LED pin is B5 in Arduino */
/* #define LED      PINB2 */
#define LED      PIND4


/* other ATmegas have only one UART, so only one pin is defined to enter bootloader */
#define BL_DDR  DDRD
#define BL_PORT PORTD
#define BL_PIN  PIND
#define BL      PIND6

#define LEDPORT (1<<PD4);
#define INITLED {DDRD|=LEDPORT;}
#define LEDON {PORTD|=LEDPORT;}
#define LEDOFF {PORTD&=~LEDPORT;}
#define TOGGLELED {PORTD^=LEDPORT;}

	// use TxD ( PD1 ) as audio input	
#define INPUTAUDIOPIN (1<<PD3)
#define PINVALUE (PIND&INPUTAUDIOPIN)
#define INITPORT {PORTD|=INPUTAUDIOPIN; } //turn on pull up 

#define PINLOW (PINVALUE==0)
#define PINHIGH (!PINLOW)


#define true (1==1)
#define false !true


/* function prototypes */
/* void putch(char); */
/* char getch(void); */
/* void getNch(uint8_t); */
/* void byte_response(uint8_t); */
/* void nothing_response(void); */
/* char gethex(void); */
/* void puthex(char); */
/* void flash_led(uint8_t); */
/* void u_main(); */
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


void (*app_start)(void) = 0x0000;



int main()
{
  INITLED;
  INITPORT;
  //u_main();
  a_main(); // start the main function
  return 0;
}


//void u_main()
//{
//    uint8_t ch,ch2;
//    uint16_t w;
//    uint8_t firstchar = 0; // make sure we dont start bootloader by accident
//
//#ifdef WATCHDOG_MODS
//    ch = MCUSR;
//    MCUSR = 0;
//
//    WDTCSR |= _BV(WDCE) | _BV(WDE);
//    WDTCSR = 0;
//
//    // Check if the WDT was used to reset, in which case we dont bootload and skip straight to the code. woot.
//    if (! (ch &  _BV(EXTRF))) // if its a not an external reset...
//      app_start();  // skip bootloader
//#else
//    asm volatile("nop\n\t");
//#endif
//
//
//
//
//    /* check if flash is programmed already, if not start bootloader anyway */
//    if(pgm_read_byte_near(0x0000) != 0xFF) {
//
//
//    }
//
//
//
//    UBRR0L = (uint8_t)(F_CPU/(BAUD_RATE*16L)-1);
//    //UBRR0H = (F_CPU/(BAUD_RATE*16L)-1) >> 8;     // always 0
//    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
//    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
//
//    /* Enable internal pull-up resistor on pin D0 (RX), in order
//    to supress line noise that prevents the bootloader from
//    timing out (DAM: 20070509) */
//    DDRD &= ~_BV(PIND0);
//    PORTD |= _BV(PIND0);
//
//    /* set LED pin as output */
//    LED_DDR |= _BV(LED);
//
//
//    /* flash onboard LED to signal entering of bootloader */
//
//    flash_led(NUM_LED_FLASHES);
//
//    
//    /* 20050803: by DojoCorp, this is one of the parts provoking the
//                 system to stop listening, cancelled from the original */
//    //putch('\0');
//
//
//    /* forever loop */
//    for (;;) {
//
//	/* get character from UART */
//	ch = getch();
//
//	/* A bunch of if...else if... gives smaller code than switch...case ! */
//
//	/* Hello is anyone home ? */ 
//	if(ch=='0') {
//	  firstchar = 1; // we got an appropriate bootloader instruction
//	  nothing_response();
//	} else if (firstchar == 0) {
//	  // the first character we got is not '0', lets bail!
//	  // autoreset via watchdog (sneaky!)
//	  WDTCSR = _BV(WDE);
//	  while (1); // 16 ms
//	}
//
//	/* Request programmer ID */
//	/* Not using PROGMEM string due to boot block in m128 being beyond 64kB boundry  */
//	/* Would need to selectively manipulate RAMPZ, and it's only 9 characters anyway so who cares.  */
//	else if(ch=='1') {
//	    if (getch() == ' ') {
//		putch(0x14);
//		putch('A');
//		putch('V');
//		putch('R');
//		putch(' ');
//		putch('I');
//		putch('S');
//		putch('P');
//		putch(0x10);
//	    } else {
//		if (++error_count == MAX_ERROR_COUNT)
//		    app_start();
//	    }
//	}
//
//
//	/* AVR ISP/STK500 board commands  DON'T CARE so default nothing_response */
//	else if(ch=='@') {
//	    ch2 = getch();
//	    if (ch2>0x85) getch();
//	    nothing_response();
//	}
//
//
//	/* AVR ISP/STK500 board requests */
//	else if(ch=='A') {
//	    ch2 = getch();
//	    if(ch2==0x80) byte_response(HW_VER);		// Hardware version
//	    else if(ch2==0x81) byte_response(SW_MAJOR);	// Software major version
//	    else if(ch2==0x82) byte_response(SW_MINOR);	// Software minor version
//	    else if(ch2==0x98) byte_response(0x03);		// Unknown but seems to be required by avr studio 3.56
//	    else byte_response(0x00);				// Covers various unnecessary responses we don't care about
//	}
//
//
//	/* Device Parameters  DON'T CARE, DEVICE IS FIXED  */
//	else if(ch=='B') {
//	    getNch(20);
//	    nothing_response();
//	}
//
//
//	/* Parallel programming stuff  DON'T CARE  */
//	else if(ch=='E') {
//	    getNch(5);
//	    nothing_response();
//	}
//
//
//	/* Enter programming mode  */
///* Erase device, don't care as we will erase one page at a time anyway.  */
//	else if(ch=='P' || ch == 'R') {
//	    nothing_response();
//	}
//
//
//	/* Leave programming mode  */
//	else if(ch=='Q') {
//	    nothing_response();
//#ifdef WATCHDOG_MODS
//	    // autoreset via watchdog (sneaky!)
//	    WDTCSR = _BV(WDE);
//	    while (1); // 16 ms
//#endif
//	}
//	
//
//	/* Set address, little endian. EEPROM in bytes, FLASH in words  */
//	/* Perhaps extra address bytes may be added in future to support > 128kB FLASH.  */
//	/* This might explain why little endian was used here, big endian used everywhere else.  */
//	else if(ch=='U') {
//	    address.byte[0] = getch();
//	    address.byte[1] = getch();
//	    nothing_response();
//	}
//
//
//	/* Universal SPI programming command, disabled.  Would be used for fuses and lock bits.  */
//	else if(ch=='V') {
//	  if (getch() == 0x30) {
//	    getch();
//	    ch = getch();
//	    getch();
//	    if (ch == 0) {
//	      byte_response(SIG1);
//	    } else if (ch == 1) {
//	      byte_response(SIG2); 
//	    } else {
//	      byte_response(SIG3);
//	    } 
//	  } else {
//	    getNch(3);
//	    byte_response(0x00);
//	  }
//	}
//
//
//	/* Write memory, length is big endian and is in bytes  */
//	else if(ch=='d') {
//	    length.byte[1] = getch();
//	    length.byte[0] = getch();
//	    flags.eeprom = 0;
//	    if (getch() == 'E') flags.eeprom = 1;
//	    for (w=0;w<length.word;w++) {
//		buff[w] = getch();	                        // Store data in buffer, can't keep up with serial data stream whilst programming pages
//	    }
//	    if (getch() == ' ') {
//		if (flags.eeprom) {		                //Write to EEPROM one byte at a time
//		  address.word <<= 1;
//		  for(w=0;w<length.word;w++) {
//		    while(EECR & (1<<EEPE));
//		    EEAR = (uint16_t)(void *)address.word;
//		    EEDR = buff[w];
//		    EECR |= (1<<EEMPE);
//		    EECR |= (1<<EEPE);
//
//		    address.word++;
//		  }			
//		}
//		else {					        //Write to FLASH one page at a time
//		    if (address.byte[1]>127) address_high = 0x01;	//Only possible with m128, m256 will need 3rd address byte. FIXME
//		    else address_high = 0x00;
//
//		    address.word = address.word << 1;	        //address * 2 -> byte location
//		    /* if ((length.byte[0] & 0x01) == 0x01) length.word++;	//Even up an odd number of bytes */
//		    if ((length.byte[0] & 0x01)) length.word++;	//Even up an odd number of bytes
//		    cli();					//Disable interrupts, just to be sure
//			// HACKME: EEPE used to be EEWE
//		    while(bit_is_set(EECR,EEPE));			//Wait for previous EEPROM writes to complete
//		    asm volatile(
//				 "clr	r17		\n\t"	//page_word_count
//				 "lds	r30,address	\n\t"	//Address of FLASH location (in bytes)
//				 "lds	r31,address+1	\n\t"
//				 "ldi	r28,lo8(buff)	\n\t"	//Start of buffer array in RAM
//				 "ldi	r29,hi8(buff)	\n\t"
//				 "lds	r24,length	\n\t"	//Length of data to be written (in bytes)
//				 "lds	r25,length+1	\n\t"
//				 "length_loop:		\n\t"	//Main loop, repeat for number of words in block							 							 
//				 "cpi	r17,0x00	\n\t"	//If page_word_count=0 then erase page
//				 "brne	no_page_erase	\n\t"						 
//				 "wait_spm1:		\n\t"
//				 "lds	r16,%0		\n\t"	//Wait for previous spm to complete
//				 "andi	r16,1           \n\t"
//				 "cpi	r16,1           \n\t"
//				 "breq	wait_spm1       \n\t"
//				 "ldi	r16,0x03	\n\t"	//Erase page pointed to by Z
//				 "sts	%0,r16		\n\t"
//				 "spm			\n\t"							 
//
//				 "wait_spm2:		\n\t"
//				 "lds	r16,%0		\n\t"	//Wait for previous spm to complete
//				 "andi	r16,1           \n\t"
//				 "cpi	r16,1           \n\t"
//				 "breq	wait_spm2       \n\t"									 
//
//				 "ldi	r16,0x11	\n\t"	//Re-enable RWW section
//				 "sts	%0,r16		\n\t"						 			 
//				 "spm			\n\t"
//
//				 "no_page_erase:		\n\t"							 
//				 "ld	r0,Y+		\n\t"	//Write 2 bytes into page buffer
//				 "ld	r1,Y+		\n\t"							 
//							 
//				 "wait_spm3:		\n\t"
//				 "lds	r16,%0		\n\t"	//Wait for previous spm to complete
//				 "andi	r16,1           \n\t"
//				 "cpi	r16,1           \n\t"
//				 "breq	wait_spm3       \n\t"
//				 "ldi	r16,0x01	\n\t"	//Load r0,r1 into FLASH page buffer
//				 "sts	%0,r16		\n\t"
//				 "spm			\n\t"
//							 
//				 "inc	r17		\n\t"	//page_word_count++
//				 "cpi r17,%1	        \n\t"
//				 "brlo	same_page	\n\t"	//Still same page in FLASH
//				 "write_page:		\n\t"
//				 "clr	r17		\n\t"	//New page, write current one first
//				 "wait_spm4:		\n\t"
//				 "lds	r16,%0		\n\t"	//Wait for previous spm to complete
//				 "andi	r16,1           \n\t"
//				 "cpi	r16,1           \n\t"
//				 "breq	wait_spm4       \n\t"
//				 							 
//				 "ldi	r16,0x05	\n\t"	//Write page pointed to by Z
//				 "sts	%0,r16		\n\t"
//				 "spm			\n\t"
//
//				 "wait_spm5:		\n\t"
//				 "lds	r16,%0		\n\t"	//Wait for previous spm to complete
//				 "andi	r16,1           \n\t"
//				 "cpi	r16,1           \n\t"
//				 "breq	wait_spm5       \n\t"									 
//				 "ldi	r16,0x11	\n\t"	//Re-enable RWW section
//				 "sts	%0,r16		\n\t"						 			 
//				 "spm			\n\t"					 		 
//
//				 "same_page:		\n\t"							 
//				 "adiw	r30,2		\n\t"	//Next word in FLASH
//				 "sbiw	r24,2		\n\t"	//length-2
//				 "breq	final_write	\n\t"	//Finished
//				 "rjmp	length_loop	\n\t"
//				 "final_write:		\n\t"
//				 "cpi	r17,0		\n\t"
//				 "breq	block_done	\n\t"
//				 "adiw	r24,2		\n\t"	//length+2, fool above check on length after short page write
//				 "rjmp	write_page	\n\t"
//				 "block_done:		\n\t"
//				 "clr	__zero_reg__	\n\t"	//restore zero register
//				 : "=m" (SPMCSR) : "M" (PAGE_SIZE) : "r0","r16","r17","r24","r25","r28","r29","r30","r31"
//
//				 );
//		    /* Should really add a wait for RWW section to be enabled, don't actually need it since we never */
//		    /* exit the bootloader without a power cycle anyhow */
//		}
//		putch(0x14);
//		putch(0x10);
//	    } else {
//		if (++error_count == MAX_ERROR_COUNT)
//		    app_start();
//	    }		
//	}
//    
//
//        /* Read memory block mode, length is big endian.  */
//        else if(ch=='t') {
//	    length.byte[1] = getch();
//	    length.byte[0] = getch();
//
//	    address.word = address.word << 1;	        // address * 2 -> byte location
//	    if (getch() == 'E') 
//	      flags.eeprom = 1;
//	    else
//	      flags.eeprom = 0;
//	    if (getch() == ' ') {		                // Command terminator
//		putch(0x14);
//		for (w=0;w < length.word;w++) {		        // Can handle odd and even lengths okay
//		    if (flags.eeprom) {	                        // Byte access EEPROM read
//			while(EECR & (1<<EEPE));
//			EEAR = (uint16_t)(void *)address.word;
//			EECR |= (1<<EERE);
//			putch(EEDR);
//
//			address.word++;
//		    }
//		    else {
//
//			if (!flags.rampz) putch(pgm_read_byte_near(address.word));
//
//			address.word++;
//		    }
//		}
//		putch(0x10);
//	    }
//	}
//
//
//        /* Get device signature bytes  */
//        else if(ch=='u') {
//	    if (getch() == ' ') {
//		putch(0x14);
//		putch(SIG1);
//		putch(SIG2);
//		putch(SIG3);
//		putch(0x10);
//	    } else {
//		if (++error_count == MAX_ERROR_COUNT)
//		    app_start();
//	    }
//	}
//
//
//        /* Read oscillator calibration byte */
//        else if(ch=='v') {
//	    byte_response(0x00);
//	}
//
//
//#ifdef MONITOR
//
//	/* here come the extended monitor commands by Erik Lins */
//
//	/* check for three times exclamation mark pressed */
//	else if(ch=='!') {
//	    ch = getch();
//	    if(ch=='!') {
//		ch = getch();
//		if(ch=='!') {
//
//
//		    uint8_t addrl, addrh;
//
//
//		    /* turn on LED */
//		    LED_DDR |= _BV(LED);
//		    LED_PORT &= ~_BV(LED);
//
//		    /* print a welcome message and command overview */
//		    for(i=0; welcome[i] != '\0'; ++i) {
//			putch(welcome[i]);
//		    }
//
//		    /* test for valid commands */
//		    for(;;) {
//			putch('\n');
//			putch('\r');
//			putch(':');
//			putch(' ');
//
//			ch = getch();
//			putch(ch);
//
//			/* toggle LED */
//			if(ch == 't') {
//			    if(bit_is_set(LED_PIN,LED)) {
//				LED_PORT &= ~_BV(LED);
//				putch('1');
//			    } else {
//				LED_PORT |= _BV(LED);
//				putch('0');
//			    }
//
//			} 
//
//			/* read byte from address */
//			else if(ch == 'r') {
//			    ch = getch(); putch(ch);
//			    addrh = gethex();
//			    addrl = gethex();
//			    putch('=');
//			    ch = *(uint8_t *)((addrh << 8) + addrl);
//			    puthex(ch);
//			}
//
//			/* write a byte to address  */
//			else if(ch == 'w') {
//			    ch = getch(); putch(ch);
//			    addrh = gethex();
//			    addrl = gethex();
//			    ch = getch(); putch(ch);
//			    ch = gethex();
//			    *(uint8_t *)((addrh << 8) + addrl) = ch;
//
//			}
//
//			/* read from uart and echo back */
//			else if(ch == 'u') {
//			    for(;;) {
//				putch(getch());
//			    }
//			}
//
//			else if(ch == 'j') {
//			    app_start();
//			}
//
//		    }
//		    /* end of monitor functions */
//
//		}
//	    }
//	}
//	/* end of monitor */
//#endif
//	else if (++error_count == MAX_ERROR_COUNT) {
//	    app_start();
//	}
//    }
//    /* end of forever loop */
//
//}


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


//char gethexnib(void) {
//  char a;
//  a = getch(); putch(a);
//  if(a >= 'a') {
//    return (a - 'a' + 0x0a);
//  } else if(a >= '0') {
//    return(a - '0');
//  }
//  //MOG
//  return 0;
//  //MOG
//}
//
//char gethex(void) {
//  return (gethexnib() << 4) + gethexnib();
//}
//
//void puthex(char ch) {
//    char ah;
//
//    ah = ch >> 4;
//    if(ah >= 0x0a) {
//	ah = ah - 0x0a + 'a';
//    } else {
//	ah += '0';
//    }
//    ch &= 0x0f;
//    if(ch >= 0x0a) {
//	ch = ch - 0x0a + 'a';
//    } else {
//	ch += '0';
//    }
//    putch(ah);
//    putch(ch);
//}
//
//
//void putch(char ch)
//{
//
//    while (!(UCSR0A & _BV(UDRE0)));
//    UDR0 = ch;
//}
//
//
//char getch(void)
//{
//
//    uint32_t count = 0;
//
//    LED_PORT &= ~_BV(LED);
//
//    while(!(UCSR0A & _BV(RXC0))){
//    	/* 20060803 DojoCorp:: Addon coming from the previous Bootloader*/               
//    	/* HACKME:: here is a good place to count times*/
//    	count++;
//    	if (count > MAX_TIME_COUNT)
//    		app_start();
//     }
//
//    LED_PORT |= _BV(LED);          // turn on the LED to indicate receiving data
//    return UDR0;
//}
//
//
//void getNch(uint8_t count)
//{
//  while (count--) {
//
//	while(!(UCSR0A & _BV(RXC0)));
//	UDR0;
//    }
//}
//
//
//void byte_response(uint8_t val)
//{
//    if (getch() == ' ') {
//	putch(0x14);
//	putch(val);
//	putch(0x10);
//    } else {
//	if (++error_count == MAX_ERROR_COUNT)
//	    app_start();
//    }
//}
//
//
//void nothing_response(void)
//{
//    if (getch() == ' ') {
//	putch(0x14);
//	putch(0x10);
//    } else {
//	if (++error_count == MAX_ERROR_COUNT)
//	    app_start();
//    }
//}
//
//void flash_led(uint8_t count)
//{
//  /* flash onboard LED three times to signal entering of bootloader */
//  /* l needs to be volatile or the delay loops below might get
//     optimized away if compiling with optimizations (DAM). */
//  //volatile uint32_t l;
//
//    if (count == 0) {
//      count = 3;
//    }
//    
//    while (count--) {
//	LED_PORT |= _BV(LED);
//	//for(l = 0; l < (F_CPU / 1000); ++l);
//	_delay_ms(100);
//	LED_PORT &= ~_BV(LED);
//	//for(l = 0; l < (F_CPU / 1000); ++l);
//	_delay_ms(100);
//    }
//}
//
//
/* end of file ATmegaBOOT.c */
