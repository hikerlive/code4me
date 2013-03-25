/**
 * a libmath test project
 */

#include "math.h"
#include <stdio.h>

int
main(int argc, char* argv[]) {
	int s = 0;
	s = add(1, 2);
	printf("1+2=%d\n", s);
	
	s = sub(1, 2);
	printf("1-2=%d\n", s);
	return 0;
}
