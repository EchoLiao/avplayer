//
// avplay.h
// ~~~~~~~~
//
// Copyright (c) 2011 Jack (jack.wgm@gmail.com)
//

#ifndef AVPLAY_H_
#define AVPLAY_H_

#ifdef _MSC_VER
#include <windows.h>
#define inline
#define __CRT__NO_INLINE
#ifdef API_EXPORTS
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif
#else
#define EXPORT_API
#endif

#include "pthread.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavcodec/audioconvert.h"
#include <assert.h>

#ifdef  __cplusplus
extern "C" {
#endif

/* ������״̬. */
typedef enum play_status
{
	inited, playing, paused, stoped
} play_status;

/* ����config_render������ʾ�����õ�render.  */
#define MEDIA_SOURCE			0
#define AUDIO_RENDER			1
#define VIDEO_RENDER			2

/* ���ڱ�ʶ��Ⱦ������. */
#define VIDEO_RENDER_D3D		0
#define VIDEO_RENDER_DDRAW		1
#define VIDEO_RENDER_OPENGL	2
#define VIDEO_RENDER_SOFT		3

/* ����.	*/
typedef struct av_queue
{
	void *m_first_pkt, *m_last_pkt;
	int m_size; /* ���д�С.	*/
	int m_type; /* ��������.	*/
	int abort_request;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
} av_queue;

/* ý������Դ�ӿ�. */
#define MEDIA_TYPE_FILE	0
#define MEDIA_TYPE_BT	1

/* ý���ļ���Ϣ. */
typedef struct media_info 
{
	char *name;			// �ļ���.
	int64_t start_pos;	// ��ʼλ��.
	int64_t file_size;	// �ļ���С.
} media_info;

/* ý������Դ�ӿ�. */
typedef struct media_source
{
	int (*init_source)(void **ctx, char *data, int len, char *save_path);
	/*
	 * name �����Ƶ����, ��Ҫ�����߷����ڴ�.
	 * pos  �����ѯ�����, ��0��ʼ; ���Ϊ��Ƶ��bt�����е���ʼλ��ƫ��.
	 * size ����name����������, �����Ƶ��С.
	 * ����torrent�е���Ƶý�����, ����-1��ʾ����.
	 */
	int (*bt_media_info)(void *ctx, char *name, int64_t *pos, int64_t *size);
	int (*read_data)(void *ctx, char* buff, int64_t offset, int buf_size);
	void (*destory)(void *ctx);
	void *ctx;
	int type;			/* ��������. */
	/*
	 * ��������, ���������MEDIA_TYPE_FILE, ���ָ���ļ���, 
	 * �����MEDIA_TYPE_BT, ���ָ��bt��������.
	 */
	char *data;
	int data_len;
	/* ý���ļ���Ϣ.	*/
	media_info *media;
	/* ý���ļ���Ϣ����.	*/
	int media_size;
	/* ��ǰ��������ƫ��, ����λ��.	*/
	int64_t offset;
} media_source;

/* ����Դ�ṹ������ͷ�. */
EXPORT_API media_source* alloc_media_source(int type, char *addition, int addition_len, int64_t size);
EXPORT_API void free_media_source(media_source *src);


/* ��Ƶ���Žӿ�. */
typedef struct audio_render
{
	int (*init_audio)(void **ctx, uint32_t channels, uint32_t bits_per_sample,
		uint32_t sample_rate, int format);
	int (*play_audio)(void *ctx, uint8_t *data, uint32_t size);
	void (*audio_control)(void *ctx, int cmd, void* arg);
	void (*destory_audio)(void *ctx);
	void *ctx;
} audio_render;

/* ��Ƶ�ṹ������ͷ�. */
EXPORT_API audio_render* alloc_audio_render();
EXPORT_API void free_audio_render(audio_render *render);

/* ��Ƶ���Žӿ�. */
typedef struct video_render
{
	int (*init_video)(void **ctx, void* user_data,int w, int h, int pix_fmt);
	int (*render_one_frame)(void *ctx, AVFrame* data, int pix_fmt);
	void (*re_size)(void *ctx, int width, int height);
	void (*aspect_ratio)(void *ctx, int srcw, int srch, int enable_aspect);
	int (*use_overlay)(void *ctx);
	void (*destory_video)(void *ctx);
	void *ctx;
	void *user_data; /* for window hwnd. */
} video_render;

/* ��Ƶ��Ⱦ�ṹ������ͷ�. */
EXPORT_API video_render* alloc_video_render(void *user_data);
EXPORT_API void free_video_render(video_render *render);


typedef struct avplay
{
	/* �ļ���ָ��. */
	AVFormatContext *m_format_ctx;

	/* ����Ƶ����.	*/
	av_queue m_audio_q;
	av_queue m_video_q;
	av_queue m_audio_dq;
	av_queue m_video_dq;

	/* ��������Ⱦ�߳�.	*/
	pthread_t m_audio_dec_thrd;
	pthread_t m_video_dec_thrd;
	pthread_t m_audio_render_thrd;
	pthread_t m_video_render_thrd;
	pthread_t m_read_pkt_thrd;

	/* �ز�����Ƶָ��.	*/
	struct SwsContext *m_swsctx;
	AVAudioConvert *m_audio_convert_ctx;
	ReSampleContext *m_resample_ctx;

	/* ��Ƶ����Ƶ��AVStream��AVCodecContextָ���index.	*/
	AVCodecContext *m_audio_ctx;
	AVCodecContext *m_video_ctx;
	AVStream *m_audio_st;
	AVStream *m_video_st;
	int m_audio_index;
	int m_video_index;

	/* ��ȡ���ݰ�ռ�û����С.	*/
	long volatile m_pkt_buffer_size;
	pthread_mutex_t m_buf_size_mtx;

	/* ͬ������. */
	int m_av_sync_type;

	/*
	 * ���ڼ�����Ƶ����ʱ��
	 * ��:  m_video_current_pts_drift = m_video_current_pts - time();
	 *      m_video_current_pts�ǵ�ǰ����֡��ptsʱ��, ������pts��ǰ�ƽ�
	 *      ��ͬʱ, timeҲͬ������ǰ�ƽ�, ����pts_drift����������һ��
	 *      time_base��Χ�ڸ���.
	 * ����ʱ�� = m_video_current_pts_drift - time()
	 */
	double m_video_current_pts_drift;
	double m_video_current_pts;

	/* ���±������ڼ�������Ƶͬ��.	*/
	double m_frame_timer;
	double m_frame_last_pts;
	double m_frame_last_duration;
	double m_frame_last_delay;
	double m_frame_last_filter_delay;
	double m_frame_last_dropped_pts;
	double m_frame_last_returned_time;
	int64_t m_frame_last_dropped_pos;
	int64_t m_video_current_pos;
	int m_drop_frame_num;

	/* seekʵ��. */
	int m_read_pause_return;
	int m_seek_req;
	int m_seek_flags;
	int64_t m_seek_pos;
	int64_t m_seek_rel;
	int m_seek_by_bytes;

	/* ���һ������֡��pts, ����֡�����СΪ2, Ҳ���ǵ�ǰ����֡����һ֡.	*/
	double m_audio_clock;
	double m_video_clock;
	double m_external_clock;
	double m_external_clock_time;

	/* ��ǰ����Դ��ȡ��. */
	media_source *m_media_source;
	AVIOContext *m_avio_ctx;
	unsigned char *m_io_buffer;
	pthread_mutex_t m_source_mtx;
	/* ��ǰ��Ƶ��Ⱦ��.	*/
	audio_render *m_audio_render;
	/* ��ǰ��Ƶ��Ⱦ��. */
	video_render *m_video_render;
	/* ��ǰ��Ⱦ��������. */
	int m_video_render_type;

	/* ��ǰ��Ƶ����buffer��С.	*/
	uint32_t m_audio_buf_size;

	/* ��ǰ��Ƶ�Ѿ�����buffer��λ��.	*/
	uint32_t m_audio_buf_index;
	int32_t m_audio_write_buf_size;
	double m_audio_current_pts_drift;

	/* ����״̬. */
	play_status m_play_status;
	/* ���ڲ��ŵ�����, ֻ����BT�ļ�����. */
	int m_current_play_index;

	/* ֹͣ��־.	*/
	int m_abort;

} avplay;

/*
 * Assign a player structural context.
 * @If the function succeeds, the return value is a pointer to the avplay,
 * If the function fails, the return value is NULL.
 */
EXPORT_API avplay* alloc_avplay_context();

/*
 * The release of the structural context of the player.
 * @param ctx allocated by alloc_avplay_context.
 * @This function does not return a value.
 */
EXPORT_API void free_avplay_context(avplay *ctx);

/*
 * Initialize the player.
 * @param play pointer to user-supplied avplayer (allocated by alloc_avplay_context).
 * @param filename filename Name of the stream to open.
 * @param video_out_type expression BT or FILE.
 * @return 0 on success, a negative AVERROR on failure.
 * example:
 * avplayer* play = alloc_avplay_context();
 * int ret;
 * 
 * ret = initialize(play, "/tmp/test.avi");
 * if (ret != 0)
 *    return ret; // ERROR!
 */
EXPORT_API int initialize(avplay *play, media_source *ms, int video_out_type);

/*
 * The Configure render or source to palyer.
 * @param play pointer to the player. 
 * @param render video render or audio render.
 * @param type Specifies render type, MEDIA_SOURCE	or AUDIO_RENDER VIDEO_RENDER.
 * @This function does not return a value.
 */
EXPORT_API void configure(avplay *play, void *render, int type);

/*
 * The start action player to play. 
 * @param play pointer to the player. 
 * @param index Specifies the index of the file to play.
 * @param Returns 0 if successful, or an error value otherwise. 
 */
EXPORT_API int start(avplay *play, int index);

/*
 * Wait for playback to complete.
 * @param play pointer to the player. 
 * @This function does not return a value.
 */
EXPORT_API void wait_for_completion(avplay *play);

/*
 * The Stop function stops playing the media. 
 * @param play pointer to the player. 
 * @This function does not return a value.
 */
EXPORT_API void stop(avplay *play);

/*
 * The Pause method pauses the current player.
 * @param play pointer to the player. 
 * @This function does not return a value.
 */
EXPORT_API void pause(avplay *play);

/*
 * The Resume function starts the player from the current position, after a Pause function call. 
 * @param play pointer to the player.
 * @This function does not return a value.
 */
EXPORT_API void resume(avplay *play);

/*
 * Moves the current seek second.
 * @param play pointer to the player.
 * @param sec at time, second.
 * @This function does not return a value.
 */
EXPORT_API void seek(avplay *play, double sec);

/*
 * The current playback time position
 * @param play pointer to the player.
 * @return current play time position, a negative on failure.
 */
EXPORT_API double curr_play_time(avplay *play);

/*
 * The Duration function return the playing duration of the media, in second units.
 * @param play pointer to the player.
 * @return the playing duration of the media, in second units.
 */
EXPORT_API double duration(avplay *play);

/*
 * Destroys an player. 
 * @param play pointer to the player.
 * @This function does not return a value.
 */
EXPORT_API void destory(avplay *play);

#ifdef  __cplusplus
}
#endif

#endif /* AVPLAY_H_ */
