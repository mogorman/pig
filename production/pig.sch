v 20110115 2
C 40000 40000 0 0 0 title-B.sym
C 46900 43700 1 0 0 atmega168pa.sym
{
T 47200 49100 5 10 1 1 0 0 1
footprint=atmega328_combo.fp
T 51000 49100 5 10 1 1 0 0 1
refdes=U1
}
C 43000 46300 1 0 0 icsp.sym
{
T 43200 47100 5 10 1 1 0 0 1
footprint=icsp
T 44400 47100 5 10 1 1 0 0 1
refdes=CONN1
}
C 43100 43900 1 0 0 battery-1.sym
{
T 43400 44800 5 10 0 0 0 0 1
device=BATTERY
T 43400 44400 5 10 1 1 0 0 1
refdes=B1
T 43400 45200 5 10 0 0 0 0 1
symversion=0.1
T 43300 43900 5 10 0 1 0 0 1
footprint=CR2032-BAT-HLD.fp
}
