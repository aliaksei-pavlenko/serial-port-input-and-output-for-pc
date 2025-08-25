this is application that do 3 things
1) read from /dev/ttyUSB0 and immediatelly write it to control terminal
2) read from controlling terminal and write to /dev/ttyUSB0
3) change setting of controlling terminal in raw mode + switch off echo but dont touch isig to be able terminate via same terminal emulator
   and btw there is handler that on exit restore all terminal setting that was before start of this application
   and this "transparent" mode is needed to avoid echo and line discipline and get ALL bytes that controlling terminal get - so just pass except that relate to signals

dont forget that on /dev/ttyUSB0 terminal driver make raw and -echo mode - also that it pass data without any holding it

setting for serial in grub 
serial --unit=0 --speed=115200 --word=8 --parity=no --stop=1
terminal_output serial
unit 0 means first com port

setting of ttyUSB0 driver
stty -F /dev/ttyUSB0 115200 cs8 -cstopb -parenb
Explanation:
   115200 → baud rate
   cs8 → character size 8 bits
   -cstopb → 1 stop bit (cstopb = 2 stop bits)
   -parenb → disable parity

and also 
stty -F /dev/ttyUSB0 raw -echo
