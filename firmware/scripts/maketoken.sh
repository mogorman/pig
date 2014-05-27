#!/bin/bash
Original_secret="4141414141414141414141414141"
#AAAAAAAAAAAAAAAAAAAAAAAAAAAA
File=$1
Match_file=$2
Deploy=$3
Secret=$4
Time=$5
First_line=""
Second_line=""

Lines=()
Headers=()
Datas=()
Lengths=()
Blob=""


intel_hex ()
{
New_line=$1
Length=${#New_line}
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

echo ${Invert}
}


while read Line; do 
    if [ "${First_line}" = "" ]; then
	Start=$((${Line::1} - 1))
	First_line="yes"
    else
	Stop=$((${Line::1} + 1))
    fi

    Length_1=`echo ${Line} | tr -d " \t\n\r" | cut -f2 -d':'`
    Data=${Length_1:8}
    Data=${Data:: -2}
    Line_1=${Length_1}
    Header=${Length_1::8}
    Length_1=${Length_1::2}
    Length=$((0x${Length_1} *2))

    Lines+=("$Line")
    Headers+=("$Header")
    Blob="${Blob}${Data}"
    Datas+=("$Data")
    Lengths+=("$Length")

done < $Match_file

head -n${Start} $File > ${Deploy}


Count=0
Byte_count=0
Length=${#Lines[@]}
New_blob=`echo ${Blob} | sed -e "s/${Original_secret}/${Secret}${Time}/g"`
while [ $Count -lt $Length ]; do
    Current_length=${Lengths[${Count}]}
    Sum=`intel_hex "${Headers[$Count]}${Datas[$Count]}"`
    New_sum=`intel_hex "${Headers[$Count]}${New_blob:${Byte_count}:${Current_length}}"`
    printf ":${Headers[$Count]}${New_blob:${Byte_count}:${Current_length}}${Checksums[$Count]}${New_sum}\r\n" >> ${Deploy}
    let Count=Count+1
    let Byte_count=Current_length+Byte_count
done

tail -n+${Stop} ${File} >> ${Deploy}
