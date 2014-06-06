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
cd ..
cp hardware/schematic.png ../pig/artifacts/schematic.png
cp hardware/schematic.png ../pig/artifacts/`git rev-parse --short HEAD`_schematic.png

cp hardware/board.png ../pig/artifacts/board.png
cp hardware/board.png ../pig/artifacts/`git rev-parse --short HEAD`.png
