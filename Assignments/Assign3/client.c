#include <stdio.h>
#include "libnetfiles.h"

int main(int argc, char const *argv[])
{
    if (netserverinit("localhost",0)<0) {
        printf("%d\n", errno);
        return 0;
    }
    return 0;
}