#!/bin/bash
#4242424242424242424242424242
#AAAAAAAAAAAAAAAAAAAAAAAAAAAA
File=$1
Match_file=$2
Deploy=$3
Secret=$4
Time=$5
First_line=""
Second_line=""
while read line; do 
    if [ "${First_line}" = "" ]; then
	First_line=`echo ${line} | tr -d " \t\n\r"`
    else
	Second_line=`echo ${line}| tr -d " \t\n\r"`
    fi
done < $Match_file

Start=$((${First_line::1} - 1))
Stop=$((${Second_line::1} +1))

head -n${Start} $File > ${Deploy}

M_first_line=${First_line:10}
M_first_line=${M_first_line:: -2}

M_second_line=${Second_line:10}
M_second_line=${M_second_line:: -2}

echo "abcd ${First_line} ef ${Second_line}"

echo "abcd           ${M_first_line}   ef           ${M_second_line}"
Double="${M_first_line}${M_second_line}"
New_double=`echo ${Double} | sed -e "s/4242424242424242424242424242/${Secret}${Time}/g"`
echo ${Double}
echo ${New_double}

tail -n+${Stop} $File >> ${Deploy}
