#include "count.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	// ****************************************************************
	// All tokens in the file and their counts will be
	// stored in the LinkedList 'head'
	// ****************************************************************
	struct Node* head = countFromSingleFile("./testcases/1.txt");


	// ****************************************************************
	// Print TOKENs and COUNTs
	// ****************************************************************
	struct Node* tmp=head;
	while (tmp!=NULL) {
		printf("%s\t\t%d\n", tmp->token,tmp->count);
		tmp=tmp->next;
	} 

	return 0;
}