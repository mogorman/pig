#!/bin/bash
#this is where continuous integration script will live
git submodule update --init
ls -la
cd hardware

make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white --photo-plating  tinned"
make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white  --photo-plating  tinned" photos
cp schematic.png ~/artifacts/pig/schematic.png
cp schematic.png ~/artifacts/pig/`git rev-parse --short HEAD`_schematic.png

cp board.png ~/artifacts/pig/board.png
cp board.png ~/artifacts/pig/`git rev-parse --short HEAD`.png

cp *.zip ~/artifacts/pig/gerbers.zip

cd ..

cd firmware

make ARDUINO=/usr/src/arduino-1.5.6-r2/arduino
cp bin/bootloader.hex ~/artifacts/pig/
cp bin/match.hex ~/artifacts/pig/
cp bin/application.hex ~/artifacts/pig/

cd ..
