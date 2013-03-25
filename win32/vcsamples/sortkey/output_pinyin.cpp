/**
 * @author gonggai
 */
#include "hz2py.h"
#include <iostream>
#include <fstream>
#include <vector>

int output_pinyin(const char* filePath) {
	char line[256] = {0};
	char utf[256] = {0};
	char *ptr = 0;
	int uni;

	int no = 0;

	std::vector<std::string> abcs;
	for (int i=0; i<20902; i++) {
		abcs.push_back(_pinyin_table_[i]);
	}

	std::ifstream ifs(filePath);
	if (ifs.is_open()) {
		while (!ifs.eof()) {
			ifs.getline(line, 256, '\n');
			char* p = strchr(line, ':');
			if (p != 0) {
				*p = '\0';
				std::string hanzi = line;
				std::string pinyin = p+1;
				strcpy(utf, hanzi.c_str());

				ptr = utf;
				while (*ptr != '\0') {
					if ((*ptr & 0xF0) == 0xE0) {
						if (*(ptr+1) != '\0' && *(ptr+2) != '\0') {
							uni = (((int)(*ptr & 0x0F)) << 12)
								| (((int)(*(ptr+1) & 0x3F)) << 6)
								| (*(ptr+2) & 0x3F);
							if (uni > 19967 && uni < 40870) {
								std::cout<<"index:" << uni <<"hanzi:" << hanzi.c_str() << "pinyin:" << pinyin.c_str() << std::endl;
								if (!pinyin.empty()) {
									int index = uni-19968;
									abcs[index] = pinyin;
								}
								ptr += 3;
							} else {
								int i=0;
								while( i < 3) {
									if (*ptr == '\0') {
										break;
									}
									ptr ++;
									i ++;
								}							
							}
						} else {
							int i=0;
							while( i < 3) {
								if (*ptr == '\0') {
									break;
								}
								ptr ++;
								i ++;
							}
						}
					}
				}
			}
		}
	}

	ifs.close();

	// std::cout << "pinyin:" << _pinyin_table_[20901] << std::endl;

	char outfilePath[256] = {0};
	strcpy(outfilePath, filePath);
	char * k = strrchr(outfilePath, '\\');
	if (0 != k) {
		*k = '\0';
		strcat(outfilePath, "\\pytable.h");
	}
	std::ofstream ofs(outfilePath);
	if (ofs.is_open()) {
		int row = 0;
		int j=0;
		for (int i=0; i<abcs.size(); i++) {
			// std::cout<<"row count:" << row <<"index:"<< i << std::endl;
			if (row == 0 && j<11) {
				ofs<<"\""<<abcs[i].c_str()<<"\", ";
				if (j == 10) {
					ofs<<"\\"<<std::endl;
					j = 0;
					row ++;
				} else {
					j ++;
				}
			}
			else if (row > 0 && j<10) {
				ofs<<"\""<<abcs[i].c_str()<<"\", ";
				if (j == 9) {
					ofs<<"\\"<<std::endl;
					j=0;
					row ++;
				} else {
					j ++;
				}
			}
		}
	}
	ofs.close();


	return 0;
}
