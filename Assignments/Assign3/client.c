#include <stdio.h>
#include <string.h>
#include "libnetfiles.h"

int main(int argc, char const* argv[])
{
    if (netserverinit("localhost", EXCLUSIVE_MODE) < 0) {
        printf("%d\n", errno);
        return 0;
    }
    char *test="Traverse the linked list2";

    int fd=netopen("./test.txt", O_RDWR);
    if (fd==-1) {
    	printf("Cannot open!\n");
    	return 0;
    }
    char buffer[512];
    netread(fd, buffer, 10);
    printf("%s\n",buffer );
    netwrite(fd, test, strlen(test));
    netclose(fd);
    return 0;
}