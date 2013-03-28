/**
 * 2013-03-28
 *
 * 演示各种内存使用时会存在的问题
 *
 */

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#include "memory_case.h"

void getMemory1(char* p) {
	p = (char*) malloc(16);
}

void testMemory1() {
	char* p = NULL;
	getMemory1(p);
	strcpy(p, "hello"); /* Error: break point, p is NULL. */
	printf(p);
}

void getMemory2(char** p) {
	*p = (char*) malloc(16);
}

void testMemory2() {
	char* p = NULL;
	getMemory2(&p);
	strcpy(p, "hello");
	printf(p); /* output hello, but memory leak. */
}

char* getString() {
	char* p = "hello";
	return p;
}

void testString() {
	char* p = NULL;
	p = getString();
	printf(p); /* invalid value */
}

char* getString2() {
	return "hello";
}

void testString2() {
	char* p = NULL;
	p = getString2();
	printf(p); /* OK: output hello */
}
