# compile ffmpegplayer

LOCAL_PATH	:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE 	:= ffmpegplayer
LOCAL_LDFLAGS	:= -L$(LOCAL_PATH)/../libffmpeg/lib

LOCAL_LDLIBS 	:= -llog -lavcodec -lavformat -lavutil -lswscale
LOCAL_STATIC_LIBRARIES	:= libopenal

LOCAL_C_INCLUDES := $(LOCA_PATH)/../libffmpeg/include

LOCAL_SRC_FILES	:= jnitool.cc audio_player_manager.cc main.cc
	
include $(BUILD_SHARED_LIBRARY)