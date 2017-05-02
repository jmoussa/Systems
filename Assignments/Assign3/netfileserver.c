#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#ifdef MUTEX
pthread_mutex_t lock;
#endif
const int portno = 8999;
struct workerArgs {
	int socket;
};
void* service_single_client(void* args);

void error(const char* msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[])
{
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	struct workerArgs* wa;
	pthread_t worker_thread;

	//Open new socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	//Prepare server address(listen address)
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	//Bind socket
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	//Listen
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	while (1) {

		newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
		if (newsockfd < 0) {
			error("ERROR on accept");
		}
		wa = malloc(sizeof(struct workerArgs));
		wa->socket = newsockfd;
		if (pthread_create(&worker_thread, NULL, service_single_client, wa) != 0) {
			free(wa);
			close(newsockfd);
			close(sockfd);
			error("Could not create a worker thread");
		}
	}
	close(sockfd);
	return 0;
}

void* service_single_client(void* args)
{
	struct workerArgs* wa;
	int socket,n;
	char buffer[256];
	wa = (struct workerArgs*)args;
	socket = wa->socket;
	pthread_detach(pthread_self());
	fprintf(stderr, "Socket %d connected\n", socket);
	
	bzero(buffer, 256);
	n = read(socket, buffer, 255);
	if (n < 0) {
		perror("ERROR reading from socket");
		close(socket);
		free(wa);
		pthread_exit(NULL);
	}
	printf("Here is the message: %s\n", buffer);

	n = write(socket, "123", 18);
	if (n < 0){
		perror("ERROR writing to socket");
		close(socket);
		free(wa);
		pthread_exit(NULL);
	}
	fprintf(stderr, "Socket %d closed\n", socket);
	close(socket);
	free(wa);
	pthread_exit(NULL);
}