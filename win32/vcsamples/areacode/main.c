/**
 * @author gonggai
 */

#include <stdio.h>
#include <string.h>

int binary_test_main(int argc, char* argv[]);
int str_test_main(int argc, char* argv[]);
int areacode_test_main(int argc, char* argv[]);


int main(int argc, char* argv[]) {
	printf("[%s=====>]\n", "MAIN_TEST");
	// binary_test_main(argc, argv);
	// str_test_main(argc, argv);
	areacode_test_main(argc, argv);
	// city_test_main(argc, argv);

	return 0;
}
  
