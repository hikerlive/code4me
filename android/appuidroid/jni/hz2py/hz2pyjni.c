/**
 * @author gonggai
 */

#include <string.h>
#include <stdio.h>
#include <jni.h>
#include "log.h"

JNIEXPORT jstring JNICALL
Java_com_channelsoft_baikudialer_utils_Hz2PyUtil_hz2py(JNIEnv* env,
		jobject thiz, jstring jstr) {
	const char *str;
	char overage_buff[7] = {0};
	char inbuf[1024] = {0};
	char outbuf[4096] = {0};

	int add_blank = 1;
	int polyphone_support = 1;
	int first_letter_only = 0;
	int convert_double_char = 0;

	int i = 0;
	int pinyin_len = 0;

	str = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
	if (str == NULL) {
		return NULL; /* invalid paramter */
	}

	strcpy(inbuf, str);
	(*env)->ReleaseStringUTFChars(env, jstr, str);

	if ( !is_utf8_string(inbuf) ) {
		return NULL; /* file is not utf8 */
	}

	/* convert hanzi to pinyin */
	utf8_to_pinyin(inbuf,
			outbuf,
			first_letter_only,
			polyphone_support,
			add_blank,
			convert_double_char,
			overage_buff);

	/* pinyin lower to upper */
	pinyin_len = strlen(outbuf);
	for (i = 0; i<pinyin_len; i++) {
		outbuf[i] = toupper(outbuf[i]);
	}
	LOGI("name:%s,pinyin:%s\n", inbuf, outbuf);

	return (*env)->NewStringUTF(env, outbuf);
}

JNIEXPORT jstring JNICALL
Java_com_channelsoft_baikudialer_utils_Hz2PyUtil_getSortkey(JNIEnv* env,
		jobject thiz, jstring jstr) {
	jstring jret = NULL;

	const char *str;
	char overage_buff[7] = {0};
	char inbuf[1024] = {0};
	char outbuf[4096] = {0};
	char *sortkey3 = 0;
	int sortkey_size_t = 0;

	int add_blank = 1;
	int polyphone_support = 1;
	int first_letter_only = 0;
	int convert_double_char = 0;
	int i = 0;
	int pinyin_len = 0;

	str = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
	if (str == NULL) {
		return NULL; /* invalid paramter */
	}

	strcpy(inbuf, str);
	(*env)->ReleaseStringUTFChars(env, jstr, str);

	if ( !is_utf8_string(inbuf) ) {
		return NULL; /* file is not utf8 */
	}

	/* convert hanzi to pinyin */
	utf8_to_pinyin(inbuf,
			outbuf,
			first_letter_only,
			polyphone_support,
			add_blank,
			convert_double_char,
			overage_buff);

	pinyin_len = strlen(outbuf);
	for (i=0; i<pinyin_len; i++) {
		outbuf[i] = toupper(outbuf[i]);
	}
	/* LOGI("name:%s, pinyin:%s\n", inbuf, outbuf); */

	/* convert pinyin to sortkey3 */
	pinyin_to_sortkey3(outbuf, &sortkey3, sortkey_size_t);
	if (0 != sortkey3) {
		jret = (*env)->NewStringUTF(env, sortkey3);
		free(sortkey3);
	} else {
		LOGE("name:%s, pinyin:%s, sortkey3 makegen failed!!!\n", inbuf, outbuf);
	}

	return jret;
}

JNIEXPORT jstring JNICALL
Java_com_channelsoft_baikudialer_utils_Hz2PyUtil_getSortkeyCodes(JNIEnv*env,
		jobject thiz, jstring jstr) {
	jstring jret = NULL;

	const char *sortkey3 = 0;
	char *sortcodes = 0;
	int size_t = 0;

	sortkey3 = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
	if (sortkey3 == NULL) {
		return NULL;
	}

	/* covert sortkey3 to dial keynumbers */
	sortkey3_to_codes(sortkey3, &sortcodes, size_t);
	if (0 != sortcodes) {
		jret = (*env)->NewStringUTF(env, sortcodes);
		free(sortcodes);
	}

	(*env)->ReleaseStringUTFChars(env, jstr, sortkey3);

	return jret;
}
