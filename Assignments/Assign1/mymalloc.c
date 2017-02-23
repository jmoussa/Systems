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

static node *head;
node* prev = NULL;
node* current = head;

void* mymalloc(size_t numBytes, const char *file, const int lineNum){
  //no entries
  if(!head){
    head = (node*)sbrk(sizeof(node)+numBytes);
    if(head == (void*)-1){
      printf("No more memory on line %d, file, %s\n", lineNum, *file);
    }
    changeMemory(head, numBytes, 0, head+1, NULL);
    return head->ptr;
  }

  while(current){
    size_t difference = current->numBytes - numBytes;
    if(current->free){
      if(difference==0){
        current->free = 0;
      }else if(diff>0){
        current->free=0;
      }
      return current->ptr;
    }
    prev = current;
    current = current->next;
  }

  //Allocating on the heap
  current = (node*)sbrk(sizeof(node)+numBytes);
  if(current == (void*)-1){
    printf("No more memory on line %d, file %s\n", lineNum, *file);
    return NULL;
  }
  changeMemory(current, numBytes, 0, (void*)(current+1), NULL);
  prev->next = current;
  return current->ptr;
}



//-------------------------------------------------------------------------------
changeMemory(node *mem, size_t numBytes, int free, void *ptr, node *next){
    mem->numBytes = numBytes;
    mem->free = free;
    mem->ptr = ptr;
    mem->next = next;
    return;
}
