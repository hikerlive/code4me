#ifndef __FFAUDIO_CONTEXT_H
#define __FFAUDIO_CONTEXT_H

#include <libavformat/avformat.h>
#include <AL/al.h>

struct ALFFAudioDecoder;

/**
 * 音频上下文
 */
struct ALFFAudioContext {
	AVFormatContext *format_context; /* 格式上下文 */
	double total_time; /* 播放时长 */
	int stream_index; /* 流索引 */
	AVStream *stream; /* 流 */
	AVPacketList *packet_list; /* 数据包列表 */
	AVFrame *frame; /* 音频帧 */
	const uint8_t *frame_data; /* 指向音频帧数据的指针，无需单独申请或释放内存 */
	size_t frame_data_size; /* 音频帧数据的大小 */
};

/**
 * 打开输入流
 * @param url 打开流的地址
 * @param total_time 打开的流播放时长
 * @return 成功返回0, 失败返回-1
 */
int ffaudio_open_input_stream(const char* url, double *total_time);

/**
 * 关闭流
 */
void ffaudio_close_input_stream();

/**
 * 打开编码器
 * @param sample_rate 采样率
 * @param channel_type 通道类型
 * @param sample_type 采样类型
 * @return 成功返回0, 失败返回-1
 */
void ffaudio_open_codec(ALuint *sample_rate, ALenum *channel_type, ALenum *sample_type);

/**
 * 关闭编解码
 */
void ffaudio_close_codec();

/**
 * 解码
 * @param data, 解码之后的数据
 * @param length, 最大的解码长度
 * @param pts_in_seconds, 显示时间, 单位为秒
 * @return 实际的解码长度
 */
size_t ffaudio_decode(void *data, size_t length, double *pts_in_seconds);

/**
 * 查找
 * @param time, 指定查找的时间, 单位为秒
 */
void ffaudio_seek(double time);

#endif /* __FFAUDIO_CONTEXT_H */
