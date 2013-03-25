/**
 * @author gonggai
 */

#include <stdio.h>
#include <string.h>

int city_test_main(int argc, char* argv[]) {
	FILE *fp;
	FILE *fo;
	char line[64] = {0};
	char *p = 0;
	char *k = 0;
	int len = 0;

	fo = fopen("D:\\develop\\vcsamples\\areacode\\2.txt", "w");

	fp = fopen("D:\\develop\\vcsamples\\areacode\\1.txt", "r");
	while (1) {
		memset(line, 0, 64);
		p = fgets(line, 64, fp);
		if (p == 0) {
			break;
		} else {
			len = strlen(line);
			k = strchr(line, '\n');
			if (k != 0) {
				*k = '|';
			}
			// line[len] = '|';
			fwrite(line, len, 1, fo);
		}
	}
	fclose(fp);
	fclose(fo);


	return 0;
}