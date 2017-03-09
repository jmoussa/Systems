//LECTURE 5 malloc calloc realloc
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char ** argv){
//malloc
//	returns memory 
//calloc
//	stores memory contiguously and sets memory to zero
//realloc
//	changes the size of memory block pointed to to size bytes
//	contents are unchanged
//	copy is deleted

	int* p = (int*)malloc(sizeof(int));
	p = (int*)realloc(p, sizeof(int)*2); //works but deletes pointer p after
	//	*p -> might get old definition of p since realloc might not be done

	int* q=(int*)realloc(p, sizeof(int)*2); //this is convention
	//	*q -> will get new value since q doesn't have an old value

	
	return 0;
}
