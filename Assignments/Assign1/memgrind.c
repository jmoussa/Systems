#include "mymalloc.h"

int main(int argc, char const *argv[])
{
	void* p= malloc(100);
	free(p);
	printHeap();
	return 0;
}