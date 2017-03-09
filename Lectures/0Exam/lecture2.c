
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
	//printf("argc: %d\n", argc);
	//printf("var: %s\n", argv[0]);
	
	char wor[] = {'h','e','l','l','o', '\0'};
	char WOR[] = {'H', 'E', 'L', 'L', 'O', '\0'};

	// * p is a pointer address
	char * p = wor;
	int i = 0;
	printf(": %c, %c\n", WOR[16+i], *p);
	
	//sizeof() returns size of type in bytes
	printf(": %d\n",sizeof(int));
	
	//printf(": %c, %c\n", wor[0] , *p);
	//printf(": %s, %d, %d\n",wor,wor,wor[0]);
	
	return 0;
}
