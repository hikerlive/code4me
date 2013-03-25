/**
 * @author gonggai
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Node {
	char desc;
	char index[5];
	struct Node* next;
}NODE;


int read(const char* path, struct Node* node, int size) {
	FILE* fp = 0;
	size_t count = 0;

	fp = fopen(path, "rb");
	count = fread(node, size, 1, fp);
	fclose(fp);

	return 0;
}

int write(const char* path, struct Node* node, int size) {
	FILE* fp = 0;
	size_t count = 0;

	fp = fopen(path, "wb");
	count = fwrite(node, size, 1, fp);
	fclose(fp);

	return 0;
}

int test_write() {
	struct Node *node = NULL;
	int size_t = 0;

	size_t = sizeof(struct Node);
	node = (struct Node*) malloc(size_t);
	memset(node, 0, size_t);
	node->desc = 'a';
	strcpy(node->index, "china");
	node->next = node;

	printf("[WRITE] node:%p, desc:%c, index:%s, next:%p\n", node, node->desc, node->index, node->next);
	write("d:\\node.bin", node, size_t);

	free(node);

	return 0;
}

int test_read() {
	struct Node *node = 0;
	int size_t = 0;

	size_t = sizeof(struct Node);
	node = (struct Node*) malloc(size_t);
	memset(node, 0, size_t);

	read("d:\\node.bin", node, size_t);
	printf("[READ] node:%p, desc:%c, index:%s, next:%p\n", node, node->desc, node->index, node->next);

	free(node);

	return 0;
}

int binary_test_main(int argc, char* argv[]) {
	char* abc = 0;
	printf("[%s=====>]\n", "BINARY_TEST");

	test_write();

	abc = (char*)malloc(11);
	strcpy(abc, "helloworld");

	test_read();

	free(abc);

	return 0;
}
