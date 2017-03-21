#include "count.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Node* save(struct Node* head, char* token)
{
	//Convert to LowerCase
	int i = 0;
	for (i = 0; token[i]; i++) {
		token[i] = (char)tolower((int)token[i]);
	}
	//printf("-%s-\n", token);

	//Insert into LinkedList or Change Count
	if (head == NULL) {
		head = malloc(sizeof(struct Node));
		strcpy(head->token, token);
		head->count = 1;
		head->next = NULL;
	}
	else {
		struct Node* tmp = head;
		while (1) {
			if (strcmp(tmp->token, token) == 0) {
				(tmp->count)++;
				break;
			}
			if (tmp->next != NULL) {
				tmp = tmp->next;
			}
			else {
				tmp->next = malloc(sizeof(struct Node));
				strcpy(tmp->next->token, token);
				tmp->next->count = 1;
				tmp->next->next = NULL;
				break;
			}
		}
	}
	return head;
}

struct Node* countFromFile(char* fileAddress)
{
	struct Node* head = NULL;
	char c;
	char token[maxTokenLength];
	int i = 0;
	FILE* file;
	file = fopen(fileAddress, "r");
	if (file) {
		while ((c = getc(file)) != EOF) {
			if (i == 0) {
				//Allow (a-z,A-Z)
				if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
					token[i] = c;
					i++;
				}
			}
			else {
				//Allow (a-z,A-Z,0-9)
				if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
					token[i] = c;
					i++;
				}
				else {
					token[i] = '\0';
					i = 0;
					head = save(head, token);
				}
			}
		}
		if (i != 0) {
			token[i] = '\0';
			i = 0;
			head = save(head, token);
		}
		fclose(file);
	}
	return head;
}