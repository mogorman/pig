PCBNAME=pig
AUTHOR="Matthew O'Gorman <mog@rldn.net>  , Tim Heath <crashcartpro@gmail.com>"
REV=12.0
STACK=-
SOLDERMASK=red
SILKSCREEN=white
#CUSTOMPCB=/home/mog/nonapt/pcb/src/pcb
#CUSTOMPCB_ARGS=--photo-mask-colour $(SOLDERMASK) --photo-silk-colour $(SILKSCREEN)
all: get_submodule

get_submodule:
	../update_submodule.sh

-include ../meatstand_common/schematics/production_Makefile
