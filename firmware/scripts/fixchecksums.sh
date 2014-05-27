#!/bin/bash
File=$1
Output_file=$2
Match=4141414141414141414141414141
a=0
#rm  new.hex
#touch new.hex
First_line=""
while read line
do a=$(($a+1));

# Remove newline and remove beginning colon
    New_line=${line:1: -1}
    if [ "${First_line}" = "" ]; then
	Complete_first_line=${line}
	First_line=${New_line:8}
	First_line=${First_line:: -2}
	continue;
    fi
    Second_line=${New_line:8}
    Second_line=${Second_line:: -2}
    Complete_second_line=${line}
    Double=${First_line}${Second_line}
    if [ "${Double/$Match}" != "${Double}" ]; then
	echo "$((a -1))${Complete_first_line}" > ${Output_file}
	echo "$a${Complete_second_line}" >> ${Output_file}
	exit
    fi
    First_line=${Second_line}
    Complete_first_line=${line}
done < $File
echo "Final! line count is: $a";
