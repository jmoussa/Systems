//LECTURE 4 1/26

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int *funccorrect(int);	
//char * argv[] <==> char ** argv --- Stores an array of pointers (NEVER CHANGE/DEREFERENCE, only copy)
//char args[] --- Stores character array


//MEMORY ALLOCATION----------------------------------------------------------------------------------------------
//malloc(sizeof(<type>)) --- Returns a void * (CAST TO TYPE YOU WANT) --- malloc() is a request (CHECK return)
//int * p = (int*)malloc(number*sizeof(int))
//p[0]=5
//p[1]=4

int main(int argc, char *argv[]){
//MEMORY ALLOCATION---------------------------------------------------------------------------------------------
	int* p=(int*)malloc(5*sizeof(int)); //Requests space for an integer array of pointers for 5 integers
	
	//Check if program will fail	
	if(p==NULL){
		printf("Malloc Failed"); //Try Again
	}
	if(argc!= 1){
		printf("Incorrect number of parameters: \n Usage: ./pointersorter \"astring\"\n");
	}
	
	p[0]=5; // *p=5
	p[1]=5; // *(p+1)=4

//GARBAGE COLLECTION--------------------------------------------------------------------------------------------
//free(<memory location>) --- Should free memory everytime when you're done
	
	free(p);

	int*q=funccorrect(4);
	printf("%d",q);
	return 0;
}

//SCOPE OF POINTERS---------------------------------------------------------------------------------------------
int * func(int b){
	//int * p = b+1 --- Points to memory address b+1, NOT THE VALUE
	int c = b+1; 
	int * p=&c; //Points to address in memory of c

	return p; //A STACK VARIABLE, NO MALLOC, Scope is limited to function, disappears after return
}

int * funccorrect(int b){
	int c = b+1; 
	int * p=(int*)malloc(sizeof(int)); //Allocates memory for p
	*p=c;	//Points to address in memory of c

	return p; //Will return memory address of c
}
