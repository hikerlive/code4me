#include <stdlib.h>
#include "jnitool.h"
#include "jnilog.h"

int jniRegisterNativeMethods(JNIEnv* env,
		const char* className,
		const JNINativeMethod* gMethods,
		int numMethods)
{
	jclass clazz;

	LOGI("Register natives\n", className);
	clazz = env->FindClass(className);
	if (NULL == clazz) {
		LOGI("Native registration unable to find class '%s'\n", className);
		return -1;
	}

	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
		LOGI("RegisterNatives failed for '%s'\n", className);
		return -1;
	}

	return 0;
}

int jniThrowException(JNIEnv* env,
		const char* className,
		const char* msg)
{
	jclass clazz;

	clazz = env->FindClass(className);
	if (NULL == clazz) {
		LOGI("Unable to find exception class '%s'", className);
		return -1;
	}
	
	if (env->ThrowNew(clazz, msg) != JNI_OK) {
		LOGI("Failed throwing '%s' '%s'", className, msg);
	}
	
	return 0;
}
