/**
 * @author gonggai
 */

#include <stdio.h>
#include <string.h>

int str_test_main(int argc, char* argv[]) {
	char str[] = {"hello"};
	char str1[6] = {"hello"};
	unsigned char desc = 0xFF;

	printf("[%s======>]\n", "STRING_TEST");
	printf("str len:%d, str1 len:%d\n", strlen(str), strlen(str1));
	printf("desc:%c\n", desc);

	if (desc == 0xFF) {
		printf("0xFF equal\n");
	}

	return 0;
}
