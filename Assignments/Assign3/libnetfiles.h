#ifndef __libnetfiles_h__
#define __libnetfiles_h__

#include <unistd.h>
#include <errno.h>

#define INVALID_FILE_MODE 10001

int netopen(const char *pathname, int flags);
ssize_t netread(int fildes, void *buf, size_t nbyte);
ssize_t netwrite(int fildes, const void *buf, size_t nbyte);
int netclose(int fd);
int netserverinit(char * hostname, int filemode);


#endif