/**
 * @author gonggai
 */

#include <stdio.h>
#include <string>
#include <iostream>

extern int output_pinyin(const char* filePath);
extern "C" int output_baijiaxing();

int main(int argc, char* argv[]) {
	if (argc > 1) {
		if (strcmp(argv[1], "-py") == 0) {
			if (argc < 3) {
				std::cout << "-py has a parameter, new pinyin file path" << std::endl;
				return 0;
			}
			output_pinyin(argv[2]);
		}
		else if (strcmp(argv[1], "-baijiaxing") == 0) {
			output_baijiaxing();
		}
	} else {
		std::cout << "usage:\n"
			<< "\t1.-py [new pinyin file path], output new pinyin table.\n"
			<< "\t2.-baijiaxing, output baijiaxing pinyin.\n"
			<< std::endl;
	}
	return 0;
}
