/**
 * @author gonggai
 */

#include "areacode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HLR_13X_PATH "D:\\devs\\hikerlive\\code4me\\win32\\vcsamples\\areacode\\hlr.13x"
#define HLR_14X_PATH "D:\\devs\\hikerlive\\code4me\\win32\\vcsamples\\areacode\\hlr.14x"
#define HLR_15X_PATH "D:\\devs\\hikerlive\\code4me\\win32\\vcsamples\\areacode\\hlr.15x"
#define HLR_18X_PATH "D:\\devs\\hikerlive\\code4me\\win32\\vcsamples\\areacode\\hlr.18x"

struct Area{
	unsigned char desc;
	char shlr[5];
};

int parse_area_code(struct Area* area, long offset, int no7, FILE* fp) {
	int ret = 0;
	unsigned int index = 0;

	fseek(fp, offset, SEEK_SET);
	fread(area, sizeof(struct Area), 1, fp);
	if (area->desc == 0) {
		ret = -1;
	}
	else if (area->desc == 0xFF) {
		/** 
		 *	hlr扩展表索引，指向十个扩展shlr项，其含义是当前号码向后一位以0~9排列的shlr码表
		 *  0x000186DC		-> 文件读取位置：100060
		 *  50092-当前索引   -> 文件读取位置：50092*6 = 300552
		 *  有几种情况：
		 *   a.是100060读写位置，area结构不对,desc为0。
		 *   b.是100060索引，即100060*6所对应的shlr码表。（对）
		 *   c.是index*6+100060的文件读取益，不对。
		 */
		memcpy(&index, area->shlr, 4);
		ret = parse_area_code(area, index*6+no7*6, no7, fp);
	}
	else if (area->desc >= 1 && area->desc <= 5) {
		ret = 0;
	}
	return ret;
}

int get_area_code_mobile(const char* dn, char* area_code, int size_t) {
	FILE* fp;
	int ret = 0;
	char prefix[3] = {0};
	char pos[6] = {0};
	int index = 0;
	struct Area *area = 0;
	int size = 0;
	
	int no7 = 0;
	unsigned int offset = 0;

	strncpy(prefix, dn, 2);
	strncpy(pos, dn+2, 5);
	index = atoi(pos);
	no7 = *(dn+7) - '0';

	size = sizeof(struct Area);
	area = (struct Area*) malloc(size);
	memset(area, 0, size);

	if (strcmp(prefix, "13") == 0) {
		fp = fopen(HLR_13X_PATH, "rb");
	}
	else if (strcmp(prefix, "14") == 0) {
		fp = fopen(HLR_14X_PATH, "rb");
	}
	else if (strcmp(prefix, "15") == 0) {
		fp = fopen(HLR_15X_PATH, "rb");
	}
	else if (strcmp(prefix, "18") == 0) {
		fp = fopen(HLR_18X_PATH, "rb");
	}

	ret = parse_area_code(area, index*6, no7, fp);
	if (area->desc >=1 && area->desc <=5) {
		strncpy(area_code, area->shlr, area->desc);
	}

	fclose(fp);
	free(area);

	return ret;
}

// "13000000000" ,/* Failed */
// "13000999999" ,/* Failed */
// "13001000000" ,/* 10 */
// "13909081234" ,/* 830 */
// "13909095678" ,/* 831 */
// "13010109012" ,/* Failed */
// "13010119999" ,/* 10 */
// "13010158787" ,/* Failed */
// "13010178888" ,/* 531 */
// "13010189791" ,/* 311 */
// "13430000000" ,/* 663 */
// "13490000000" ,/* 10 */
// "13490999999" ,/* 10 */
// "13489999999" ,/* 591 */
// "13999999999" ,/* 991 */

// "13500920000" ,/* 433 */
// "13500921999" ,/* 433 */
// "13500922011" ,/* 433 */
// "13000199999" ,/* Failed */
// "13909723333" ,/* 972 */
// "13909724999" ,/* 971 */

const char* dns[] = {
	"13010421999",
	"13010422999",
	"13010423999",
	"13010424999",
	"13010425999",
	"13010426999",
	"13010427999",
	"13010428999",
	"13010429999",

	"13000000000" ,/* Failed */
	"13000999999" ,/* Failed */
	"13001000000" ,/* 10 */
	"13909081234" ,/* 830 */
	"13909095678" ,/* 831 */
	"13010109012" ,/* Failed */
	"13010119999" ,/* 10 */
	"13010158787" ,/* Failed */
	"13010178888" ,/* 531 */
	"13010189791" ,/* 311 */
	"13430000000" ,/* 663 */
	"13490000000" ,/* 10 */
	"13490999999" ,/* 10 */
	"13489999999" ,/* 591 */
	"13999999999" ,/* 991 */

	"13500920000" ,/* 433 */
	"13500921999" ,/* 433 */
	"13500922011" ,/* 433 */
	"13000199999" ,/* Failed */
	"13909723333" ,/* 972 */
	"13909724999" ,/* 971 */

	"13331107322", 
	"13581877736",
	"13581512085",

	"15095916313",
	"15011136419",
	"15305518685",
	"15801345017",
	"15870780763",
	"15855107795",

	"18610107039", /* 联通 */
	"18600049977",
	"18612706120",

	"18089436108",
	"18993987281", /* 电信 */
	"18910888200",

	"13818437469",
};

int areacode_array_test_main() {
	char areacode[6] = {0};
	int ret = 0;
	int count = 0;
	int i=0;

	count = sizeof(dns)/sizeof(const char*);
	for (i=0; i<count; i++) {
		memset(areacode, 0, 6);
		ret = get_area_code_mobile(dns[i], areacode, 6);
		printf("dn:%s, areacode:%s, ret:%s\n", dns[i], areacode, ((ret == -1) ? "Failed" : ""));
	}
}

/**
 * 返回的号码是1301026
 */
int read_first_long_number() {
	FILE* fp=0;
	struct Area *area = 0;
	int size_t = 0;
	int no = 0;

	size_t = sizeof(struct Area);
	area = (struct Area*) malloc(size_t);
	memset(area, 0, size_t);

	fp = fopen(HLR_13X_PATH, "rb");
	while(1) {
		fread(area, size_t, 1, fp);
		if (area->desc == 0xFF) {
			printf("no:%d\n", no);
		}
		no ++;
	}
	fclose(fp);

	free(area);
}

int get_area_city(const char* areacode, char* city, int size) {
	char substr[10] = {0};
	const char *p = 0;
	const char *end = 0;
	int sublen = 0, copylen = 0;

	strcat(substr, "|");
	strcat(substr, areacode);
	strcat(substr, ",");
	
	p = strstr(_area_cities, substr);
	if (0 != p) {
		end = strchr(p+1, '|');
		if (0 != end) {
			// copylen = strlen(p)-strlen(end)-strlen(substr);
			sublen = strlen(substr);
			for (;;) {
				++ copylen;
				if (end == p + sublen + copylen) {
					break;
				}
			}
			strncpy(city, p+sublen, copylen);
		}
	}
}

int get_mobile_provider(const char* dn, char* provider, int size) {
	char prefix[4] = {0};
	char substr[10] = {0};
	const char* p = 0;
	const char* end = 0;
	int sublen = 0, copylen = 0;

	strncpy(prefix, dn, 3);

	strcat(substr, "|");
	strcat(substr, prefix);
	strcat(substr, ",");

	p = strstr(_mobile_providers, substr);
	if (0 != p) {
		end = strchr(p+1, '|');
		if (0 != end) {
			sublen = strlen(substr);
			for (;;) {
				++ copylen;
				if (end == p + sublen + copylen)  {
					break;
				}
			}
			strncpy(provider, p+sublen, copylen);
		}
	}
}

int areacode_test_main(int argc, char* argv[]) {
	char areacode[6] = {0};
	char city[64] = {0};
	char provider[10] = {0};
	int ret = 0;
	int count = 0;
	int i=0;

	printf("[%s====>]\n", "AREACODE_TEST");

	/* read_first_long_number(); */

	/* ret = get_area_code_mobile(dn, areacode, 6); 
	printf("dn:%s, areacode:%s, ret:%s\n", dn, areacode, ((ret == -1) ? "Failed" : "")); */

	count = sizeof(dns)/sizeof(const char*);
	for (i=0; i<count; i++) {
		memset(areacode, 0, 6);
		memset(city, 0, 64);
		memset(provider, 0, 10);
		ret = get_area_code_mobile(dns[i], areacode, 6);
		if (ret == 0) {
			get_area_city(areacode, city, 64);
		}
		get_mobile_provider(dns[i], provider, 10);
		printf("dn:%s, areacode:%s, city:%s, provider:%s, ret:%s\n", dns[i], areacode, city, provider, ((ret == -1) ? "Failed" : ""));
	}
	
	return 0;	
}
