all:	tcpclient tcpserver

tcpclient: tcpclient.c
	gcc -Wall $< -o $@

tcpserver: tcpserver.c
	gcc -Wall $< -o $@

clean:
	rm -f tcpclient tcpserver *.o *~ core

