#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void syserr(char *msg) { perror(msg); exit(-1); }

void revert(char* buf)
{
  int i = 0, j = strlen(buf)-1;
  while(i < j) { 
    char c = buf[j];
    buf[j--] = buf[i];
    buf[i++] = c;
  }
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, n;
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

  printf("new incoming connection, block on receive\n");
  n = recv(newsockfd, buffer, 255, 0); 
  if(n < 0) syserr("can't receive from client"); 
  else buffer[n] = '\0';
  printf("SERVER GOT MESSAGE: %s\n", buffer); 

  revert(buffer);
  n = send(newsockfd, buffer, sizeof(buffer), 0);
  if(n < 0) syserr("can't send to server"); 
  printf("send message...\n");

  close(newsockfd); 
}
  close(sockfd); 
  return 0;
}
