#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "libnetfiles.h"

const int portno=8999;
int inited = 0;
int fmode =0;
struct sockaddr_in serv_addr;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int netserverinit(char * hostname, int filemode) {
	if ((filemode<0)||(filemode>2)) {
		errno=INVALID_FILE_MODE;
		return -1;
	}
	fmode=filemode;
	//Prepare server address
    struct hostent *server;
    server = gethostbyname(hostname);
    if (server == NULL) {
    	h_errno=HOST_NOT_FOUND;
        return -1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    inited = 1;
}

int netopen(const char *pathname, int flags) {
	if (!inited) {
		h_errno=HOST_NOT_FOUND;
        return -1;		
	}
	int sockfd, n;
    //Open new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    //Connect
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    n = write(sockfd,pathname,strlen(pathname));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}