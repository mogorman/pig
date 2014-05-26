#!/bin/bash
#size of file - 8 and size of file -44
echo Hello world
rm -f application.wav riff.wav header.wav
#RIFF bit
echo -ne "$(printf '\\x%x' 0x52)" >> riff.wav
echo -ne "$(printf '\\x%x' 0x49)" >> riff.wav
echo -ne "$(printf '\\x%x' 0x46)" >> riff.wav
echo -ne "$(printf '\\x%x' 0x46)" >> riff.wav

#4 byte file size figure out soon

#WAVEfmt
echo -ne "$(printf '\\x%x' 0x57)" >> header.wav
echo -ne "$(printf '\\x%x' 0x41)" >> header.wav
echo -ne "$(printf '\\x%x' 0x56)" >> header.wav
echo -ne "$(printf '\\x%x' 0x45)" >> header.wav
echo -ne "$(printf '\\x%x' 0x66)" >> header.wav
echo -ne "$(printf '\\x%x' 0x6d)" >> header.wav
echo -ne "$(printf '\\x%x' 0x74)" >> header.wav
echo -ne "$(printf '\\x%x' 0x20)" >> header.wav
#length of format data
echo -ne "$(printf '\\x%x' 0x10)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
#type of format, pcm
echo -ne "$(printf '\\x%x' 0x01)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
#number of channels
echo -ne "$(printf '\\x%x' 0x02)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
#sample rate 44100khz
echo -ne "$(printf '\\x%x' 0x44)" >> header.wav
echo -ne "$(printf '\\x%x' 0xac)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
#(Sample Rate * BitsPerSample * Channels) / 8.
echo -ne "$(printf '\\x%x' 0xb1)" >> header.wav
echo -ne "$(printf '\\x%x' 0xac)" >> header.wav
echo -ne "$(printf '\\x%x' 0x02)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
#(BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo/16 bit mono4 - 16 bit stereo
echo -ne "$(printf '\\x%x' 0x04)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
#bits per sample
echo -ne "$(printf '\\x%x' 0x10)" >> header.wav
echo -ne "$(printf '\\x%x' 0x00)" >> header.wav
#data because data...
echo -ne "$(printf '\\x%x' 0x64)" >> header.wav
echo -ne "$(printf '\\x%x' 0x61)" >> header.wav
echo -ne "$(printf '\\x%x' 0x74)" >> header.wav
echo -ne "$(printf '\\x%x' 0x61)" >> header.wav
#4 byte size of the file bc 70 09 00 

cat riff.wav header.wav > application.wav
