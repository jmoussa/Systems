#include <stdio.h>
#include "libnetfiles.h"

int main(int argc, char const *argv[])
{
    if (netserverinit("localhost",EXCLUSIVE_MODE)<0) {
        printf("%d\n", errno);
        return 0;
    }
    netopen("./serverd.exe", O_RDONLY);
        netopen("./server.exe", O_RDONLY);

    return 0;
}