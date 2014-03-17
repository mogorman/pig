v 20110115 2
C 45300 52900 1 0 0 atmega168pa.sym
{
T 45600 58300 5 10 1 1 0 0 1
footprint=atmega328_combo.fp
T 49400 58300 5 10 1 1 0 0 1
refdes=U1
}
C 60200 57400 1 0 0 icsp.sym
{
T 60400 58200 5 10 1 1 0 0 1
footprint=icsp
T 61600 58200 5 10 1 1 0 0 1
refdes=CONN1
}
C 46900 46900 1 0 0 battery-1.sym
{
T 47200 47800 5 10 0 0 0 0 1
device=BATTERY
T 47200 47400 5 10 1 1 0 0 1
refdes=B1
T 47200 48200 5 10 0 0 0 0 1
symversion=0.1
T 47100 46900 5 10 0 1 0 0 1
footprint=CR2032-BAT-HLD.fp
}
C 47900 46900 1 90 0 ground.sym
C 46900 47000 1 0 1 output-2.sym
{
T 46000 47200 5 10 0 0 0 6 1
net=Vcc:1
T 46700 47700 5 10 0 0 0 6 1
device=none
T 46000 47100 5 10 1 1 0 7 1
value=Vcc
}
C 51300 56500 1 0 1 input-2.sym
{
T 51300 56700 5 10 0 0 0 6 1
net=Vcc:1
T 50700 57200 5 10 0 0 0 6 1
device=none
T 50800 56600 5 10 1 1 0 1 1
value=Vcc
}
C 51300 56300 1 0 1 input-2.sym
{
T 51300 56500 5 10 0 0 0 6 1
net=Vcc:1
T 50700 57000 5 10 0 0 0 6 1
device=none
T 50800 56400 5 10 1 1 0 1 1
value=Vcc
}
C 51300 56100 1 0 1 input-2.sym
{
T 51300 56300 5 10 0 0 0 6 1
net=Vcc:1
T 50700 56800 5 10 0 0 0 6 1
device=none
T 50800 56200 5 10 1 1 0 1 1
value=Vcc
}
C 50500 57000 1 90 0 ground.sym
N 50200 57200 49900 57200 4
N 50100 56800 50100 57200 4
N 50100 57000 49900 57000 4
N 50100 56800 49900 56800 4
C 51800 47700 1 270 0 resistor-1.sym
{
T 52200 47400 5 10 0 0 270 0 1
device=RESISTOR
T 51500 47100 5 10 1 1 0 0 1
refdes=R1
T 52000 47100 5 10 1 1 0 0 1
value=10k
T 51800 47700 5 10 0 1 0 0 1
footprint=0805
}
C 51900 47500 1 0 0 capacitor-1.sym
{
T 52100 48200 5 10 0 0 0 0 1
device=CAPACITOR
T 52100 48000 5 10 1 1 0 0 1
refdes=C1
T 52100 48400 5 10 0 0 0 0 1
symversion=0.1
T 52500 48000 5 10 1 1 0 0 1
value=0.1uF
T 51900 47500 5 10 0 1 0 0 1
footprint=0805
}
C 45300 53600 1 0 1 output-2.sym
{
T 44400 53800 5 10 0 0 0 6 1
net=Sck:1
T 45100 54300 5 10 0 0 0 6 1
device=none
T 44400 53700 5 10 1 1 0 7 1
value=Sck
}
C 45300 53800 1 0 1 output-2.sym
{
T 44400 54000 5 10 0 0 0 6 1
net=Miso:1
T 45100 54500 5 10 0 0 0 6 1
device=none
T 44400 53900 5 10 1 1 0 7 1
value=Miso
}
C 45300 54000 1 0 1 output-2.sym
{
T 44400 54200 5 10 0 0 0 6 1
net=Mosi:1
T 45100 54700 5 10 0 0 0 6 1
device=none
T 44400 54100 5 10 1 1 0 7 1
value=Mosi
}
C 63800 57900 1 0 1 input-2.sym
{
T 63800 58100 5 10 0 0 0 6 1
net=Vcc:1
T 63200 58600 5 10 0 0 0 6 1
device=none
T 63300 58000 5 10 1 1 0 1 1
value=Vcc
}
C 62400 57300 1 0 0 ground.sym
N 62600 57600 62400 57600 4
C 63800 57700 1 0 1 input-2.sym
{
T 63800 57900 5 10 0 0 0 6 1
net=Mosi:1
T 63200 58400 5 10 0 0 0 6 1
device=none
T 63300 57800 5 10 1 1 0 1 1
value=Mosi
}
C 58800 57900 1 0 0 input-2.sym
{
T 58800 58100 5 10 0 0 0 0 1
net=Miso:1
T 59400 58600 5 10 0 0 0 0 1
device=none
T 59300 58000 5 10 1 1 0 7 1
value=Miso
}
C 58800 57700 1 0 0 input-2.sym
{
T 58800 57900 5 10 0 0 0 0 1
net=Sck:1
T 59400 58400 5 10 0 0 0 0 1
device=none
T 59300 57800 5 10 1 1 0 7 1
value=Sck
}
C 58800 57500 1 0 0 input-2.sym
{
T 58800 57700 5 10 0 0 0 0 1
net=Reset:1
T 59400 58200 5 10 0 0 0 0 1
device=none
T 59300 57600 5 10 1 1 0 7 1
value=Reset
}
C 45300 55000 1 0 1 output-2.sym
{
T 44400 55200 5 10 0 0 0 6 1
net=Reset:1
T 45100 55700 5 10 0 0 0 6 1
device=none
T 44400 55100 5 10 1 1 0 7 1
value=Reset
}
C 50500 47600 1 0 0 input-2.sym
{
T 50500 47800 5 10 0 0 0 0 1
net=Reset:1
T 51100 48300 5 10 0 0 0 0 1
device=none
T 51000 47700 5 10 1 1 0 7 1
value=Reset
}
C 51800 45400 1 270 1 input-2.sym
{
T 52000 45400 5 10 0 0 270 6 1
net=Vcc:1
T 52500 46000 5 10 0 0 270 6 1
device=none
T 52100 46200 5 10 1 1 0 1 1
value=Vcc
}
C 54200 47600 1 0 1 input-2.sym
{
T 54200 47800 5 10 0 0 0 6 1
net=Serial_reset:1
T 53600 48300 5 10 0 0 0 6 1
device=none
T 53700 47700 5 10 1 1 0 1 1
value=Serial_reset
}
C 52400 56300 1 0 0 connector6-1.sym
{
T 54200 58100 5 10 0 0 0 0 1
device=CONNECTOR_6
T 52500 58300 5 10 1 1 0 0 1
refdes=CONN2
T 52700 58000 5 10 0 1 0 0 1
footprint=ftdi
}
C 54100 56400 1 0 0 output-2.sym
{
T 55000 56600 5 10 0 0 0 0 1
net=Serial_reset:1
T 54300 57100 5 10 0 0 0 0 1
device=none
T 55000 56500 5 10 1 1 0 1 1
value=Serial_reset
}
C 45300 57900 1 0 1 output-2.sym
{
T 44400 58100 5 10 0 0 0 6 1
net=Uart_Rx:1
T 45100 58600 5 10 0 0 0 6 1
device=none
T 44400 58000 5 10 1 1 0 7 1
value=Uart_Rx
}
C 45300 57700 1 0 1 output-2.sym
{
T 44400 57900 5 10 0 0 0 6 1
net=Uart_Tx:1
T 45100 58400 5 10 0 0 0 6 1
device=none
T 44400 57800 5 10 1 1 0 7 1
value=Uart_Tx
}
C 55500 57000 1 0 1 input-2.sym
{
T 55500 57200 5 10 0 0 0 6 1
net=Uart_Rx:1
T 54900 57700 5 10 0 0 0 6 1
device=none
T 55000 57100 5 10 1 1 0 1 1
value=Uart_Rx
}
C 55500 56700 1 0 1 input-2.sym
{
T 55500 56900 5 10 0 0 0 6 1
net=Uart_Tx:1
T 54900 57400 5 10 0 0 0 6 1
device=none
T 55000 56800 5 10 1 1 0 1 1
value=Uart_Tx
}
C 55000 57800 1 90 0 ground.sym
N 54700 58000 54100 58000 4
N 54500 58000 54500 57700 4
N 54500 57700 54100 57700 4
C 53300 49200 1 0 1 led-1.sym
{
T 52500 49800 5 10 0 0 0 6 1
device=LED
T 52500 49600 5 10 1 1 0 6 1
refdes=LED1
T 52500 50000 5 10 0 0 0 6 1
symversion=0.1
T 53200 49500 5 10 0 1 0 0 1
footprint=0805
}
C 44600 52800 1 0 0 crystal-1.sym
{
T 44800 53300 5 10 0 0 0 0 1
device=CRYSTAL
T 44800 53100 5 10 1 1 0 0 1
refdes=U2
T 44800 53500 5 10 0 0 0 0 1
symversion=0.1
T 44500 52600 5 10 1 1 0 0 1
value=32.768khz
T 45000 52800 5 10 0 1 0 0 1
footprint=abs07_crystal
}
N 45300 53500 44600 53500 4
N 44600 53500 44600 52900 4
N 45300 53300 45300 52900 4
C 45300 57500 1 0 1 output-2.sym
{
T 44400 57700 5 10 0 0 0 6 1
net=Button:1
T 45100 58200 5 10 0 0 0 6 1
device=none
T 44400 57600 5 10 1 1 0 7 1
value=Button
}
C 58100 52400 1 0 0 input-2.sym
{
T 58100 52600 5 10 0 0 0 0 1
net=Button:1
T 58700 53100 5 10 0 0 0 0 1
device=none
T 58600 52500 5 10 1 1 0 7 1
value=Button
}
N 59500 52500 60300 52500 4
C 59800 52500 1 270 0 resistor-1.sym
{
T 60200 52200 5 10 0 0 270 0 1
device=RESISTOR
T 59500 51900 5 10 1 1 0 0 1
refdes=R2
T 60000 51900 5 10 1 1 0 0 1
value=10k
T 59800 52500 5 10 0 1 0 0 1
footprint=0805
}
C 59700 51300 1 0 0 ground.sym
C 62700 52400 1 0 1 input-2.sym
{
T 62700 52600 5 10 0 0 0 6 1
net=Vcc:1
T 62100 53100 5 10 0 0 0 6 1
device=none
T 62200 52500 5 10 1 1 0 1 1
value=Vcc
}
C 57800 54700 1 0 0 input-2.sym
{
T 57800 54900 5 10 0 0 0 0 1
net=Tip:1
T 58400 55400 5 10 0 0 0 0 1
device=none
T 58300 54800 5 10 1 1 0 7 1
value=Tip
}
C 59600 53600 1 0 0 ground.sym
C 59700 54800 1 270 0 resistor-1.sym
{
T 60100 54500 5 10 0 0 270 0 1
device=RESISTOR
T 59400 54200 5 10 1 1 0 0 1
refdes=R3
T 59900 54200 5 10 1 1 0 0 1
value=10k
T 59700 54800 5 10 0 1 0 0 1
footprint=0805
}
N 59200 54800 60300 54800 4
C 59700 55700 1 270 0 resistor-1.sym
{
T 60100 55400 5 10 0 0 270 0 1
device=RESISTOR
T 59400 55100 5 10 1 1 0 0 1
refdes=R4
T 59900 55100 5 10 1 1 0 0 1
value=10k
T 59700 55700 5 10 0 1 0 0 1
footprint=0805
}
C 59900 57100 1 90 1 input-2.sym
{
T 59700 57100 5 10 0 0 90 6 1
net=Vcc:1
T 59200 56500 5 10 0 0 90 6 1
device=none
T 60300 56200 5 10 1 1 180 1 1
value=Vcc
}
C 60300 54600 1 0 0 capacitor-1.sym
{
T 60500 55300 5 10 0 0 0 0 1
device=CAPACITOR
T 60500 55100 5 10 1 1 0 0 1
refdes=C2
T 60500 55500 5 10 0 0 0 0 1
symversion=0.1
T 60900 55100 5 10 1 1 0 0 1
value=100uF
T 60300 54600 5 10 0 1 0 0 1
footprint=5mm_EL_Cap
}
N 62300 54800 61200 54800 4
C 51100 49300 1 0 0 resistor-1.sym
{
T 51400 49700 5 10 0 0 0 0 1
device=RESISTOR
T 51400 49700 5 10 1 1 180 0 1
refdes=R5
T 51900 49700 5 10 1 1 180 0 1
value=330
T 51100 49300 5 10 0 1 0 0 1
footprint=0805
}
N 52000 49400 52400 49400 4
C 50800 49200 1 270 1 ground.sym
C 54700 49300 1 0 1 input-2.sym
{
T 54700 49500 5 10 0 0 0 6 1
net=Indicator:1
T 54100 50000 5 10 0 0 0 6 1
device=none
T 54200 49400 5 10 1 1 0 1 1
value=Indicator
}
C 57100 46500 1 0 0 LY091WG15.sym
{
T 57400 46500 5 10 1 1 0 0 1
footprint=LY091WG15
T 57400 49200 5 10 1 1 0 0 1
refdes=DISP1
}
C 59300 48300 1 0 0 capacitor-1.sym
{
T 59500 49000 5 10 0 0 0 0 1
device=CAPACITOR
T 59500 48800 5 10 1 1 0 0 1
refdes=C3
T 59500 49200 5 10 0 0 0 0 1
symversion=0.1
T 59900 48800 5 10 1 1 0 0 1
value=1uF
T 59300 48300 5 10 0 1 0 0 1
footprint=0805
}
C 59300 49200 1 0 0 capacitor-1.sym
{
T 59500 49900 5 10 0 0 0 0 1
device=CAPACITOR
T 59500 49700 5 10 1 1 0 0 1
refdes=C4
T 59500 50100 5 10 0 0 0 0 1
symversion=0.1
T 59900 49700 5 10 1 1 0 0 1
value=1uF
T 59300 49200 5 10 0 1 0 0 1
footprint=0805
}
N 59300 48500 59000 48500 4
N 59000 48300 59400 48300 4
N 59400 48300 59400 48000 4
N 59400 48000 60200 48000 4
N 60200 48000 60200 48500 4
N 59000 48700 59300 48700 4
N 59300 48700 59300 49100 4
N 59300 49100 60200 49100 4
N 60200 49100 60200 49400 4
N 59300 49400 59100 49400 4
N 59100 49400 59100 48900 4
C 64400 48400 1 0 1 resistor-1.sym
{
T 64100 48800 5 10 0 0 0 6 1
device=RESISTOR
T 64100 48800 5 10 1 1 180 6 1
refdes=R6
T 63600 48800 5 10 1 1 180 6 1
value=10k
T 64400 48400 5 10 0 1 0 0 1
footprint=0805
}
C 62800 50400 1 270 0 input-2.sym
{
T 63000 50400 5 10 0 0 90 8 1
net=Vcc:1
T 63500 49800 5 10 0 0 90 8 1
device=none
T 62400 49600 5 10 1 1 180 7 1
value=Vcc
}
C 65800 48400 1 0 1 input-2.sym
{
T 65800 48600 5 10 0 0 0 6 1
net=Disp_on:1
T 65200 49100 5 10 0 0 0 6 1
device=none
T 65300 48500 5 10 1 1 0 1 1
value=Disp_on
}
C 62500 47000 1 90 0 capacitor-1.sym
{
T 61800 47200 5 10 0 0 90 0 1
device=CAPACITOR
T 62200 47700 5 10 1 1 180 0 1
refdes=C5
T 61600 47200 5 10 0 0 90 0 1
symversion=0.1
T 62800 47300 5 10 1 1 180 0 1
value=1uF
T 62500 47000 5 10 0 1 0 0 1
footprint=0805
}
C 61300 47700 1 270 0 capacitor-1.sym
{
T 62000 47500 5 10 0 0 270 0 1
device=CAPACITOR
T 61200 47400 5 10 1 1 0 0 1
refdes=C6
T 62200 47500 5 10 0 0 270 0 1
symversion=0.1
T 61600 47000 5 10 1 1 0 0 1
value=1uF
T 61300 47700 5 10 0 1 0 0 1
footprint=0805
}
N 59000 47900 62900 47900 4
N 62900 48000 62900 47900 4
C 62500 46700 1 0 1 ground.sym
C 56900 47100 1 270 1 ground.sym
N 59000 47700 61500 47700 4
N 61500 47700 61500 47900 4
C 61700 46500 1 0 1 ground.sym
C 55800 48800 1 0 0 input-2.sym
{
T 55800 49000 5 10 0 0 0 0 1
net=Sck:1
T 56400 49500 5 10 0 0 0 0 1
device=none
T 56300 48900 5 10 1 1 0 7 1
value=Sck
}
C 55800 48600 1 0 0 input-2.sym
{
T 55800 48800 5 10 0 0 0 0 1
net=Mosi:1
T 56400 49300 5 10 0 0 0 0 1
device=none
T 56300 48700 5 10 1 1 0 7 1
value=Mosi
}
C 55800 48000 1 0 0 input-2.sym
{
T 55800 48200 5 10 0 0 0 0 1
net=Miso:1
T 56400 48700 5 10 0 0 0 0 1
device=none
T 56300 48100 5 10 1 1 0 7 1
value=Miso
}
C 45300 54200 1 0 1 output-2.sym
{
T 44400 54400 5 10 0 0 0 6 1
net=CS:1
T 45100 54900 5 10 0 0 0 6 1
device=none
T 44400 54300 5 10 1 1 0 7 1
value=CS
}
C 55800 48400 1 0 0 input-2.sym
{
T 55800 48600 5 10 0 0 0 0 1
net=CS:1
T 56400 49100 5 10 0 0 0 0 1
device=none
T 56300 48500 5 10 1 1 0 7 1
value=CS
}
C 55800 48200 1 0 0 input-2.sym
{
T 55800 48400 5 10 0 0 0 0 1
net=Disp_reset:1
T 56400 48900 5 10 0 0 0 0 1
device=none
T 56300 48300 5 10 1 1 0 7 1
value=Disp_reset
}
C 58900 45600 1 270 1 resistor-1.sym
{
T 59300 45900 5 10 0 0 270 6 1
device=RESISTOR
T 58600 46300 5 10 1 1 180 6 1
refdes=R7
T 59100 45900 5 10 1 1 180 6 1
value=390K
T 58900 45600 5 10 0 1 0 0 1
footprint=0805
}
N 59000 46500 59000 47100 4
C 59200 45300 1 0 1 ground.sym
C 60600 46500 1 270 0 capacitor-1.sym
{
T 61300 46300 5 10 0 0 270 0 1
device=CAPACITOR
T 60500 46200 5 10 1 1 0 0 1
refdes=C7
T 61500 46300 5 10 0 0 270 0 1
symversion=0.1
T 60900 45800 5 10 1 1 0 0 1
value=2.2uF
T 60600 46500 5 10 0 1 0 0 1
footprint=0805
}
C 59700 46500 1 270 0 capacitor-1.sym
{
T 60400 46300 5 10 0 0 270 0 1
device=CAPACITOR
T 59600 46200 5 10 1 1 0 0 1
refdes=C8
T 60600 46300 5 10 0 0 270 0 1
symversion=0.1
T 60000 45800 5 10 1 1 0 0 1
value=4.7uF
T 59700 46500 5 10 0 1 0 0 1
footprint=0805
}
C 61000 45300 1 0 1 ground.sym
C 60100 45300 1 0 1 ground.sym
N 59000 47500 60800 47500 4
N 60800 47500 60800 46500 4
N 59000 47300 59900 47300 4
N 59900 47300 59900 46500 4
C 63500 49000 1 180 0 pnp-3.sym
{
T 62600 48500 5 10 0 0 180 0 1
device=PNP_TRANSISTOR
T 62600 48500 5 10 1 1 180 0 1
refdes=Q1
T 60900 49400 5 10 0 1 0 0 1
footprint=SOT23
}
C 55500 57300 1 0 1 input-2.sym
{
T 55500 57500 5 10 0 0 0 6 1
net=Vcc:1
T 54900 58000 5 10 0 0 0 6 1
device=none
T 55000 57400 5 10 1 1 0 1 1
value=Vcc
}
C 60300 52500 1 0 0 switch-pushbutton-no-1.sym
{
T 60700 52800 5 10 1 1 0 0 1
refdes=S1
T 60700 53100 5 10 0 0 0 0 1
device=SWITCH_PUSHBUTTON_NO
T 60700 52700 5 10 0 1 0 0 1
footprint=SuperSurfaceButton
}
C 55500 54200 1 0 1 input-2.sym
{
T 55500 54400 5 10 0 0 0 6 1
net=PB0:1
T 54900 54900 5 10 0 0 0 6 1
device=none
T 55000 54300 5 10 1 1 0 1 1
value=PB0
}
C 45300 54600 1 0 1 output-2.sym
{
T 44400 54800 5 10 0 0 0 6 1
net=PB0:1
T 45100 55300 5 10 0 0 0 6 1
device=none
T 44400 54700 5 10 1 1 0 7 1
value=PB0
}
C 45300 56700 1 0 1 output-2.sym
{
T 44400 56900 5 10 0 0 0 6 1
net=PD6:1
T 45100 57400 5 10 0 0 0 6 1
device=none
T 44400 56800 5 10 1 1 0 7 1
value=PD6
}
C 45300 56500 1 0 1 output-2.sym
{
T 44400 56700 5 10 0 0 0 6 1
net=PD7:1
T 45100 57200 5 10 0 0 0 6 1
device=none
T 44400 56600 5 10 1 1 0 7 1
value=PD7
}
C 55500 54500 1 0 1 input-2.sym
{
T 55500 54700 5 10 0 0 0 6 1
net=PD7:1
T 54900 55200 5 10 0 0 0 6 1
device=none
T 55000 54600 5 10 1 1 0 1 1
value=PD7
}
C 55500 54800 1 0 1 input-2.sym
{
T 55500 55000 5 10 0 0 0 6 1
net=PD6:1
T 54900 55500 5 10 0 0 0 6 1
device=none
T 55000 54900 5 10 1 1 0 1 1
value=PD6
}
C 50100 54200 1 270 0 capacitor-1.sym
{
T 50800 54000 5 10 0 0 270 0 1
device=CAPACITOR
T 49800 53900 5 10 1 1 0 0 1
refdes=C10
T 51000 54000 5 10 0 0 270 0 1
symversion=0.1
T 50400 53500 5 10 1 1 0 0 1
value=100uF
T 50100 54200 5 10 0 1 0 0 1
footprint=5mm_EL_Cap
}
C 50500 53000 1 0 1 ground.sym
C 50200 55600 1 270 0 input-2.sym
{
T 50400 55600 5 10 0 0 90 8 1
net=Vcc:1
T 50900 55000 5 10 0 0 90 8 1
device=none
T 49800 54800 5 10 1 1 180 7 1
value=Vcc
}
C 51300 55900 1 0 1 input-2.sym
{
T 51300 56100 5 10 0 0 0 6 1
net=Vcc:1
T 50700 56600 5 10 0 0 0 6 1
device=none
T 50800 56000 5 10 1 1 0 1 1
value=Vcc
}
T 59200 43500 14 10 1 0 0 0 1
Pig
T 59100 43200 14 8 1 0 0 0 1
http://dev.meatstand.com/meatstand/pig.git
T 60200 42900 14 10 1 0 0 0 1
1
T 60700 42900 14 10 1 0 0 0 1
1
T 63000 43200 14 10 1 0 0 0 1
XXX
T 63100 42900 14 10 1 0 0 0 1
Matthew O'Gorman <mog@rldn.net>
T 63400 43500 14 10 1 0 0 0 1
This schematic is licensed GPLv3
C 62300 54000 1 0 0 phone.sym
{
T 62495 55000 5 10 1 1 0 0 1
refdes=CONN3
T 63400 54800 5 10 0 1 0 0 1
footprint=35RASMT2BHNTRX
}
C 62000 54400 1 270 0 ground.sym
N 59100 48900 59000 48900 4
C 43400 43000 1 0 0 include-1.sym
{
T 43500 43100 5 10 1 1 0 0 1
device=none
T 44100 43100 5 10 1 1 0 0 1
footprint=oshw
T 43500 43400 5 10 1 1 0 0 1
refdes=ICO1
T 43800 43000 5 10 0 1 0 0 1
value=dnp
}
T 64600 43200 14 10 1 0 0 0 1
GIT_REV
C 43100 42600 1 0 0 EMBEDDEDtitle-bordered-A2.sym
[
B 43100 42600 23300 16500 15 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
L 62100 43400 62100 42800 15 0 0 0 -1 -1
B 58600 42800 7600 1400 15 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
L 58600 43400 66200 43400 15 0 0 0 -1 -1
B 43300 42800 22900 16100 15 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
L 43300 44569 43100 44569 15 0 0 0 -1 -1
L 43300 46537 43100 46537 15 0 0 0 -1 -1
L 43300 48506 43100 48506 15 0 0 0 -1 -1
L 43300 50474 43100 50474 15 0 0 0 -1 -1
L 43300 52443 43100 52443 15 0 0 0 -1 -1
L 43300 54411 43100 54411 15 0 0 0 -1 -1
L 43300 56380 43100 56380 15 0 0 0 -1 -1
L 43300 58348 43100 58348 15 0 0 0 -1 -1
L 45069 42800 45069 42600 15 0 0 0 -1 -1
L 47037 42800 47037 42600 15 0 0 0 -1 -1
L 49006 42800 49006 42600 15 0 0 0 -1 -1
L 50974 42800 50974 42600 15 0 0 0 -1 -1
L 52943 42800 52943 42600 15 0 0 0 -1 -1
L 54911 42800 54911 42600 15 0 0 0 -1 -1
L 56880 42800 56880 42600 15 0 0 0 -1 -1
L 58848 42800 58848 42600 15 0 0 0 -1 -1
L 60817 42800 60817 42600 15 0 0 0 -1 -1
L 62785 42800 62785 42600 15 0 0 0 -1 -1
L 64754 42800 64754 42600 15 0 0 0 -1 -1
L 66400 44569 66200 44569 15 0 0 0 -1 -1
L 66400 46537 66200 46537 15 0 0 0 -1 -1
L 66400 48506 66200 48506 15 0 0 0 -1 -1
L 66400 50474 66200 50474 15 0 0 0 -1 -1
L 66400 52443 66200 52443 15 0 0 0 -1 -1
L 66400 54411 66200 54411 15 0 0 0 -1 -1
L 66400 56380 66200 56380 15 0 0 0 -1 -1
L 66400 58348 66200 58348 15 0 0 0 -1 -1
L 45069 59100 45069 58900 15 0 0 0 -1 -1
L 47037 59100 47037 58900 15 0 0 0 -1 -1
L 49006 59100 49006 58900 15 0 0 0 -1 -1
L 50974 59100 50974 58900 15 0 0 0 -1 -1
L 52943 59100 52943 58900 15 0 0 0 -1 -1
L 54911 59100 54911 58900 15 0 0 0 -1 -1
L 56880 59100 56880 58900 15 0 0 0 -1 -1
L 58848 59100 58848 58900 15 0 0 0 -1 -1
L 60817 59100 60817 58900 15 0 0 0 -1 -1
L 62785 59100 62785 58900 15 0 0 0 -1 -1
L 64754 59100 64754 58900 15 0 0 0 -1 -1
T 69600 44400 5 10 0 0 0 0 1
graphical=1
T 58700 43200 15 8 1 0 0 0 1
FILE:
T 62200 43200 15 8 1 0 0 0 1
REVISION:
T 62200 42900 15 8 1 0 0 0 1
DRAWN BY: 
T 58700 42900 15 8 1 0 0 0 1
PAGE
T 60400 42900 15 8 1 0 0 0 1
OF
T 58700 43500 15 8 1 0 0 0 1
TITLE
T 43200 43584 15 8 1 0 0 4 1
A
T 43200 45553 15 8 1 0 0 4 1
B
T 43200 47521 15 8 1 0 0 4 1
C
T 43200 49490 15 8 1 0 0 4 1
D
T 43200 51458 15 8 1 0 0 4 1
E
T 43200 53427 15 8 1 0 0 4 1
F
T 43200 55395 15 8 1 0 0 4 1
G
T 43200 57364 15 8 1 0 0 4 1
H
T 44084 42700 15 8 1 0 0 4 1
1
T 46053 42700 15 8 1 0 0 4 1
2
T 48021 42700 15 8 1 0 0 4 1
3
T 49990 42700 15 8 1 0 0 4 1
4
T 51958 42700 15 8 1 0 0 4 1
5
T 53927 42700 15 8 1 0 0 4 1
6
T 55895 42700 15 8 1 0 0 4 1
7
T 57864 42700 15 8 1 0 0 4 1
8
T 59832 42700 15 8 1 0 0 4 1
9
T 61801 42700 15 8 1 0 0 4 1
10
T 63769 42700 15 8 1 0 0 4 1
11
T 65738 42700 15 8 1 0 0 4 1
12
T 66300 43584 15 8 1 0 0 4 1
A
T 66300 45553 15 8 1 0 0 4 1
B
T 66300 47521 15 8 1 0 0 4 1
C
T 66300 49490 15 8 1 0 0 4 1
D
T 66300 51458 15 8 1 0 0 4 1
E
T 66300 53427 15 8 1 0 0 4 1
F
T 66300 55395 15 8 1 0 0 4 1
G
T 66300 57364 15 8 1 0 0 4 1
H
T 44084 59000 15 8 1 0 0 4 1
1
T 46053 59000 15 8 1 0 0 4 1
2
T 48021 59000 15 8 1 0 0 4 1
3
T 49990 59000 15 8 1 0 0 4 1
4
T 51958 59000 15 8 1 0 0 4 1
5
T 53927 59000 15 8 1 0 0 4 1
6
T 55895 59000 15 8 1 0 0 4 1
7
T 57864 59000 15 8 1 0 0 4 1
8
T 59832 59000 15 8 1 0 0 4 1
9
T 61801 59000 15 8 1 0 0 4 1
10
T 63769 59000 15 8 1 0 0 4 1
11
T 65738 59000 15 8 1 0 0 4 1
12
T 66300 58864 15 8 1 0 0 4 1
I
T 43200 58864 15 8 1 0 0 4 1
I
]
C 45300 56000 1 0 1 output-2.sym
{
T 44400 56200 5 10 0 0 0 6 1
net=Indicator:1
T 45100 56700 5 10 0 0 0 6 1
device=none
T 44400 56100 5 10 1 1 0 7 1
value=Indicator
}
C 45300 56200 1 0 1 output-2.sym
{
T 44400 56400 5 10 0 0 0 6 1
net=Tip:1
T 45100 56900 5 10 0 0 0 6 1
device=none
T 44400 56300 5 10 1 1 0 7 1
value=Tip
}
C 45300 57100 1 0 1 output-2.sym
{
T 44400 57300 5 10 0 0 0 6 1
net=Disp_reset:1
T 45100 57800 5 10 0 0 0 6 1
device=none
T 44400 57200 5 10 1 1 0 7 1
value=Disp_reset
}
C 45300 57300 1 0 1 output-2.sym
{
T 44400 57500 5 10 0 0 0 6 1
net=Disp_on:1
T 45100 58000 5 10 0 0 0 6 1
device=none
T 44400 57400 5 10 1 1 0 7 1
value=Disp_on
}
C 45300 55400 1 0 1 output-2.sym
{
T 44400 55600 5 10 0 0 0 6 1
net=SDA:1
T 45100 56100 5 10 0 0 0 6 1
device=none
T 44400 55500 5 10 1 1 0 7 1
value=SDA
}
C 45300 55200 1 0 1 output-2.sym
{
T 44400 55400 5 10 0 0 0 6 1
net=SCL:1
T 45100 55900 5 10 0 0 0 6 1
device=none
T 44400 55300 5 10 1 1 0 7 1
value=SCL
}
C 52400 52800 1 0 0 connector2-1.sym
{
T 52600 53800 5 10 0 0 0 0 1
device=CONNECTOR_2
T 52400 53600 5 10 1 1 0 0 1
refdes=CONN5
T 52800 53400 5 10 0 1 0 0 1
footprint=JUMPER2
}
C 55500 52900 1 0 1 input-2.sym
{
T 55500 53100 5 10 0 0 0 6 1
net=SDA:1
T 54900 53600 5 10 0 0 0 6 1
device=none
T 55000 53000 5 10 1 1 0 1 1
value=SDA
}
C 55500 53200 1 0 1 input-2.sym
{
T 55500 53400 5 10 0 0 0 6 1
net=SCL:1
T 54900 53900 5 10 0 0 0 6 1
device=none
T 55000 53300 5 10 1 1 0 1 1
value=SCL
}
C 52400 54100 1 0 0 connector4-1.sym
{
T 54200 55000 5 10 0 0 0 0 1
device=CONNECTOR_4
T 52400 55500 5 10 1 1 0 0 1
refdes=CONN6
T 55000 57700 5 10 0 1 0 0 1
footprint=JUMPER4
}
C 45300 56900 1 0 1 output-2.sym
{
T 44400 57100 5 10 0 0 0 6 1
net=PD5:1
T 45100 57600 5 10 0 0 0 6 1
device=none
T 44400 57000 5 10 1 1 0 7 1
value=PD5
}
C 55500 55100 1 0 1 input-2.sym
{
T 55500 55300 5 10 0 0 0 6 1
net=PD5:1
T 54900 55800 5 10 0 0 0 6 1
device=none
T 55000 55200 5 10 1 1 0 1 1
value=PD5
}
