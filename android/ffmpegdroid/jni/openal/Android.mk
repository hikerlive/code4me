# compile openal static lib

LOCAL_PATH	:= $(call my-dir)

LOCAL_MODULE := openal
LOCAL_MODULE_FILENAME := libopenal
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/include/openal32
LOCAL_SRC_FILES	 := \
	openal32/alAuxEffectSlot.c \
	openal32/alBuffer.c \
	openal32/alDatabuffer.c \
	openal32/alEffect.c \
	openal32/alError.c \
	openal32/alExtension.c \
	openal32/alFilter.c \
	openal32/alListener.c \
	openal32/alSource.c \
	openal32/alState.c \
	openal32/alThunk.c \
	alc/alc.c \
	alc/alcConfig.c \
	alc/alcEcho.c \
	alc/alcModulator.c \
	alc/alcReverb.c \
	alc/alcRing.c \
	alc/alcThread.c \
	alc/alu.c \
	alc/android.c \
	alc/bs2b.c \
	alc/null.c
	
LOCAL_CFLAGS	:= -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES
LOCAL_LDLIBS	:= -llog -Wl, -s

LOCAL_STATIC_LIBRARIES	:= libtremolo

include $(BUILD_STATIC_LIBRARY)

