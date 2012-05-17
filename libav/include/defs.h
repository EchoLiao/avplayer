//
// defs.h
// ~~~~~~
//
// Copyright (c) 2012 Jack (jack.wgm@gmail.com)
//

#ifndef __DEFS_H__
#define __DEFS_H__

#include "libavcodec/avcodec.h"

/* ý������Դ�ӿ�. */
#define MEDIA_TYPE_FILE	0
#define MEDIA_TYPE_BT	1
#define MEDIA_TYPE_HTTP 2
#define MEDIA_TYPE_RTSP 3

/* ý���ļ���Ϣ. */
typedef struct media_info 
{
	char *name;			// �ļ���.
	int64_t start_pos;	// ��ʼλ��.
	int64_t file_size;	// �ļ���С.
} media_info;

/* ý������Դ�ӿ�. */
typedef struct source_context
{
	int (*init_source)(void *ctx);
	/*
	 * name �����Ƶ����, ��Ҫ�����߷����ڴ�.
	 * pos  �����ѯ�����, ��0��ʼ; ���Ϊ��Ƶ��bt�����е���ʼλ��ƫ��.
	 * size ����name����������, �����Ƶ��С.
	 * ����torrent�е���Ƶý�����, ����-1��ʾ����.
	 */
	int (*bt_media_info)(void *ctx, char *name, int64_t *pos, int64_t *size);
	int (*read_data)(void *ctx, char* buff, int64_t offset, int buf_size);
	void (*close)(void *ctx);
	void (*destory)(void *ctx);
	/* io_dev�Ǳ����ڲ����ڷ���ʵ�����ݵĶ���ָ��. */
	void *io_dev;
	/*
	 * ��������, ����������ֵ
	 * MEDIA_TYPE_FILE��MEDIA_TYPE_BT��MEDIA_TYPE_HTTP��MEDIA_TYPE_RTSP 
	 * ˵��: ����http��rtspֱ��ʹ����ffmpeg��demux, ���Ծ������ʼ
	 * ������ĸ�����ָ��.
	 */
	int type;
	/*
	 * ���������MEDIA_TYPE_BT, ���ָ��bt��������.
	 */
	char *torrent_data;
	int torrent_len;

	/* torrent�е�ý���ļ���Ϣ, ֻ���ڴ�
	 * torrent֮��, ������ſ���������.
	 */
	media_info *media;
	/*
	 * ý���ļ���Ϣ����, ��ҪΪBT�ļ��������, ��Ϊһ��torrent�п�
	 * �ܴ��ڶ����Ƶ�ļ�.
	 */
	int media_size;
	/* ��¼��ǰ��������ƫ��, ����λ��. */
	int64_t offset;
} source_context;

/* ��Ƶ���Žṹ����. */
typedef struct vo_context
{
	int (*init_video)(void* ctx, int w, int h, int pix_fmt);
	int (*render_one_frame)(void *ctx, AVFrame* data, int pix_fmt);
	void (*re_size)(void *ctx, int width, int height);
	void (*aspect_ratio)(void *ctx, int srcw, int srch, int enable_aspect);
	int (*use_overlay)(void *ctx);
	void (*destory_video)(void *ctx);
	void *video_dev;
	void *user_data; /* for window hwnd. */
} vo_context;

/* ��Ƶ��������ṹ����. */
typedef struct ao_context 
{
	int (*init_audio)(void *ctx, uint32_t channels, uint32_t bits_per_sample,
		uint32_t sample_rate, int format);
	int (*play_audio)(void *ctx, uint8_t *data, uint32_t size);
	void (*audio_control)(void *ctx, double vol);
	void (*destory_audio)(void *ctx);
	void *audio_dev;
} ao_context;

#endif // __DEFS_H__
