# compile areacode.c

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= areacode
LOCAL_SRC_FILES	:= areacode.c areacodejni.c
LOCAL_LDLIBS	:= -llog -landroid

include $(BUILD_SHARED_LIBRARY)
