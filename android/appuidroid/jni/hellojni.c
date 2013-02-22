/**
  * hellojni.c
  */

#include <string.h>
#include <jni.h>

jstring
Java_com_example_appuidroid_utils_HelloJni_stringFromJNI(JNIEnv* env,
		jobject thiz) {
	return (*env)->NewStringUTF(env, "helloworld from jni");
}
