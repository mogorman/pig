#!/bin/bash
~/nonapt/arduino-1.5.6-r2/arduino \
 --verbose-build \
 --board arduino:avr:fio \
 --pref build.path=`pwd`/application_build \
 --pref sketchbook.path=`pwd` \
 --verify `pwd`/application/application.ino

cp application_build/application.cpp.hex ./application.hex
reboot_avr
sleep 1
avrdude -pm328p -c stk500v1 -P/dev/arduino -b9600 -U flash:w:application.hex
