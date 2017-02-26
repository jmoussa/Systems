#ifndef mymalloc

#include <stdlib.h>

void* mymalloc(size_t numBytes, const char* filename, const int lineNumber);

void myfree(void* ptr, const char* filename, const int lineNumber);

void printHeap();

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)
#endif
