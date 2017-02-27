#include "mymalloc.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_MEM 5000

typedef struct memoryAllocation {
	size_t numBytes;
	int free;
	void* ptr;
	struct memoryAllocation* next;
} node;

static node* head;

//prototypes
void changeMemory(node* mem, size_t numBytes, int free, void* ptr, node* next);
int clean(node* mem, int place);

void* mymalloc(size_t numBytes, const char* file, const int lineNum)
{
  if((int)numBytes > 5000){
    printf("No more memory\n");
    return head;
  }
  
  //no entries
	if (!head) {
		head = (node*)sbrk(sizeof(node) + numBytes);
		if (head == (void*)-1) {
			printf("No more memory\n");
		}
		changeMemory(head, numBytes, 0, head + 1, NULL);
		return head->ptr;
	}
	node* prev = NULL;
	node* current = head;
	while (current) {
		size_t difference = current->numBytes - numBytes;
		if (current->free) {
			if (difference == 0) {
				current->free = 0;
			}
			else if (difference > 0) {
				current->free = 0;
			}
			return current->ptr;
		}
		prev = current;
		current = current->next;
	}

	//Allocating on the heap
	current = (node*)sbrk(sizeof(node) + numBytes);
	if (current == (void*)-1) {
		printf("No more memory\n");
		return NULL;
	}
	changeMemory(current, numBytes, 0, (void*)(current + 1), NULL);
	prev->next = current;
	return current->ptr;
}

void myfree(void* ptr, const char* file, const int lineNum)
{
	node* prev = NULL;
	node* current = head;
  //Check if it's already free
  if(current->free){
    printf("Pointer has already been freed\n");
    return;
  }

	while (current) {
		if (current->ptr == ptr) {
			if (current->free) {
				printf("Pointer already freed\n");
				return;
			}
			current->free = 1;
			//combining 2 free blocks if possible
			if (current->next && current->next->free) {
				changeMemory(current, current->numBytes + current->next->numBytes + sizeof(node), 1, current->ptr, current->next->next);
			}
			if (prev && prev->free) {
				changeMemory(prev, prev->numBytes + current->numBytes + sizeof(node), 1, prev->ptr, current->next);
			}
			clean(head, 0);
			return;
		}
		prev = current;
		current = current->next;
	}
	printf("ERROR Pointer not given\n");
	return;
}

//--------------------------------Extra Help Methods-------------------------------------
void changeMemory(node* mem, size_t numBytes, int free, void* ptr, node* next)
{
	mem->numBytes = numBytes;
	mem->free = free;
	mem->ptr = ptr;
	mem->next = next;
	return;
}

/* Return integer meanings
 * 0=Used
 * 1=Free
 * 2=NULL
 */
int clean(node* mem, int place)
{
	if (!mem) {
		return 2;
	}

	int iteration = clean(mem->next, place + 1);
	if (iteration == 0) {
		return 0;
	}
	else if (iteration == 1) {
		sbrk((mem->next->numBytes + sizeof(node)) * -1);
		mem->next = NULL;

		if (mem->free && place == 0) {
			sbrk((mem->numBytes + sizeof(node)) * -1);
			head = NULL;
			return 0;
		}

		return mem->free;
	}
	if (mem->free && place == 0) {
		sbrk((mem->numBytes + sizeof(node)) * -1);
		head = NULL;
		return 0;
	}
	return mem->free;
}

void printHeap()
{
	node* current = head;
	printf("%s\n", current ? "Heap contents:\n" : "Heap is Empty");

	int i = 1;
	while (current) {
		printf("Block %d: free: %s\tbytes: %zu\n", i++, (current->free) ? "yes" : "no", current->numBytes);
		current = current->next;
	}
	return;
}
