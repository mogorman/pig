#!/bin/bash
File=$1
Match=4242424242424242424242424242
a=0
rm  new.hex
touch new.hex
First_line=""
while read line
do a=$(($a+1));

# Remove newline and remove beginning colon
    New_line=${line:1: -1}
    if [ "${First_line}" = "" ]; then
	Complete_first_line=${line}
	First_line=${New_line:8}
	First_line=${First_line:: -2}
	echo data ${First_line} data ${New_line}
	continue;
    fi
    Second_line=${New_line:8}
    Second_line=${Second_line:: -2}
    Complete_second_line=${line}
    Double=${First_line}${Second_line}
    if [ "${Double/$Match}" != "${Double}" ]; then
	echo $a $Double
	echo "$((a -1))${Complete_first_line}" > match.hex
	echo "$a${Complete_second_line}" >> match.hex
	exit
    fi
    First_line=${Second_line}
    Complete_first_line=${line}
    Length=${#New_line}
    let Length=Length-2
    Count=0
    Sum=0
    while [ $Count -lt $Length ]; do
	Sum=`printf '%X' $((0x${Sum} + 0x${New_line:${Count}:2}))`
	let Count=Count+2
    done
    let Length=Length+2

    if [ "${#Sum}" -gt 2 ]; then
	Sum=${Sum: -2}
    fi

    if [ "${Sum}" = "0" ]; then
	Invert=00
    elif [ "${Sum}" = "00" ]; then
	Invert=00
    elif [ "${Sum}" = "01" ]; then
	Invert=FF
    elif [ "${Sum}" = "1" ]; then
	Invert=FF
    else
	Invert=`printf "%X" $((0xFF-0x${Sum}+1))`
    fi

    if [ "${#Invert}" = "1" ]; then
	Invert=0${Invert}
    fi
    printf ":${New_line:: -2}${Invert}\r\n" >> new.hex

done < $File
echo "Final! line count is: $a";
