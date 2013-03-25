/**
 * @author gonggai
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
std::vector<std::string> shlrs;

int load_hlrtable() {	
	std::ifstream ifs;
	ifs.open("D:\\develop\\vcsamples\\areacode\\hlr.13x", std::ios::in);
	if (ifs.is_open()) {
		char line[6] = {0};
		while (!ifs.eof()) {
			ifs.get(line, 6);
			shlrs.push_back(line);
		}
	}
	ifs.close();
	return 0;
}

int get_areacode(const char* dn, char* areacode, int size_t){
	char index[5] = {0};
	int idx = 0;
	std::string shlr = "";
	char desc = '\0';

	strncpy(index, dn+2, 5);
	idx = atoi(index);
	if (idx > shlrs.size()) {
		return -1;
	}

	shlr = shlrs.at(idx);
	desc = shlr.at(0);
	if (desc == 0x30) { // 无此数据
		return -1;
	}
	else if (desc >= 0x31 && desc <= 0x35) { // 区号长度
		int len = desc - 0x30;
		strncpy(areacode, shlr.substr(1, len).c_str(), len);
	}
	else if (desc == 0xFF ) { // 有扩展
	}

	return 0;
}

int cread() {
	FILE *fp;
	int c;
	fp = fopen("D:\\develop\\vcsamples\\areacode\\hlr.13x", "r");
	while((c = fgetc(fp)) != EOF) {
		printf("%c", c);
	}
	fclose(fp);
	return 0;
}

int main(int argc, char* argv[]) {
	char number[] = {"13581877736"};
	char areacode[4] = {0};
	int ret = 0;

	load_hlrtable();
	ret = get_areacode(number, areacode, 5);
	printf("ret:%d, number:%s, areacode:%d\n", ret, number, areacode);

	return 0;
}
