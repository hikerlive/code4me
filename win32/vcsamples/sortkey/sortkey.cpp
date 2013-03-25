/**
 * authoer gonggai
 */

#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

struct PinYin {
	bool isshuoxie; // 是否缩写
	bool isfull; // 是否是拼音
	std::string self;


	PinYin(std::string text, bool shuoxie, bool ispinyin) {
		self = text;
		isshuoxie = shuoxie;
		isfull = ispinyin;
	}

	std::string getSpace() {
		if (!isfull) {
			std::string a = "";
			for (int i=0; i<self.length(); i++) {
				a += " ";
			}
			return a;
		}
		return "";
	}
};

typedef std::vector<std::string> strings;
typedef std::vector<PinYin> PinYinList;
typedef std::vector<PinYinList> PinYinLists;

void mergePinYinText(PinYin& p1, PinYin& p2, PinYinList& result) {
	std::string text = "";
	if (p1.isfull && p2.isfull) { // 两者均是拼音
		if (!p1.isshuoxie) { // 不是缩写
			if (p1.self.find_first_of(' ') == std::string::npos) {
				// 首字母缩写
				text = p1.self.substr(0,1) + " " + p2.self.substr(0, 1);
				result.push_back(PinYin(text, true, true));
				if(p1.self.find_first_of('h') == 1) {
					if (p2.self.find('h') == 2) {
						text = p1.self.substr(0, 2) + " " + p2.self.substr(0, 2);
						result.push_back(PinYin(text, true, true));
					} else {
						text = p1.self.substr(0, 2) + " " + p2.self.substr(0, 1);
						result.push_back(PinYin(text, true, true));
					}
				} else {
					if (p2.self.find_first_of('h') == 1) {
						text = p1.self.substr(0, 1) + " " + p2.self.substr(0, 2);
						result.push_back(PinYin(text, true, true));
					}
				}
				text = p1.self + " " + p2.self.substr(0, 1);
				result.push_back(PinYin(text, true, true));
				if (p2.self.find_first_of('h') == 1) {
					text = p1.self + " " +p2.self.substr(0, 2);
					result.push_back(PinYin(text, true, true));
				}
			}
			text = p1.self + " " + p2.self;
			result.push_back(PinYin(text, false, true));
		} else { // 是缩写
			text = p1.self + " " +p2.self.substr(0, 1);
			result.push_back(PinYin(text, true, true));
			if (p2.self.find_first_of('h') == 1) {
				text = p1.self + " " + p2.self.substr(0, 2);
				result.push_back(PinYin(text, true, true));
			}
		}
	}
	else if (p1.isfull && !p2.isfull) { // 前者是拼音，后者是特殊词
		if (!p1.isshuoxie) { // 不是缩写
			if (p1.self.find_first_of(' ') == std::string::npos) {
				text = p1.self.substr(0, 1) + " " + p2.getSpace();
				result.push_back(PinYin(text, true, true));
				if (p1.self.find_first_of('h') == 1) {
					text = p1.self.substr(0, 2) + " " + p2.getSpace();
					result.push_back(PinYin(text, true, true));
				}
			}
			text = p1.self + " " + p2.getSpace();
			result.push_back(PinYin(text, false, true));
		} else { // 缩写
			text = p1.self + " " + p2.getSpace();
			result.push_back(PinYin(text, false, true));
		}
	} 
	else if (!p1.isfull && p2.isfull) { // 前者是特殊词，后者是拼音
		if (!p1.isshuoxie) {
			text = p1.getSpace() + " " + p2.self.substr(0, 1);
			result.push_back(PinYin(text, true, true));
			if (p2.self.find_first_of('h') == 1) {
				text = p1.getSpace() + " " + p2.self.substr(0, 2);
				result.push_back(PinYin(text, true, true));
			}
			text = p1.self + " " + p2.self;
			result.push_back(PinYin(text, false, true));
		} else {
		}
	}
	else if (!p1.isfull && !p2.isfull) { // 两者均是特殊词，不处理
	}
}

/**
 * merge jian xie pinyin
 */
PinYinList mergePinYinList(PinYinList& py1,PinYinList& py2) {
	PinYinList result;
	for (int i=0; i<py1.size(); i++) {
		for (int j=0; j<py2.size(); j++) {
			mergePinYinText(py1.at(i), py2.at(j), result);
		}
	}
	return result;
}

bool isFullPinYin(const char* split) {
	/* is full */
	int k=0;
	bool isfull = false;
	for (int i=0; i<strlen(split); i++) {
		char ch = split[i];
		if (ch >= 'a' && ch <= 'z') {
			k++;
		}
	}
	if (k > 0 && (k == strlen(split))) {
		isfull = true;
	}

	return isfull;
}

void splitStr(const char* str, PinYinList& subpys) {
	int size = strlen(str)*sizeof(char)+1;
	char* copy_split = (char*)malloc(size);
	memset(copy_split, 0, size);
	strcpy(copy_split, str);
			
	char* split2 = strtok(copy_split, "|");
	while (0 != split2) {
		subpys.push_back(PinYin(split2, false, true));
		/* next split2 */
		split2 = strtok(NULL, "|");
	}

	free(copy_split);
}

/**
 * pinyin convert to sortkey3
 *   - pinyin:
 *     zhong guo yan|yao|yin
 *   - sortkey3:
 *     z g y\tzh g y\tzhong g y\tzhong guo y\tzhong guo yin\tzhong guo yan\tzhong guo yao\tzhong guo yin\t
 */
void pinyin_to_sortkey3(char* pinyin, char** sortkey3, int& size_t) {
	PinYinLists pyTotal; 

	/* step1: divide pinyin words */
	char* split = strtok(pinyin, " ");
	while (0 != split) {
		PinYinList subpys;

		/* parse multi pinyin */
		char *p = strchr(split, '|');
		if (0 != p) {
			std::string astr = split;
			int pos = 0, idx=0;
			std::string split2="";
			while((idx = astr.find_first_of('|', pos)) != std::string::npos){
				split2 = astr.substr(pos, idx-pos);
				subpys.push_back(PinYin(split2, false, true));
				pos = idx+1;
			}
			split2 = astr.substr(pos);
			subpys.push_back(PinYin(split2, false, true));
		} else {
			bool isfull = isFullPinYin(split);
			subpys.push_back(PinYin(split, false, isfull));
		}
		pyTotal.push_back(subpys);

		/* next split */
		split = strtok(NULL, " ");
	}

	/* step2: merge sortkey3 */
	PinYinList pyResult;
	int total = pyTotal.size();
	if (total == 1) { // 仅一个拼音词
		pyResult = pyTotal.at(0);
	}
	if (total >= 2){ // 有两个拼音词及以上
		pyResult = mergePinYinList(pyTotal.at(0), pyTotal.at(1));
		for (int i=2; i<pyTotal.size(); i++) {
			pyResult = mergePinYinList(pyResult, pyTotal.at(i));
		}
	}
	
	std::string out="";
	strings pyExist;
	for (int i=0; i<pyResult.size(); i++) {
		std::string str = pyResult.at(i).self;
		bool exist = false;
		for (int j=0; j<pyExist.size(); j++) {
			if (pyExist.at(j).compare(str) == 0) {
				exist = true;
				break;
			}
		}
		if (!exist) {
			out += str + "\t";
			pyExist.push_back(str);
		}
	}

	if (!out.empty()) {
		size_t = out.length() * sizeof(char)+1;
		*sortkey3 = (char*) malloc(size_t);
		memset(*sortkey3, 0, size_t);
		strcpy(*sortkey3, out.c_str());
	}
}

/**
 * sortkey3 to keycodes
 *   - sortkey3
 *   - codes
 *     dial keyboard char-number maps.
 */
void sortkey3_to_codes(char* sortkey3, char** sortcodes, int& size_t) {
	std::string out = "";
	for (int i=0; i<strlen(sortkey3); i++) {
		char ch = sortkey3[i];
		if (ch >= 'a' && ch <=  'c') {
			out += "2";
		} else if (ch >= 'd' && ch <= 'f') {
			out += "3";
		} else if (ch >= 'g' && ch <= 'i') {
			out += "4";
		} else if (ch >= 'j' && ch <= 'l') {
			out += "5";
		} else if (ch >= 'm' && ch <= 'o') {
			out += "6";
		} else if (ch >='p' && ch <= 's') {
			out += "7";
		} else if (ch >='t' && ch <= 'v') {
			out += "8";
		} else if (ch >='w' && ch <= 'z') {
			out += "9";
		} else {
			out += ch;
		}
	}

	if (!out.empty()) {
		size_t = out.length() * sizeof(char)+1;
		*sortcodes = (char*) malloc(size_t);
		memset(*sortcodes, 0, size_t);
		strcpy(*sortcodes, out.c_str());
	}
}

void splitStr(const char* str, char delim) {
	std::string astr = str;
	int pos = 0, idx=0;
	while((idx = astr.find_first_of('|', pos)) != std::string::npos){
		std::string str = astr.substr(pos, idx-pos);
		pos = idx+1;
	}
	std::string stra = astr.substr(pos);
}

/*
int main(int argc, char** argv) {
	// splitStr("liu|you|gong|gou", '|');

	/*
	char pinyin[] = {"liu|you zhen xiong"};

	char* sortkey3 = NULL;
	int size_t = 0;
	pinyin_to_sortkey3(pinyin, &sortkey3, size_t);
	if (sortkey3 != NULL) {
		printf("sortkey:%s\n", sortkey3);
		
		char* sortcodes = NULL;
		sortkey3_to_codes(sortkey3, &sortcodes, size_t);
		if (NULL != sortcodes) {
			printf("sortcodes:%s\n", sortcodes);
			free(sortcodes);
		}
		free(sortkey3);
	} */

	/*
	char test[] = {"abc"};
	for (int i=0; i<strlen(test); i++) {
		test[i] = toupper(test[i]);
	}
	printf("test:%s\n", test);
	
	return 0;
} */