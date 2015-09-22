#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

void syserr(char *msg) { perror(msg); exit(-1); }
void ftpcomm(int newsockfd, char* buffer);
void readandsend(int tempfd, int newsockfd, char* buffer);

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, pid;
  struct sockaddr_in serv_addr, clt_addr;
  socklen_t addrlen;
  char buffer[256];

  if(argc != 2) { 
    fprintf(stderr,"Usage: %s <port>\n", argv[0]);
    return 1;
  } 
  portno = atoi(argv[1]);

  sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  if(sockfd < 0) syserr("can't open socket"); 
  	printf("create socket...\n");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
    syserr("can't bind");
  printf("bind socket to port %d...\n", portno);

  listen(sockfd, 5); 

for(;;) {
  printf("wait on port %d...\n", portno);
  addrlen = sizeof(clt_addr); 
  newsockfd = accept(sockfd, (struct sockaddr*)&clt_addr, &addrlen);
  if(newsockfd < 0) syserr("can't accept"); 
  
  pid = fork();
   if (pid < 0)
     syserr("Error on fork");
   if (pid == 0)
   {
     close(sockfd);
     ftpcomm(newsockfd, buffer);
     exit(0);
   }
   else
	 close(newsockfd);
}
  close(sockfd); 
  return 0;
}

void ftpcomm(int newsockfd, char* buffer)
{
	int n, size, tempfd;
    struct stat filestats;
	char * filename;
	for(;;)
	{
		n = recv(newsockfd, buffer, sizeof(buffer), 0);
		if(n < 0) syserr("can't receive from client");
		sscanf(buffer, "%s", command);
		
		if(strcmp(command, "ls") == 0)
		{
			system("ls -a | cat >remotelist.txt");
			stat("remotelist.txt", &filestats);
			size = filestats.st._size;
            size = htonl(size);        
			send(newsockfd, &size, sizeof(int), 0)
			tempfd = open("remotelist.txt", O_REDONLY);
			if(tempfd < 0) syserr("failed to open remotelist.txt, server side");
			readandsend(tempfd, newsockfd, buffer);			
		}
		
		if(strcmp(command, "exit") == 0)
		{
			printf("Server shutting down\n");
			int = 1;
			send(newsockfd, &1, sizeof(int), 0);
			break;  // check to make sure it doesn't need to be exit
		}
		
		if(strcmp(command, "get") == 0)
		{
			sscanf(buffer, "%*s%s", filename);
			stat(filename, filestats);
			size = filestats.st_size;
            size = htonl(size);      
			send(newsockfd, &size, sizeof(int), 0);
			tempfd = open(filename, O_REDONLY);
			if(tempfd < 0) syserr("failed to get file, server side");
			readandsend(tempfd, newsockfd, buffer);			
		}
	}
}

void readandsend(int tempfd, int newsockfd, char* buffer)
{
	while (1)
	{
		int bytes_read = read(tempfd, buffer, sizeof(buffer)); //is buffer cleared here?
		if (bytes_read == 0) // We're done reading from the file
			break;

		if (bytes_read < 0) syserr("error reading file"); 
		
		int total = 0;
		int n;
		int bytesleft = sizeof(buffer);
		while(total < sizeof(buffer))
		{
			n = send(newsockfd, buffer+total, bytesleft, 0);
			if (n == -1) 
			{ 
			   syserr("error sending file"); 
			   break;
			}
			total += n;
			bytesleft -= n;
		}
	}
}
