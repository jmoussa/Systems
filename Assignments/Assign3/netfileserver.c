#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <assert.h>
#include <fcntl.h>

#ifdef MUTEX
pthread_mutex_t lock;
#endif

#define UNRESTRICTED_MODE 0
#define EXCLUSIVE_MODE 1
#define TRANSACTION_MODE 2

const int portno = 8999;
struct workerArgs {
    int socket;
};

int fd_count = -2;

typedef struct opener {
    int fd;
    int real_fd;
    int fmode;
    int flag;
    struct opener* next;
} opener;

typedef struct node {
    char path[512];
    struct opener* openers;
    struct node* next;
} node;

node* fileListHead = NULL;

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
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    count += last_comma < (a_str + strlen(a_str) - 1);
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
    char buffer[8192];
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
        return NULL;
    }
    // printf("Here is the message: %s\n", buffer);
    //Derive operation
    //method	:	tokens[0]
    //path 		:	actualpath_string
    //fmode 	:	tokens[2]_string fmode_int
    //flags 	:	tokens[3]_string flag_int
    char** tokens;
    tokens = str_split(buffer, '|');
    printf("Operation: %s\n", tokens[0]);
    char actualpath[512];
    char* ptr;
    ptr = realpath(tokens[1], actualpath);
    // printf("Path: %s\n", actualpath);
    // printf("fmode: %s\n", tokens[2]);
    // printf("flags: %s\n", tokens[3]);
    int flag = atoi(tokens[3]);
    int fmode = atoi(tokens[2]);
    int fake_return_fd = -1;
    //netopen
    if (strcmp(tokens[0], "open") == 0) {
#ifdef MUTEX
        pthread_mutex_lock(&lock);
#endif
        //Check if file exists
        if (access(actualpath, F_OK) == -1) {
            n = write(socket, "-1|NF", 16);
            if (n < 0) {
                perror("ERROR writing to socket");
                close(socket);
                free(wa);
                pthread_exit(NULL);
                return NULL;
            }
        }
        else {
            //find file in opened file list
            if (fileListHead == NULL) {
                //if not found in in the list
                //create
                fileListHead = (node*)malloc(sizeof(node));
                strcpy(fileListHead->path, actualpath);
                fileListHead->next = NULL;
                fileListHead->openers = (opener*)malloc(sizeof(opener));
                fileListHead->openers->real_fd = open(actualpath, flag);
                fileListHead->openers->fd = fd_count;
                fd_count--;
                fileListHead->openers->fmode = fmode;
                fileListHead->openers->flag = flag;
                fileListHead->openers->next = NULL;
                fake_return_fd = fileListHead->openers->fd;
                // printf("new open file real_fd:%d\n",fileListHead->openers->real_fd);
            }
            else {
                //if found in the list
                //check permission
                node* cursor = fileListHead;
                while (cursor != NULL) {
                    if (strcmp(cursor->path, actualpath) == 0) {
                        // printf("find! %s\n", cursor->path);
                        int allow = 1;
                        opener* o_cursor = cursor->openers;
                        while (o_cursor != NULL) {
                            if (o_cursor->fmode == TRANSACTION_MODE) {
                                n = write(socket, "-1|PER", 16);
                                if (n < 0) {
                                    perror("ERROR writing to socket");
                                    close(socket);
                                    free(wa);
                                    pthread_exit(NULL);
                                    return NULL;
                                }
                                allow = 0;
                                break;
                            }
                            if (fmode == UNRESTRICTED_MODE) {
                                if (flag == O_WRONLY) {
                                    if ((o_cursor->fmode == EXCLUSIVE_MODE) && ((o_cursor->flag == O_WRONLY) || (o_cursor->flag == O_RDWR))) {
                                        n = write(socket, "-1|PER", 16);
                                        if (n < 0) {
                                            perror("ERROR writing to socket");
                                            close(socket);
                                            free(wa);
                                            pthread_exit(NULL);
                                            return NULL;
                                        }
                                        allow = 0;
                                        break;
                                    }
                                }
                            }
                            if (fmode == EXCLUSIVE_MODE) {
                                if ((o_cursor->flag == O_WRONLY) || (o_cursor->flag == O_RDWR)) {
                                    n = write(socket, "-1|PER", 16);
                                    if (n < 0) {
                                        perror("ERROR writing to socket");
                                        close(socket);
                                        free(wa);
                                        pthread_exit(NULL);
                                        return NULL;
                                    }
                                    allow = 0;
                                    break;
                                }
                            }
                            if (fmode == TRANSACTION_MODE) {
                                n = write(socket, "-1|PER", 16);
                                if (n < 0) {
                                    perror("ERROR writing to socket");
                                    close(socket);
                                    free(wa);
                                    pthread_exit(NULL);
                                    return NULL;
                                }
                                allow = 0;
                                break;
                            }
                            o_cursor = o_cursor->next;
                        }
                        if (allow) {
                            o_cursor = cursor->openers;
                            while (o_cursor->next != NULL)
                                o_cursor = o_cursor->next;
                            o_cursor->next = (opener*)malloc(sizeof(opener));
                            o_cursor->next->real_fd = open(actualpath, flag);
                            o_cursor->next->fd = fd_count;
                            fd_count--;
                            o_cursor->next->fmode = fmode;
                            o_cursor->next->flag = flag;
                            o_cursor->next->next = NULL;
                            fake_return_fd = o_cursor->next->fd;
                        }
                        break;
                    }

                    cursor = cursor->next;
                }
            }
        }
        if (fake_return_fd != -1) {
            bzero(buffer, sizeof buffer);
            snprintf(buffer, sizeof buffer, "%d", fake_return_fd);
            n = write(socket, buffer, strlen(buffer));
            if (n < 0) {
                perror("ERROR writing to socket");
                close(socket);
                free(wa);
                pthread_exit(NULL);
            }
        }
#ifdef MUTEX
        pthread_mutex_unlock(&lock);
#endif
    }
    if (strcmp(tokens[0], "read") == 0) {
        int input_fake_fd = atoi(tokens[1]);
        int input_length = atoi(tokens[2]);
        int fd = -1;
        node* cursor = fileListHead;
        while (cursor != NULL) {
            opener* o_cursor = cursor->openers;
            while (o_cursor != NULL) {
                if (o_cursor->fd == input_fake_fd) {
                    fd = o_cursor->real_fd;
                    break;
                }
                o_cursor = o_cursor->next;
            }
            if (fd != -1)
                break;
            cursor = cursor->next;
        }
        // printf("%d\n", fd);
        if (fd == -1) {
            n = write(socket, "**fderr", strlen("**fderr"));
            if (n < 0) {
                perror("ERROR writing to socket");
                close(socket);
                free(wa);
                pthread_exit(NULL);
            }
        }
        else {
            char nbuffer[8192];
            bzero(nbuffer, sizeof(nbuffer));
            if (input_length > 8192)
                input_length = 8192;
            n = read(fd, nbuffer, input_length);
            n = write(socket, nbuffer, strlen(nbuffer));
            if (n < 0) {
                perror("ERROR writing to socket");
                close(socket);
                free(wa);
                pthread_exit(NULL);
            }
        }
    }
    if (strcmp(tokens[0], "write") == 0) {
        int input_fake_fd = atoi(tokens[1]);
        int input_length = atoi(tokens[3]);
        int fd = -1;
        node* cursor = fileListHead;
        while (cursor != NULL) {
            opener* o_cursor = cursor->openers;
            while (o_cursor != NULL) {
                if (o_cursor->fd == input_fake_fd) {
                    fd = o_cursor->real_fd;
                    break;
                }
                o_cursor = o_cursor->next;
            }
            if (fd != -1)
                break;
            cursor = cursor->next;
        }
        // printf("%d\n", fd);
        if (fd == -1) {
            n = write(socket, "**fderr", strlen("**fderr"));
            if (n < 0) {
                perror("ERROR writing to socket");
                close(socket);
                free(wa);
                pthread_exit(NULL);
            }
        }
        else {
            char nbuffer[8192];
            bzero(nbuffer, sizeof(nbuffer));
            if (input_length > 8192)
                input_length = 8192;
            n = write(fd, tokens[2], input_length);
            n = write(socket, '0', 1);
            if (n < 0) {
                perror("ERROR writing to socket");
                close(socket);
                free(wa);
                pthread_exit(NULL);
            }
        }
    }
    if (strcmp(tokens[0], "close") == 0) {
    	int succ=0;
        int input_fake_fd = atoi(tokens[1]);
        node* cursor = fileListHead;
        while (cursor != NULL) {
            if (cursor->openers->fd == input_fake_fd) {
                close(cursor->openers->real_fd);
                succ=1;
                cursor->openers = cursor->openers->next;
                break;
            }
            else {
                opener* o_cursor = cursor->openers;
                opener* tmp = o_cursor;
                while (o_cursor != NULL) {
                    if (o_cursor->fd == input_fake_fd) {
                        close(o_cursor->real_fd);
                        succ=1;
                        tmp->next = o_cursor->next;
                        break;
                    }
                    tmp = o_cursor;
                    o_cursor = o_cursor->next;
                }
            }
            cursor = cursor->next;
        }
        cursor = fileListHead;
        node* ctmp = cursor;
        while (cursor != NULL) {
            if (cursor->openers == NULL) {
                if (cursor == fileListHead) {
                    fileListHead = cursor->next;
                }
                else {
                    ctmp->next = cursor->next;
                }
                break;
            }
            ctmp = cursor;
            cursor = cursor->next;
        }
        if (succ) {
        	n = write(socket, '1', 1);
        } else {
        	n = write(socket, '0', 1);
        }
        if (n < 0) {
            perror("ERROR writing to socket");
            close(socket);
            free(wa);
            pthread_exit(NULL);
        }
    }
    free(tokens);
    // n = write(socket, "123", 18);
    // if (n < 0) {
    //     perror("ERROR writing to socket");
    //     close(socket);
    //     free(wa);
    //     pthread_exit(NULL);
    // }
    fprintf(stderr, "Socket %d closed\n", socket);
    close(socket);
    free(wa);
    pthread_exit(NULL);
}