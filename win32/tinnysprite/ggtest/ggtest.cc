// A simple test application:
// Test compile with python and scon tools.
// This is a quick script batch compile, so strong.

#include <stdio.h>
#include "strtest.h"

const char* usage_string={
	"Usage:\n"
	"\tA simple test application for ggtest here."
};	

int main(int argc, char* argv[])
{
	if (argc >=1 && argv)
	{
		printf(usage_string);
	}

	StrTest strt("Hello world");
	printf(strt.str());
}
