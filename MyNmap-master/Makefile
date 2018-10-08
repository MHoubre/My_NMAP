CC=gcc
CFLAGS=-Wall -g -o
THR=-lpthread

CF=portlist.c activeDevice.c parser.c threadUtils.c TCPUtils.c ping.c scanTCP.c scanTCPSYN.c scanUDP.c scanIDLE.c UDPUtils.c main.c

make: 
	@$(CC) $(CF) $(CFLAGS) MyNmap $(THR)
	sudo valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./MyNmap 192.168.1.0/24

TestList:
	@$(CC) portlist.c testPortList.c $(CFLAGS) TestList
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./TestList

TestActiveDevice:
	@$(CC) portlist.c activeDevice.c testActiveDevice.c $(CFLAGS) TestActiveDevice
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./TestActiveDevice

TestScanTCP:
	@$(CC) portlist.c activeDevice.c TCPUtils.c threadUtils.c scanTCP.c testScanTCP.c $(CFLAGS) TestScanTCP $(THR)
	sudo valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./TestScanTCP

TestScanTCPSYN:
	@$(CC) portlist.c activeDevice.c TCPUtils.c scanTCPSYN.c testScanTCPSYN.c $(CFLAGS) TestScanTCPSYN $(THR)
	sudo valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./TestScanTCPSYN

TestScanUDP:
	@$(CC) portlist.c activeDevice.c UDPUtils.c threadUtils.c scanUDP.c testScanUDP.c $(CFLAGS) TestScanUDP $(THR)
	sudo valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./TestScanTCP


Test:
	TestList
	TestActiveDevice
	TestScanTCP
	TestScanTCPSYN
	TestScanUDP

clean:
	@sudo rm Test* MyNmap

send:
	scp -P 10027 MyNmap montrouge@tardis.telecomnancy.univ-lorraine.fr:/home/montrouge/

connect:
	ssh -p 10027 montrouge@tardis.telecomnancy.univ-lorraine.fr