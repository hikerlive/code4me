/**
 * 2013-03-28
 *
 * 演示sizeof编译器函数使用
 */

#include <string.h>
#include <malloc.h>
#include <stdio.h>

#include "sizeof_case.h"

void testSizeof1() {
	char* str = "hello";
	int n = 10;
	char* p = NULL;

	int a=0, b=0, c=0;
	a = sizeof(str); /* 6 */
	b = sizeof(n); /* 4 */
	c = sizeof(p); /* 4 */
	printf("a=%d, b=%d, c=%d\n", a, b, c);
}

void testSizeof2(char* arr[100]) {
	int a = sizeof(arr); /* 4 */
	printf("a=%d\n", a);
}

void testSizeof3(char** p) {
	*p = (char*) malloc(128);
	int a = sizeof(p); /* 4 */
	printf("a=%d\n", a);
}

