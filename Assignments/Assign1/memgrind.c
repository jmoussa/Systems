#include "mymalloc.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>



void test1()
{

	//1000 separate malloc()s of 1 byte, then free() the 1000 1 byte pointers one by one
	void* p[1000];
	int i;
	for (i = 0; i < 1000; i++) {
		p[i] = malloc(1);
	}
	for (i = 0; i < 1000; i++) {
		free(p[i]);
	}
}

void test2()
{
	//first malloc() 1 byte and immediately free it - do this 1000 times
	void* p[1000];
	int i;
	for (i = 0; i < 1000; i++) {
		p[i] = malloc(1);
		free(p[i]);
	}
}

void test3() {
	//Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer - do this 1000 times
	void* p[1000];
	int i=0;
	int malloc_index=0;
	int free_index=-1;
	p[0]=malloc(1);
	while (1) {
	int r = rand()%2;
		if (r==0) { //malloc
			if (malloc_index<999) {
				malloc_index++;
				p[malloc_index]=malloc(1);
			}
		}
		if (r==1) { //free
			if (free_index<malloc_index) {
				free_index++;
				free(p[free_index]);
			}
		}
		if ((malloc_index==999)&&(free_index==999)) {
			return;
		}
	}

}

void test4() {
	// Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many times
	void* p[1000];
	int i=0;
	int malloc_num=0;
	int free_num=0;
	int free_index=-1;
	int p_index=-1;
	while (1) {
		int r = rand()%2;
		if (r==0) { //malloc
			if (malloc_num<1000) {
				int size = (rand()%64) +1; //1 to 64
				p_index++;
				p[p_index] = malloc(size);
				malloc_num++;
			}
		}
		if (r==1) { //free
			if (free_index<p_index) {
				free_index++;
				free(p[free_index]);
			}
		}
		if ((malloc_num==1000)&&(free_index==p_index)) {
			return;
		}
	}

}

int main(int argc, char const* argv[])
{
	srand(time(NULL));
	double time_spent;
	int i;
	clock_t begin;
	clock_t end;
	//Test 1
	begin= clock();
	for (i = 0; i < 100; i++) {
		test1();
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Test1: \n");
	printf("1000 separate malloc()s of 1 byte, then free() the 1000 1 byte pointers one by one\n");
	printf("Costs %f seconds\n", time_spent);
	printf("\n");
	//Test 2
	begin= clock();
	for (i = 0; i < 100; i++) {
		test2();
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Test2: \n");
	printf("first malloc() 1 byte and immediately free it - do this 1000 times\n");
	printf("Costs %f seconds\n", time_spent);
	printf("\n");
	//Test 3
	begin= clock();
	for (i = 0; i < 100; i++) {
		test3();
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Test3: \n");
	printf("Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer - do this 1000 times\n");
	printf("Costs %f seconds\n", time_spent);
	printf("\n");
	//Test 4
	begin= clock();
	for (i = 0; i < 100; i++) {
		test4();
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Test4: \n");
	printf("Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many times\n");
	printf("Costs %f seconds\n", time_spent);
	printf("\n");
	return 0;
}
