#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <assert.h>

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
#ifdef MUTEX
    pthread_mutex_init(&lock, NULL);
#endif
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
#ifdef MUTEX
    pthread_mutex_destroy(&lock);
#endif
    close(sockfd);
    return 0;
}

char** str_split(char* a_str, const char a_delim)
{
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result) {
        size_t idx = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void* service_single_client(void* args)
{
    struct workerArgs* wa;
    int socket, n;
    char buffer[512];
    wa = (struct workerArgs*)args;
    socket = wa->socket;
    pthread_detach(pthread_self());
    fprintf(stderr, "Socket %d connected\n", socket);
    //Read string
    bzero(buffer, sizeof(buffer));
    n = read(socket, buffer, sizeof(buffer));
    if (n < 0) {
        perror("ERROR reading from socket");
        close(socket);
        free(wa);
        pthread_exit(NULL);
    }
    printf("Here is the message: %s\n", buffer);
    //Derive operation
    char** tokens;
    tokens = str_split(buffer, '|');
    printf("Operation: %s\n", tokens[0]);
    char actualpath[512];
    char* ptr;
    ptr = realpath(tokens[1], actualpath);
    printf("Path: %s\n", tokens[1]);
    printf("fmode: %s\n", tokens[2]);
    printf("flags: %s\n", tokens[3]);
    free(tokens);
    n = write(socket, "123", 18);
    if (n < 0) {
        perror("ERROR writing to socket");
        close(socket);
        free(wa);
        pthread_exit(NULL);
    }
    fprintf(stderr, "Socket %d closed\n", socket);
    close(socket);
    free(wa);
    pthread_exit(NULL);
#ifdef MUTEX
    pthread_mutex_lock(&lock);
#endif
#ifdef MUTEX
    pthread_mutex_unlock(&lock);
#endif
}
