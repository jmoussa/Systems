#ifndef mymalloch
#define mymalloch

#include <stdlib.h>

void* mymalloc(size_t numBytes, const char *filename, const int lineNumber);

void* myfree(void *ptr, const char *filename, const int lineNumber);

#define malloc(x) mymalloc((x),_FILE_,_LINE_)
#define free(x) myfree((x),_FILE_,_LINE_)
#endif
