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
cd hardware
make clean
make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white --photo-plating  tinned"
make CUSTOMPCB="/usr/src/pcb/src/pcb" \
     CUSTOMPCB_ARGS="--photo-mask-colour red \
     --photo-silk-colour white  --photo-plating  tinned" photos
ls black_board.png board.png bom order xy schematic.png schematic.pdf gerbers/pig_make.bottom.gbr > /dev/null 2>&1
if [ "$?" != "0" ]; then
    echo "I failed to create all the files I should have. build failed"
    exit 1
fi
New_pig=`git diff HEAD^ HEAD -- pig.pcb|wc -l`
New_schematic=`git diff HEAD^ HEAD -- pig.sch|wc -l`

if [ "$New_pig" == "0" ]; then
    echo "No changes to pig don't bother saving image."
else
    copy_files board.png ~/artifacts/pig/board.png
    zip -r gerbers.zip gerbers/
    copy_files gerbers.zip ~/artifacts/pig/gerbers.zip
fi

if [ "$New_schematic" == "0" ]; then
    echo "No changes to schematic don't bother saving the image."
else
    copy_files schematic.png ~/artifacts/pig/schematic.png
fi
cd ..

cd firmware
make clean
cp ~/blah/pig.txt ~/.arduino15/preferences.txt
rm ~/.arduino15/preferences.txt
make ARDUINO_DIR=/usr/src/arduino-1.5.6-r2/
ls bin/bootloader.hex bin/match.hex bin/application.hex
if [ "$?" != "0" ]; then
    echo "I failed to create all the files I should have. build failed"
    exit 1
fi
cd bin

copy_files bootloader.hex ~/artifacts/pig/bootloader.hex
copy_files match.hex ~/artifacts/pig/match.hex
copy_files application.hex ~/artifacts/pig/application.hex

cd ..

cd ..
