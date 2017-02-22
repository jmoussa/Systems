#include "mymalloc.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_MEM 5000

typedef struct memoryAllocation{
  size_t numBytes;
  int free;
  void* ptr;
  struct memoryAllocation *next;
} node;





