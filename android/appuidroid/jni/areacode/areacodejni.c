/**
 * @author gonggai
 */

#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#define LOG_TAG "libareacode"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

jstring
Java_com_channelsoft_baikudialer_utils_AreaCodeUtil_getAreaCodeText(JNIEnv* env,
		jobject thiz, jobject assetManager, jstring jstr) {
	int ret = 0;
	const char* str;
	char areacode[10] = {0};
	char city[64] = {0};
	char provider[10] = {0};

	char total[128] = {0};
	char prefix[3] = {0};

	AAssetManager *mgr = NULL;
	AAsset* asset = NULL;

	str = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
	if (str == NULL) {
		return NULL;
	}

	if (is_mobile(str)) { /* 手机号 */
		mgr = AAssetManager_fromJava(env, assetManager);
		strncpy(prefix, str, 2);
		if (strcmp(prefix, "13") == 0) {
			asset = AAssetManager_open(mgr, "hlr.13x", AASSET_MODE_UNKNOWN);
		}
		else if (strcmp(prefix, "14") == 0) {
			asset = AAssetManager_open(mgr, "hlr.14x", AASSET_MODE_UNKNOWN);
		}
		else if (strcmp(prefix, "15") == 0) {
			asset = AAssetManager_open(mgr, "hlr.15x", AASSET_MODE_UNKNOWN);
		}
		else if (strcmp(prefix, "18") == 0) {
			asset = AAssetManager_open(mgr, "hlr.18x", AASSET_MODE_UNKNOWN);
		}

		ret = get_areacode_mobile(asset, str, areacode, 10);
		if (ret == 0) {
			get_areacode_city(areacode, city, 64);
			get_mobile_provider(str, provider, 10);

			strcpy(total, city);
			strcat(total, " ");
			strcat(total, provider);
			LOGI("number:%s, area text:%s\n", str, total);
		} else {
			LOGE("number:%s, get areacode failed!!!\n", str);
		}
		AAsset_close(asset);
	}
	else if (is_telphone(str)) { /* 固话 */
		if (*(str+1) == '1' || *(str+1) == '2') {
			strncpy(areacode, str+1, 2);
		} else {
			strncpy(areacode, str+1, 3);
		}
		get_areacode_city(areacode, city, 64);
		strcpy(total, city);
		LOGI("number:%s, area text:%s\n", str, total);
	}

	(*env)->ReleaseStringUTFChars(env, jstr, str);
	
	return (*env)->NewStringUTF(env, total);
}
