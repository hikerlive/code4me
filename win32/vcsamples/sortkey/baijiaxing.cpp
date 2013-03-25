/**
 *@author gonggai
 */

#include <string>
#include <vector>
#include "strutil.h"

extern "C" int is_utf8_string(
		char *utf);

extern "C" void utf8_to_pinyin(
        char *in, 
        char *out,
        int first_letter_only, 
        int polyphone_support,
        int add_blank,
        int convert_double_char,
        char *overage_buff
        );

extern "C" int output_baijiaxing() {
	std::vector<std::string> names;
	names.push_back("ÕÔÇ®ËïÀî ÖÜÎâÖ£Íõ");
	names.push_back("·ë³ÂñÒÎÀ ½¯Éòº«Ñî");
	names.push_back("ÖìÇØÓÈĞí ºÎÂÀÊ©ÕÅ"); 
	names.push_back("¿×²ÜÑÏ»ª ½ğÎºÌÕ½ª");
	names.push_back("ÆİĞ»×ŞÓ÷ °ØË®ñ¼ÕÂ"); 
	names.push_back("ÔÆËÕÅË¸ğ ŞÉ·¶ÅíÀÉ"); 
	names.push_back("Â³Î¤²ıÂí Ãç·ï»¨·½"); 
	names.push_back("ÓáÈÎÔ¬Áø ·á±«Ê·ÌÆ"); 
	names.push_back("·ÑÁ®á¯Ñ¦ À×ºØÄßÌÀ"); 
	names.push_back("ëøÒóÂŞ±Ï ºÂÚù°²³£"); 
	names.push_back("ÀÖÓÚÊ±¸µ Æ¤±åÆë¿µ"); 
	names.push_back("ÎéÓàÔª²· ¹ËÃÏÆ½»Æ"); 
	names.push_back("ºÍÄÂÏôÒü Ò¦ÉÛÕ¿Íô"); 
	names.push_back("ÆîÃ«ÓíµÒ Ã×±´Ã÷ê°"); 
	names.push_back("¼Æ·ü³É´÷ Ì¸ËÎÃ©ÅÓ"); 
	names.push_back("ĞÜ¼ÍÊæÇü Ïî×£¶­Áº"); 
	names.push_back("¶ÅÈîÀ¶ãÉ Ï¯¼¾ÂéÇ¿"); 
	names.push_back("¼ÖÂ·Â¦Î£ ½­Í¯ÑÕ¹ù"); 
	names.push_back("Ã·Ê¢ÁÖµó ÖÓĞìÇğÂæ"); 
	names.push_back("¸ßÏÄ²ÌÌï ·®ºúÁè»ô"); 
	names.push_back("ÓİÍòÖ§¿Â êÃ¹ÜÂ¬Äª"); 
	names.push_back("¾­·¿ôÃçÑ ¸É½âÓ¦×Ú"); 
	names.push_back("¶¡ĞûêÚµË Óôµ¥º¼ºé"); 
	names.push_back("°üÖî×óÊ¯ ´Ş¼ªÅ¥¹¨"); 
	names.push_back("³ÌïúĞÏ»¬ ÅáÂ½ÈÙÎÌ"); 
	names.push_back("Ü÷Ñòì¶»İ Õçôğ¼Ò·â"); 
	names.push_back("ÜÇôà´¢½ù ¼³ÚûÃÓËÉ"); 
	names.push_back("¾®¶Î¸»Î× ÎÚ½¹°Í¹­"); 
	names.push_back("ÄÁÚóÉ½¹È ³µºîåµÅî"); 
	names.push_back("È«Û­°àÑö ÇïÖÙÒÁ¹¬"); 
	names.push_back("Äş³ğèï±© ¸Êî×À÷Ğç"); 
	names.push_back("×æÎä·ûÁõ ¾°Õ²ÊøÁú"); 
	names.push_back("Ò¶ĞÒË¾ÉØ Û¬Àè¼»±¡"); 
	names.push_back("Ó¡ËŞ°×»³ ÆÑÛ¢´Ó¶õ"); 
	names.push_back("Ë÷ÏÌ¼®Àµ ×¿İşÍÀÃÉ"); 
	names.push_back("³ØÇÇÒõÓô ñãÄÜ²ÔË«"); 
	names.push_back("ÎÅİ·µ³µÔ Ì·¹±ÀÍ·ê"); 
	names.push_back("¼§Éê·ö¶Â È½Ô×ÛªÓº"); 
	names.push_back("àSè³É£¹ğ å§Å£ÊÙÍ¨"); 
	names.push_back("±ßìèÑà¼½ Û£ÆÖÉĞÅ©"); 
	names.push_back("ÎÂ±ğ×¯êÌ ²ñöÄÑÖ³ä"); 
	names.push_back("Ä½Á¬ÈãÏ° »Â°¬ÓãÈİ"); 
	names.push_back("Ïò¹ÅÒ×É÷ ¸êÁÎâ×ÖÕ"); 
	names.push_back("ôß¾Óºâ²½ ¶¼¹¢Âúºë"); 
	names.push_back("¿ï¹úÎÄ¿Ü ¹ãÂ»ãÚ¶«"); 
	names.push_back("Å·ì¯ÎÖÀû ÎµÔ½ÙçÂ¡"); 
	names.push_back("Ê¦¹®ØÇÄô êË¹´°½ÈÚ"); 
	names.push_back("Àäö¤ĞÁãÛ ÄÇ¼òÈÄ¿Õ"); 
	names.push_back("ÔøÎãÉ³Ø¿ Ñø¾ÏĞë·á"); 
	names.push_back("³²¹ØØáÏà ²éºó¾£ºì"); 
	names.push_back("ÓÎóÃÈ¨åÖ ¸ÇÒæ»¸¹«"); 
	names.push_back("ÍòÙ¹Ë¾Âí ÉÏ¹ÙÅ·Ñô"); 
	names.push_back("ÏÄºîÖî¸ğ ÎÅÈË¶«·½"); 
	names.push_back("ºÕÁ¬»Ê¸¦ Î¾³Ù¹«Ñò"); 
	names.push_back("å£Ì¨¹«Ò± ×ÚÕşå§Ñô");
	names.push_back("´¾ÓÚµ¥ÓÚ Ì«ÊåÉêÍÀ"); 
	names.push_back("¹«ËïÖÙËï ĞùÔ¯Áîºü"); 
	names.push_back("ÖÓÀëÓîÎÄ ³¤ËïÄ½Èİ"); 
	names.push_back("Ë¾Í½Ë¾¿Õ °Ù¼ÒĞÕÖÕ");

	// names.push_back("Î¾å£ÙçêË Ì«³¤");

	for (int i=0; i<names.size(); i++) {
		std::string utf8_name = ansi2utf8(names.at(i));
		char inbuf[1024] = {0};
		strcpy(inbuf, utf8_name.c_str());

		char outbuf[4096] = {0};
		char overage_buff[7] = {0};

		int add_blank = 1;
		int polyphone_support = 1;
		int first_letter_only = 0;
		int convert_double_char = 0;

		if (!is_utf8_string(inbuf)) {
			printf("File encoding is not utf-8!\n");
			return -1;
		}

		// printf("No:%d ", i);
		// printf("No:%d name:%s", names.at(i).c_str());
		utf8_to_pinyin(inbuf,
			outbuf,
			first_letter_only,
			polyphone_support,
			add_blank,
			convert_double_char,
			overage_buff);
		printf("\n");

		printf("No:%d name:%s, pinyin:%s",i, names.at(i).c_str(), outbuf);
	}
}