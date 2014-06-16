#!/bin/bash
Original_secret="4141414141414141414141414141"
#AAAAAAAAAAAAAAAAAAAAAAAAAAAA
File=$1
Match_file=$2
Deploy=$3
Secret=$4
if [ -z "$5" ]; then
Offset=0
else
Offset=$5
fi
if [ -z "$6" ]; then
	Date=`date +%s`
#180000 for tz offset
	let Date=Date+Offset
#-18000 your killing me 
	Time=$(printf '%2X' ${Date})
else
	Time=$6
fi
First_line=""
Second_line=""

Lines=()
Headers=()
Datas=()
Lengths=()
Blob=""


intel_checksum ()
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
    Invert=`printf "%X" $((0xFF-0x${Sum}))`
fi

if [ "${#Invert}" = "1" ]; then
    Invert=0${Invert}
fi
echo ${Invert}
}

#echo `intel_checksum "137AF00A0A0D00000000000000000000000000"`
#exit 
while read Line; do 
    if [ "${First_line}" = "" ]; then
	Start=`echo ${Line} | cut -f1 -d'S'`
	let Start=Start-1
	First_line="yes"
    else
	Stop=`echo ${Line} | cut -f1 -d'S'`
	let Stop=Stop+1
    fi

    Length_1=`echo ${Line} | tr -d " \t\n\r" | cut -f2 -d'S'`
    Data=${Length_1:7}
    Data=${Data:: -2}
    Line_1=${Length_1}
    Header=${Length_1::7}
    Length_1=${Length_1:1:2}
    Length=$(((0x${Length_1} *2) -6))

    Lines+=("$Line")
    Headers+=("$Header")
    Blob="${Blob}${Data}"
    Datas+=("$Data")
    Lengths+=("$Length")

#    echo Length ${Length}
#    echo Header ${Header}
#    echo Data ${Data}

done < $Match_file

head -n${Start} $File > ${Deploy}

Count=0
Byte_count=0
Length=${#Lines[@]}
New_blob=`echo ${Blob} | sed -e "s/${Original_secret}/${Secret}${Time}/g"`
#echo X:${Blob}
#echo Y:${New_blob}
while [ $Count -lt $Length ]; do
    Current_length=${Lengths[${Count}]}
#    Sum=`intel_checksum "${Headers[$Count]:1}${Datas[$Count]}"`
    New_sum=`intel_checksum "${Headers[$Count]:1}${New_blob:${Byte_count}:${Current_length}}"`
#    echo "S${Headers[$Count]}${New_blob:${Byte_count}:${Current_length}}"
#    echo "Length=${Current_length}"
#    echo "Bytecount=${Byte_count}"
    printf "S${Headers[$Count]}${New_blob:${Byte_count}:${Current_length}}${Checksums[$Count]}${New_sum}\r\n" >> ${Deploy}
    let Count=Count+1
    let Byte_count=Current_length+Byte_count
done

tail -n+${Stop} ${File} >> ${Deploy}
