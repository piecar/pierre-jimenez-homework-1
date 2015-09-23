all:	ftpclient

tcpclient: ftpclient.c
	gcc -Wall $< -o $@

clean:
	rm -f ftpclient *.o *~ core

