killall Python
make udp-server.sparrow.u -j10 || exit 1
make udp-client.sparrow.u NODE=3 AVRDUDE_PORT=/dev/tty.usbserial-A600eHIs || exit 1
../../tools/runtest.py /dev/tty.usbserial-A4009YFR 
#../../tools/runtest.py /dev/tty.usbserial-A600eHIs
#killall Python 
