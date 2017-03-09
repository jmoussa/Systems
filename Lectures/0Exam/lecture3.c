#include <stdlib.h>
#include <unistd.h>

//allocates memory for a Node
//struct is a concatnation of types
struct fNode{
	char * token;
	struct fNode * next;
}

//TYPEDEF DEFINES A NEW TYPE
//typedef <stuff> <typename>

typedef struct fNode Node;
//defined struct fNode to Node




//ENUM
//Holds a set number of values
//enum <enum name>{<values...>};
//all caps by convention
enum volumes{PINT=2, QUART=4, GALLON=16};

enum volumes aJar = PINT;
// ex: if(aJar == PINT){do something}

//make enum a type
typedef enum volumes capacity;
capacity somesize = QUART;



//POINTERS - memory address

int b = 5;
//pointer p points to the address of b
int * p = &b;

//test to see where pointer points to
//dereference p by using *p to go to address and get value
if(*p==5){/*TRUE*/}

int b = 5;
int * p = &b;
//*p + 1 -> adds 1 to value at the pointer (ans = 6); 
//p + 1 -> adds 1 to memory address (NOT VALUE) by the length of type (increase by sizeof(int))

//Arrays are already pointers so & isn't needed
char stuff[] =  {'b','v','v'};
//pointer to front of array
char * letter = stuff;

//stuff[0]==*(letter+0)
//stuff[1]==*(letter+1)
//stuff[2]==*(letter+2)


//DYNAMIC MEMORY ALLOCATION
//malloc(<space requesting>*sizeof(<type>))

char * p = (char*)malloc(11*sizeof(char));

