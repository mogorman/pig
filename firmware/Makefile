ARDUINO=~/nonapt/arduino-1.5.6-r2/arduino
AVRDUDE=/usr/bin/avrdude
AVR-GCC=/usr/bin/avr-gcc
AVR-OBJCOPY=/usr/bin/avr-objcopy

all: compile_boot compile_app

compile_app:
	$(ARDUINO) \
	 --verbose-build \
	 --board arduino:avr:fio \
	 --pref build.path=`pwd`/build \
	 --pref sketchbook.path=`pwd` \
	 --verify `pwd`/application/application.ino
	@cp build/application.cpp.hex ./bin/application.hex


compile_boot:
	$(AVR-GCC) -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' '-DNUM_LED_FLASHES=5'   -c -o ./build/ATmegaBOOT_xx8.o ./bootloader/ATmegaBOOT_xx8.c
	$(AVR-GCC) -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' '-DNUM_LED_FLASHES=5' -Wl,--section-start=.text=0x7800 -o ./build/ATmegaBOOT_xx8_mog.elf ./build/ATmegaBOOT_xx8.o 
	$(AVR-OBJCOPY) -j .text -j .data -O ihex ./build/ATmegaBOOT_xx8_mog.elf ./build/ATmegaBOOT_xx8_mog.hex
	cp ./build/ATmegaBOOT_xx8_mog.hex ./bin/bootloader.hex

bootloader: compile_boot
	$(AVRDUDE) -c buspirate -p m328p -P /dev/buspirate -e -U lock:w:0x3f:m 
	$(AVRDUDE) -v -c buspirate -p m328p -P /dev/buspirate \
	 -U efuse:w:0x07:m \
	 -U hfuse:w:0xDA:m \
	 -U lfuse:w:0xE2:m \
	 -U  flash:w:./bin/bootloader.hex -U lock:w:0x0F:m

app: compile_app
	stty -F /dev/arduino hupcl 
	sleep 2
	$(AVRDUDE) -pm328p -c stk500v1 -P/dev/arduino -b9600 -U flash:w:./bin/application.hex
clean:
	rm -f build/*
	rm -f bin/*
