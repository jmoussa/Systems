#ifndef count_h
#define count_h

#define maxTokenLength 100

typedef struct Node
{
    char token[maxTokenLength];
    int count;
    struct Node* next;
} Node;

Node* countFromSingleFile(char* fileAddress);

#endif
