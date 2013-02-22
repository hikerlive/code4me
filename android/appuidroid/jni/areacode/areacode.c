/**
 * @author gonggai
 */

#include "areacode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

struct Area{
	unsigned char desc;
	char shlr[5];
};

int parse_area_code(AAsset* asset, struct Area* area, long offset, int no7) {
	int ret = 0;
	unsigned int index = 0;

	AAsset_seek(asset, offset, SEEK_SET);
	AAsset_read(asset, area, sizeof(struct Area));
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
		 *   c.是index*6+100060文件读取位置，不对。
		 */
		memcpy(&index, area->shlr, 4);
		ret = parse_area_code(asset, area, index*6+no7*6, no7);
	}
	else if (area->desc >= 1 && area->desc <= 5) {
		ret = 0;
	}
	return ret;
}

int get_areacode_mobile(AAsset* asset, const char* dn, char* areacode, int size_t) {
	int ret = 0;
	char path[256] = {0};
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

	ret = parse_area_code(asset, area, index*6, no7);
	if (area->desc >=1 && area->desc <=5) {
		strncpy(areacode, area->shlr, area->desc);
	}

	free(area);

	return ret;
}

int get_areacode_city(const char* areacode, char* city, int size) {
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

int is_mobile(const char* str) {
	int len = 0;

	len = strlen(str);
	if (len > 0 && len == 11) {
		if ('1' == *str) {
			return 1;
		}
	}
	return 0;
}

int is_telphone(const char* str) {
	int len = 0;

	len = strlen(str);
	if (len >= 7) {
		if ('0' == *str) {
			return 1;
		}
	}
	return 0;
}
