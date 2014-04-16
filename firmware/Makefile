ARDUINO=~/nonapt/arduino-1.5.6-r2/arduino
AVRDUDE=/usr/bin/avrdude
AVR-GCC=/usr/bin/avr-gcc
AVR-OBJCOPY=/usr/bin/avr-objcopy
PROGRAMMER=buspirate
PROGRAMMER_PORT=/dev/buspirate
ARDUINO_PORT=/dev/arduino

all:  compile_app compile_boot

compile_app:
	@md5sum application/application.ino | cmp -s build/app.md5 -; \
	 if test $$? -ne 0; then \
	  $(ARDUINO) \
	   --verbose-build \
	   --board arduino:avr:fio \
	   --pref build.path=`pwd`/build \
	   --pref sketchbook.path=`pwd` \
	   --verify `pwd`/application/application.ino; \
	  md5sum application/application.ino > build/app.md5; \
	fi
	@touch build/.git_empty
	@cp build/application.cpp.hex ./bin/application.hex


compile_boot:
	md5sum bootloader/bootloader.c | cmp -s build/bootloader.md5 -; \
	 if test $$? -ne 0; then \
	   md5sum bootloader/bootloader.c > build/bootloader.md5; \
	$(AVR-GCC) -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' '-DNUM_LED_FLASHES=5'   -c -o ./build/bootloader.o ./bootloader/bootloader.c; \
	$(AVR-GCC) -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' '-DNUM_LED_FLASHES=5' -Wl,--section-start=.text=0x7800 -o ./build/bootloader.elf ./build/bootloader.o; \
	$(AVR-OBJCOPY) -j .text -j .data -O ihex ./build/bootloader.elf ./build/bootloader.hex; \
	fi
	cp ./build/bootloader.hex ./bin/bootloader.hex

bootloader: compile_boot
	$(AVRDUDE) -c $(PROGRAMMER) -p m328p -P $(PROGRAMMER_PORT) -e -U lock:w:0x3f:m 
	$(AVRDUDE) -v -c $(PROGRAMMER) -p m328p -P $(PROGRAMMER_PORT) \
	 -U lfuse:w:0xE2:m \
	 -U hfuse:w:0xDA:m \
	 -U efuse:w:0x07:m \
	 -U  flash:w:./bin/bootloader.hex -U lock:w:0x0F:m

app: compile_app
	stty -F $(ARDUINO_PORT) hupcl 
	sleep 2
	$(AVRDUDE) -pm328p -c stk500v1 -P $(ARDUINO_PORT) -b9600 -U flash:w:./bin/application.hex; \
	if test $$? -ne 0; then \
	  stty -F /dev/arduino hupcl; \
	  sleep 2; \
	  $(AVRDUDE) -pm328p -c stk500v1 -P $(ARDUINO_PORT) -b9600 -U flash:w:./bin/application.hex; \
	fi
clean:
	rm -f build/*
	rm -f bin/*