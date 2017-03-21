#ifndef count_h
#define count_h

#define maxTokenLength 100

struct Node
{
    char token[maxTokenLength];
    int count;
    struct Node* next;
};

struct Node* countFromFile(char* fileAddress);

#endif
