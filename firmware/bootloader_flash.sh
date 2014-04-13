#!/bin/bash
rm -f ATmegaBOOT_xx8_mog.hex
avr-gcc -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' '-DNUM_LED_FLASHES=5'   -c -o ATmegaBOOT_xx8.o ATmegaBOOT_xx8.c
avr-gcc -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' '-DNUM_LED_FLASHES=5' -Wl,--section-start=.text=0x7800 -o ATmegaBOOT_xx8_mog.elf ATmegaBOOT_xx8.o 
avr-objcopy -j .text -j .data -O ihex ATmegaBOOT_xx8_mog.elf ATmegaBOOT_xx8_mog.hex
rm ATmegaBOOT_xx8.o ATmegaBOOT_xx8_mog.elf
sleep 3
avrdude -c buspirate -p m328p -P /dev/buspirate -e -U lock:w:0x3f:m 
avrdude -c buspirate -p m328p -P /dev/buspirate -U efuse:w:0x07:m -U hfuse:w:0xDA:m -U lfuse:w:0xE2:m -U  flash:w:ATmegaBOOT_xx8_mog.hex -U lock:w:0x0F:m

