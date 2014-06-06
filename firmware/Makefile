ARDUINO=~/nonapt/arduino-1.5.6-r2/arduino
AVRDUDE=/usr/bin/avrdude
AVR-GCC=/usr/bin/avr-gcc
AVR-OBJCOPY=/usr/bin/avr-objcopy
PROGRAMMER=buspirate
PROGRAMMER_PORT=/dev/buspirate
ARDUINO_PORT=/dev/arduino

all: check_submodule compile_app compile_boot


check_submodule:
	@echo ""
ifeq ("$(wildcard ./audioboot-writer/COPYING )", "")

	@echo "I didn't have libraries i need to compile cloned."
	@echo "I will get it for you now."
	../update_submodule.sh
	@echo "I got my dependencies I am ready to go"
	@make
endif

compile_app:
ifeq ("$(wildcard ./bin/application.hex )", "")
	@rm -f ./build/app.md5
endif
	@md5sum application/application.ino | cmp -s build/app.md5 -; \
	 if test $$? -ne 0; then \
	  xvfb-run $(ARDUINO) \
	   --verbose-build \
	   --board arduino:avr:pig \
	   --pref build.path=`pwd`/build \
	   --pref sketchbook.path=`pwd` \
	   --verify `pwd`/application/application.ino; \
	  md5sum application/application.ino > build/app.md5; \
	  cp build/application.cpp.hex ./bin/application.hex; \
	  touch build/.git_empty; \
	  scripts/fixchecksums.sh bin/application.hex bin/match.hex; \
	fi



compile_boot:
	md5sum bootloader/bootloader.c | cmp -s build/bootloader.md5 -; \
	 if test $$? -ne 0; then \
	   md5sum bootloader/bootloader.c > build/bootloader.md5; \
	$(AVR-GCC) -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' -c -o ./build/bootloader.o ./bootloader/bootloader.c; \
	$(AVR-GCC) -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' -Wl,--section-start=.text=0x7800 -o ./build/bootloader.elf ./build/bootloader.o; \
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
	@scripts/maketoken.sh bin/application.hex bin/match.hex bin/deploy.hex \
	 48656c6c6f21deadbeef 32
	$(AVRDUDE) -pm328p -c arduino -P $(ARDUINO_PORT) -b9600 -U flash:w:./bin/deploy.hex

audio_app: compile_app
	@scripts/maketoken.sh bin/application.hex bin/match.hex bin/deploy.hex \
	 48656c6c6f21deadbeef 15
	@java -jar audioboot-writer/bin/audioboot.jar ./bin/deploy.hex ./bin/deploy.wav
	@mplayer bin/deploy.wav
clean:
	rm -rf build/*
	rm -rf bin/*
