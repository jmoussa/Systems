#include "mymalloc.h"
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{
	//B 
	int i;
	int *p;
	for(i=0; i<1001; i++){
		int *p=(int*)malloc(1);
		free(p);
	}
	printHeap();

	//A
	
	return 0;
}
