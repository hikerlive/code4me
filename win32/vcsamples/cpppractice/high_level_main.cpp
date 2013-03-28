#include <stdio.h>

/**
 * high_level_program.cpp
 */

#include "sizeof_case.h"
#include "memory_case.h"

int main(int argc, char** argv) {
	printf("sizeof():\n");
	char* arr[100] = {0};
	testSizeof1();
	testSizeof2(arr);
	testSizeof3(arr);

	return 0;
}