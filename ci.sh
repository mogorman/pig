#!/bin/bash
#this is where continuous integration script will live

copy_files() {
    local new_copy="$1"
    local old_copy="$2"
    local versioned=`git rev-parse --short HEAD`"_$1"
    if diff "$new_copy" "$old_copy" 2>/dev/null ; then
	echo Same file nothing to do
    else
	echo File changed make copies.
	cp "$1" "$2"
	cp "$1" "/home/gitlab_ci_runner/artifacts/pig/build/"`git rev-parse --short HEAD`_"$1"
    fi    
}

git submodule update --init
ls -la
cd hardware

make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white --photo-plating  tinned"
make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white  --photo-plating  tinned" photos


copy_files schematic.png ~/artifacts/pig/schematic.png

copy_files board.png ~/artifacts/pig/board.png

cp *.zip gerbers.zip

copy_files gerbers.zip ~/artifacts/pig/gerbers.zip

cd ..

cd firmware

make ARDUINO=/usr/src/arduino-1.5.6-r2/arduino

cd bin

copy_files bootloader.hex ~/artifacts/pig/bootloader.hex
copy_files match.hex ~/artifacts/pig/match.hex
copy_files application.hex ~/artifacts/pig/application.hex

cd ..

cd ..
