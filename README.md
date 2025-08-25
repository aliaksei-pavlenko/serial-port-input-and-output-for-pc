this is application that do 3 things
1) read from /dev/ttyUSB0 and immediatelly write it to control terminal
2) read from controlling terminal and write to /dev/ttyUSB0
3) change setting of controlling terminal in raw mode + switch off echo but dont touch isig to be able terminate via same terminal emulator
   and btw there is handler that on exit restore all terminal setting that was before start of this application
   and this "transparent" mode is needed to avoid echo and line discipline and get ALL bytes that controlling terminal get - so just pass except that relate to signals
