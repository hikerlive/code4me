/**
  * hellojni.c
  */

#include <string.h>
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "libhellojni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

jstring
Java_com_example_appuidroid_JniCallerActivity_stringFromJNI(JNIEnv* env,
		jobject thiz) {

	jstring jstr;
	jclass clazz;
	jmethodID method;
	jmethodID method2;
	jobject result;
	const char *str;

	jstr = (*env)->NewStringUTF(env, "The string comes from jni.");
	clazz = (*env)->FindClass(env, "com/example/appuidroid/JniCallerActivity");
	if (clazz == 0) {
		LOGE("find class failed");
		return NULL;
	}

	method = (*env)->GetMethodID(env, clazz, "messageMe", "(Ljava/lang/String;)Ljava/lang/String;");
	if (method == 0) {
		LOGE("find method failed");
		return NULL;
	}
	result = (*env)->CallObjectMethod(env, thiz, method, jstr);
	if (result == 0) {
		LOGE("call method, but result is empty.");
		return NULL;
	}

	method2 = (*env)->GetMethodID(env, clazz, "messageMe2", "(Ljava/lang/String;)V");
	if (method2 == 0) {
		LOGE("find method2 failed!!!");
		return NULL;
	}
	(*env)->CallVoidMethod(env, thiz, method2, jstr);

	str = (*env)->GetStringUTFChars(env, (jstring)result, NULL);
	LOGI("call method result:%s\n", str);

	return (*env)->NewStringUTF(env, str);

	/**
	return (*env)->NewStringUTF(env, "hello world");
	*/
}
