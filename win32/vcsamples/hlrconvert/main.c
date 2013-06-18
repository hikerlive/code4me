/**
 * 将CCOD输出的号码归属地文件，转成纯区号的二进制文件。以号段分文件。
 * 输入：
 *   hlrtranslate.unl文件。其格式为86|0|1300100|10|。
 * 输出：
 *   hlr.13x|hlr.14x|hlr.15x|hlr.18x，其格式为：desc(1个字节)+shlr(5个字节)的字节块
 * 作者: gonggai
 * 时间：2013-06-18
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Area {
	unsigned char desc; /* 描述 0 - 无此数据、1~5 - 区号长度、0xFF - 重定向到扩展区 */
	char shlr[5]; /* 数据区，区号 或 扩展索引值*/
};

struct ShlrArea {
	struct Area shlrs[10]; /* 十个扩展 */
	int index; /* 手机号码中间5位编号 */
	int num; /* 在shlrArea列表中的编号 */
};

static struct Area hlr13[100000] = {0}; /* 13号段是00000~99999个号码 */
static struct ShlrArea shlr13[100] = {0}; /* 先预分配100个 */
static int shlr13_count = 0; /* 实际占有shlr13个数 */

static struct Area hlr14[100000] = {0};
static struct ShlrArea shlr14[100] = {0};
static int shlr14_count = 0;

static struct Area hlr15[100000] = {0};
static struct ShlrArea shlr15[100] = {0};
static int shlr15_count = 0;

static struct Area hlr18[100000] = {0};
static struct ShlrArea shlr18[100] = {0};
static int shlr18_count = 0;

/**
 * 以手机号码编号去查找存在的扩展区编号
 */
int find_exist_shlr(struct ShlrArea shlrs[], int index) {
	int i = 0;

	for (i = 0; i < shlr13_count; i++) {
		if (shlrs[i].index == index) {
			return i;
		}
	}

	return -1;
}

/**
 * 数字字符转整形数字
 */
int num_char_to_int(char chr) {
	switch (chr) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	default: return -1;
	}
}

void parse_area_line_hlr13(char * line) {

	int num = 0;
	char index[7] = {0};
	char area[4] = {0};
	char *str = NULL;
	int n7 = -1;
	int len = 0;
	int i=0; 
	char chr = 0;

	int nidx = 0;

	int npos = 0;
	int npos_shlr = 0;
	int shlr_index = 0;

	str = line + 3;
	len = strlen(str);
	for (i=0; i<len; i++) {
		chr = *(str+i);
		if (chr != '|') {
			index[i] = chr;
		} else {
			str = str + i + 1;
			break;
		}
	}

	num = strlen(index);
	if (num == 6) {
		n7 = num_char_to_int(index[5]);
		index[5] = '\0';
	}
	npos = atoi(index);

	len = strlen(str);
	for (i = 0; i < len; i++) {
		chr = *(str+i);
		if (chr != '|') {
			area[i] = chr;
		} else {
			break;
		}
	}

	/* printf("index:%d, [n7]:%d, area:%s\n", npos, n7, area); */

	if (n7 >= 0 && n7 <= 9) {
		shlr_index = find_exist_shlr(shlr13, npos);
		if (shlr_index == -1) {
			shlr_index = shlr13_count;
			shlr13_count ++;
		}
	
		/* 先标识该号码需重定向到扩展区 */
		hlr13[npos].desc = 0xFF;
		npos_shlr = 100000 + shlr_index*10;
		memcpy(hlr13[npos].shlr, &npos_shlr, 4);

		/* 以号码后一位去补充十个扩展区中某项数据 */
		shlr13[shlr_index].index = npos;
		shlr13[shlr_index].num = shlr_index;
		shlr13[shlr_index].shlrs[n7].desc = strlen(area);
		strcpy(shlr13[shlr_index].shlrs[n7].shlr, area);

	} else {
		hlr13[npos].desc = strlen(area);
		strcpy(hlr13[npos].shlr, area);
	}
}

void parse_area_line_hlr14(char * line) {
	int num = 0;
	char index[7] = {0};
	char area[4] = {0};
	char *str = NULL;
	int n7 = -1;
	int len = 0;
	int i=0; 
	char chr = 0;

	int nidx = 0;

	int npos = 0;
	int npos_shlr = 0;
	int shlr_index = 0;

	str = line + 3;
	len = strlen(str);
	for (i=0; i<len; i++) {
		chr = *(str+i);
		if (chr != '|') {
			index[i] = chr;
		} else {
			str = str + i + 1;
			break;
		}
	}
	num = strlen(index);
	if (num == 6) {
		n7 = num_char_to_int(index[5]);
		index[5] = '\0';
	}
	npos = atoi(index);

	len = strlen(str);
	for (i = 0; i < len; i++) {
		chr = *(str+i);
		if (chr != '|') {
			area[i] = chr;
		} else {
			break;
		}
	}

	/* printf("index:%d, [n7]:%d, area:%s\n", npos, n7, area); */

	if (n7 >= 0 && n7 <= 9) {
		shlr_index = find_exist_shlr(shlr14, npos);
		if (shlr_index == -1) {
			shlr_index = shlr14_count;
			shlr14_count ++;
		}
	
		/* 先标识该号码需重定向到扩展区 */
		hlr14[npos].desc = 0xFF;
		npos_shlr = 100000 + shlr_index*10;
		memcpy(hlr14[npos].shlr, &npos_shlr, 4);

		/* 以号码后一位去补充十个扩展区中某项数据 */
		shlr14[shlr_index].index = npos;
		shlr14[shlr_index].num = shlr_index;
		shlr14[shlr_index].shlrs[n7].desc = strlen(area);
		strcpy(shlr14[shlr_index].shlrs[n7].shlr, area);

	} else {
		hlr14[npos].desc = strlen(area);
		strcpy(hlr14[npos].shlr, area);
	}
}

void parse_area_line_hlr15(char * line) {
	int num = 0;
	char index[7] = {0};
	char area[4] = {0};
	char *str = NULL;
	int n7 = -1;
	int len = 0;
	int i=0; 
	char chr = 0;

	int nidx = 0;

	int npos = 0;
	int npos_shlr = 0;
	int shlr_index = 0;

	str = line + 3;
	len = strlen(str);
	for (i=0; i<len; i++) {
		chr = *(str+i);
		if (chr != '|') {
			index[i] = chr;
		} else {
			str = str + i + 1;
			break;
		}
	}
	num = strlen(index);
	if (num == 6) {
		n7 = num_char_to_int(index[5]);
		index[5] = '\0';
	}
	npos = atoi(index);

	len = strlen(str);
	for (i = 0; i < len; i++) {
		chr = *(str+i);
		if (chr != '|') {
			area[i] = chr;
		} else {
			break;
		}
	}

	/* printf("index:%d, [n7]:%d, area:%s\n", npos, n7, area); */

	if (n7 >= 0 && n7 <= 9) {
		shlr_index = find_exist_shlr(shlr15, npos);
		if (shlr_index == -1) {
			shlr_index = shlr15_count;
			shlr15_count ++;
		}
	
		/* 先标识该号码需重定向到扩展区 */
		hlr15[npos].desc = 0xFF;
		npos_shlr = 100000 + shlr_index*10;
		memcpy(hlr15[npos].shlr, &npos_shlr, 4);

		/* 以号码后一位去补充十个扩展区中某项数据 */
		shlr15[shlr_index].index = npos;
		shlr15[shlr_index].num = shlr_index;
		shlr15[shlr_index].shlrs[n7].desc = strlen(area);
		strcpy(shlr15[shlr_index].shlrs[n7].shlr, area);

	} else {
		hlr15[npos].desc = strlen(area);
		strcpy(hlr15[npos].shlr, area);
	}
}

void parse_area_line_hlr18(char * line) {

	int num = 0;
	char index[7] = {0};
	char area[4] = {0};
	char *str = NULL;
	int n7 = -1;
	int len = 0;
	int i=0; 
	char chr = 0;

	int nidx = 0;

	int npos = 0;
	int npos_shlr = 0;
	int shlr_index = 0;

	str = line + 3;
	len = strlen(str);
	for (i=0; i<len; i++) {
		chr = *(str+i);
		if (chr != '|') {
			index[i] = chr;
		} else {
			str = str + i + 1;
			break;
		}
	}
	num = strlen(index);
	if (num == 6) {
		n7 = num_char_to_int(index[5]);
		index[5] = '\0';
	}
	npos = atoi(index);

	len = strlen(str);
	for (i = 0; i < len; i++) {
		chr = *(str+i);
		if (chr != '|') {
			area[i] = chr;
		} else {
			break;
		}
	}

	/* printf("index:%d, [n7]:%d, area:%s\n", npos, n7, area); */

	if (n7 >= 0 && n7 <= 9) {
		shlr_index = find_exist_shlr(shlr18, npos);
		if (shlr_index == -1) {
			shlr_index = shlr18_count;
			shlr18_count ++;
		}
	
		/* 先标识该号码需重定向到扩展区 */
		hlr18[npos].desc = 0xFF;
		npos_shlr = 100000 + shlr_index*10;
		memcpy(hlr18[npos].shlr, &npos_shlr, 4);

		/* 以号码后一位去补充十个扩展区中某项数据 */
		shlr18[shlr_index].index = npos;
		shlr18[shlr_index].num = shlr_index;
		shlr18[shlr_index].shlrs[n7].desc = strlen(area);
		strcpy(shlr18[shlr_index].shlrs[n7].shlr, area);

	} else {
		hlr18[npos].desc = strlen(area);
		strcpy(hlr18[npos].shlr, area);
	}
}

void output_hlr13_file(const char* path) {
	FILE* fp = NULL;
	int i = 0;
	int j = 0;

	fp = fopen(path, "wb");
	if (NULL == fp) {
		printf("open hlr13 file failed\n");
		return;
	}

	for (i = 0; i < 100000; i++) {
		fwrite(&hlr13[i], sizeof(struct Area), 1, fp);
	}

	for (i = 0; i < shlr13_count; i++) {
		for (j = 0; j < 10; j++) {
			fwrite(&shlr13[i].shlrs[j], sizeof(struct Area), 1, fp);
		}
	}

	fclose(fp);
}

void output_hlr14_file(const char* path) {
	FILE* fp = NULL;
	int i = 0;
	int j = 0;

	fp = fopen(path, "wb");
	if (NULL == fp) {
		printf("open hlr14 file failed\n");
		return;
	}

	for (i = 0; i < 100000; i++) {
		fwrite(&hlr14[i], sizeof(struct Area), 1, fp);
	}

	for (i = 0; i < shlr14_count; i++) {
		for (j = 0; j < 10; j++) {
			fwrite(&shlr14[i].shlrs[j], sizeof(struct Area), 1, fp);
		}
	}

	fclose(fp);
}

void output_hlr15_file(const char* path) {
	FILE* fp = NULL;
	int i = 0;
	int j = 0;

	fp = fopen(path, "wb");
	if (NULL == fp) {
		printf("open hlr15 file failed\n");
		return;
	}

	for (i = 0; i < 100000; i++) {
		fwrite(&hlr15[i], sizeof(struct Area), 1, fp);
	}

	for (i = 0; i < shlr15_count; i++) {
		for (j = 0; j < 10; j++) {
			fwrite(&shlr15[i].shlrs[j], sizeof(struct Area), 1, fp);
		}
	}

	fclose(fp);
}

void output_hlr18_file(const char* path) {
	FILE* fp = NULL;
	int i = 0;
	int j = 0;

	fp = fopen(path, "wb");
	if (NULL == fp) {
		printf("open hlr18 file failed\n");
		return;
	}

	for (i = 0; i < 100000; i++) {
		fwrite(&hlr18[i], sizeof(struct Area), 1, fp);
	}

	for (i = 0; i < shlr18_count; i++) {
		for (j = 0; j < 10; j++) {
			fwrite(&shlr18[i].shlrs[j], sizeof(struct Area), 1, fp);
		}
	}

	fclose(fp);
}

int main (int argc, char* argv[]) {
	FILE* fp = NULL;
	char line[256] = {0};
	char* ps = NULL;
	char* pline = NULL;

	fp = fopen("d:\\hlrtranslate.unl", "r");
	if (NULL == fp) {
		printf("file open failed\n");
		return -1;
	}

	while (!feof(fp)) {
		ps = fgets(line, sizeof(line), fp);
		if ((pline = strstr(line, "|13")) != NULL) {
			parse_area_line_hlr13(pline);
		}
		else if ((pline = strstr(line, "|14")) != NULL) {
			parse_area_line_hlr14(pline);
		}
		else if ((pline = strstr(line, "|15")) != NULL) {
			parse_area_line_hlr15(pline);
		}
		else if ((pline = strstr(line, "|18")) != NULL) {
			parse_area_line_hlr18(pline);
		}
	}
	fclose(fp);

	printf("shlr13_count:%d, shlr14_count:%d, shlr15_count:%d, shlr18_count:%d\n", 
		shlr13_count, shlr14_count, shlr15_count, shlr18_count);

	output_hlr13_file("d:\\hlr.13x");
	output_hlr14_file("d:\\hlr.14x");
	output_hlr15_file("d:\\hlr.15x");
	output_hlr18_file("d:\\hlr.18x");

	return 0;
}