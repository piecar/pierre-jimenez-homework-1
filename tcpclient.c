#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void syserr(char* msg) { perror(msg); exit(-1); }

int main(int argc, char* argv[])
{
  //Sockfd: file descriptor; portno: port number; n: return values for read and write
  int sockfd, portno, n;
  char *input;
  struct hostent* server; // server info
  struct sockaddr_in serv_addr; //server address info
  char buffer[256];

  if(argc != 3) {
    fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
    return 1;
  }
  server = gethostbyname(argv[1]);
  if(!server) {
    fprintf(stderr, "ERROR: no such host: %s\n", argv[1]);
    return 2;
  }
  portno = atoi(argv[2]);
  
  /* To display host name
  {
  struct in_addr **addr_list; int i;
  printf("Official name is: %s\n", server->h_name);
  printf("    IP addresses: ");
  addr_list = (struct in_addr **)server->h_addr_list;
  for(i = 0; addr_list[i] != NULL; i++) {
    printf("%s ", inet_ntoa(*addr_list[i]));
  }
  printf("\n");
  }*/

  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // check man page
  if(sockfd < 0) syserr("can't open socket");
  printf("create socket...\n");

 // set all to zero, then update the sturct with info
  memset(&serv_addr, 0, sizeof(serv_addr)); 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr = *((struct in_addr*)server->h_addr);
  serv_addr.sin_port = htons(portno);

 // connect with filde descriptor, server address and size of addr
  if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    syserr("can't connect to server");
  printf("connect...\n");
  
  for(;;){
  	scanf("%s", &input);
  	if(strcmp(input, "ls-local") == 0)
  	{
  	}
  	if(strcmp(input, "ls-remote") == 0)
  	{
  	}
  	if(strcmp(input, "exit") == 0)
  	{
  	} 	
  	//tokenize if get/put
  	char *comm, filename;
  	comm = strtok(input, " ");
  	filename = strtok(NULL, " ");
  	if(strcmp(input, "get") == 0)
  	{
  		strcpy(buffer, "get");
  		strcat(buffer, filename);
  	}
  	if(strcmp(input, "put") == 0)
  	{
  	}
  	
  }
  

  printf("PLEASE ENTER MESSAGE: ");
  fgets(buffer, 255, stdin);
  n = strlen(buffer); if(n>0 && buffer[n-1] == '\n') buffer[n-1] = '\0';

  n = send(sockfd, buffer, strlen(buffer), 0);
  if(n < 0) syserr("can't send to server");
  printf("send...\n");
  
  n = recv(sockfd, buffer, 255, 0);
  if(n < 0) syserr("can't receive from server");
  else buffer[n] = '\0';
  printf("CLIENT RECEIVED MESSAGE: %s\n", buffer);

  close(sockfd);
  return 0;
}
