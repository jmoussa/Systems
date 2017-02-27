#include "mymalloc.h"
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{
	//B 
	int i;
	char *p;
	for(i=0; i<1001; i++){
		p=(char*)malloc(1);
		free(p);
	}
	printHeap();

	//A
  p = (char*)malloc(5001);
  printHeap();
	return 0;
}
