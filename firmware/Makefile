ARDUINO=~/nonapt/arduino-1.5.6-r2/arduino
ARDUINO_DIR=~/nonapt/arduino-1.5.6-r2/
AVRDUDE=/usr/bin/avrdude
AVR_GCC=/usr/bin/avr-gcc
AVR_OBJCOPY=/usr/bin/avr-objcopy
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
	  $(ARDUINO_DIR)/hardware/tools/avr/bin/avr-objcopy -O srec -R .eeprom \
	              build/application.cpp.elf bin/application.hex; \
	  touch build/.git_empty; \
	  scripts/fixchecksums.sh bin/application.hex bin/match.hex; \
	fi

compile_test:
ifeq ("$(wildcard ./bin/test.hex )", "")
	@rm -f ./build/test.md5
endif
	@md5sum test/test.ino | cmp -s build/test.md5 -; \
	 if test $$? -ne 0; then \
	  xvfb-run $(ARDUINO) \
	   --verbose-build \
	   --board arduino:avr:pig \
	   --pref build.path=`pwd`/build \
	   --pref sketchbook.path=`pwd` \
	   --verify `pwd`/test/test.ino; \
	  md5sum test/test.ino > build/test.md5; \
	  $(ARDUINO_DIR)/hardware/tools/avr/bin/avr-objcopy -O srec -R .eeprom \
	              build/test.cpp.elf bin/test.hex; \
	  touch build/.git_empty; \
	fi

compile_boot:
	md5sum bootloader/bootloader.c | cmp -s build/bootloader.md5 -; \
	 if test $$? -ne 0; then \
	   md5sum bootloader/bootloader.c > build/bootloader.md5; \
	$(AVR_GCC) -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' -c -o ./build/bootloader.o ./bootloader/bootloader.c; \
	$(AVR_GCC) -g -Wall -O2 -mmcu=atmega328p -DF_CPU=8000000L  '-DMAX_TIME_COUNT=F_CPU>>4' -Wl,--section-start=.text=0x7800 -o ./build/bootloader.elf ./build/bootloader.o; \
	$(AVR_OBJCOPY) -j .text -j .data -O srec ./build/bootloader.elf ./build/bootloader.hex; \
	fi
	cp ./build/bootloader.hex ./bin/bootloader.hex

bootloader: compile_boot
	$(AVRDUDE) -c $(PROGRAMMER) -p m328p -P $(PROGRAMMER_PORT) -e -U lock:w:0x3f:m 
	$(AVRDUDE) -v -c $(PROGRAMMER) -p m328p -P $(PROGRAMMER_PORT) \
	 -U lfuse:w:0xE2:m \
	 -U hfuse:w:0xDA:m \
	 -U efuse:w:0x07:m \
	 -U  flash:w:./bin/bootloader.hex:s -U lock:w:0x0F:m

app: compile_app
	scripts/maketoken.sh bin/application.hex bin/match.hex bin/deploy.hex \
	 48656c6c6f21deadbeef 32
	$(AVRDUDE) -pm328p -c arduino -P $(ARDUINO_PORT) -b9600 -U flash:w:./bin/deploy.hex:s

test: compile_test
	$(AVRDUDE) -pm328p -c arduino -P $(ARDUINO_PORT) -b9600 -U flash:w:./bin/test.hex:s

audio_app: compile_app
	@scripts/maketoken.sh bin/application.hex bin/match.hex bin/deploy.hex \
	 48656c6c6f21deadbeef 15
	@java -jar audioboot-writer/bin/audioboot.jar ./bin/deploy.hex ./bin/deploy.wav
	@mplayer bin/deploy.wav
clean:
	rm -rf build/*
	rm -rf bin/*
