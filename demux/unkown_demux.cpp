#include "internal.h"
#include "unkown_demux.h"

#define IO_BUFFER_SIZE	32768

int unkown_demux::decode_interrupt_cb(void *ctx)
{
	unkown_demux *demux = (unkown_demux*)ctx;
	return (int)demux->m_abort;
	// return demux->is_abort();
}

int unkown_demux::read_data(void *opaque, uint8_t *buf, int buf_size)
{
 	int ret = 0;
//  	unkown_demux *demux = (avplay*)opaque;
//  
//  	// �ѱ���ֹ.
//  	if (demux->is_abort())
//  		return 0;
//  
//  	// ��ȡ����.
//  	ret = play->m_source_ctx->read_data(play->m_source_ctx, buf, buf_size);
//  
//  	// ��ȡʧ��, ����, �����Ϳ��Լ����������ݻ�������ǰ�沥��.
//  	if (ret == -1)
//  		return 0;
 
 	return ret;
}


int unkown_demux::write_data(void *opaque, uint8_t *buf, int buf_size)
{
	unkown_demux *demux = (unkown_demux*)opaque;
	return 0;
}


int64_t unkown_demux::seek_data(void *opaque, int64_t offset, int whence)
{
 	unkown_demux *demux = (unkown_demux*)opaque;
 
//  	if (play->m_abort)
//  		return -1;
//  
//  	// �������read_seek����ʵ��, �������Ӧ�ĺ���ʵ��, ��������¼�.
//  	if (play->m_source_ctx && play->m_source_ctx->read_seek)
//  		offset = play->m_source_ctx->read_seek(play->m_source_ctx, offset, whence);
//  	else
//  		assert(0);
//  
//  	if (play->m_source_ctx->dl_info.not_enough)
//  	{
//  		// TODO: �ж��Ƿ������㹻, ��������Բ���, ����ͣ����.
//  	}
 
 	return offset;
}

unkown_demux::unkown_demux(void)
	: m_format_ctx(NULL)
	, m_avio_ctx(NULL)
	, m_source_ctx(NULL)
	, m_io_buffer(NULL)
	, m_abort(false)
{}

unkown_demux::~unkown_demux(void)
{}

bool unkown_demux::open(boost::any ctx)
{
	// �õ�����Ĳ���.
	m_unkown_demux_data = boost::any_cast<unkown_demux_data>(ctx);

	// ����m_format_ctx.
	m_format_ctx = avformat_alloc_context();
	if (!m_format_ctx)
		goto FAILED_FLG;

	// ���ò���.
	m_format_ctx->flags = AVFMT_FLAG_GENPTS;
	m_format_ctx->interrupt_callback.callback = decode_interrupt_cb;
	m_format_ctx->interrupt_callback.opaque = (void*)this;

	// ����ָ��.
	m_source_ctx = *m_unkown_demux_data.source_ctx;

	// ��ʼ������Դ.
	if (m_source_ctx)
	{
		// �����ʼ��ʧ��.
		if (m_source_ctx->init_source(m_source_ctx) < 0)
			goto FAILED_FLG;
	}

	int ret = 0;
	AVInputFormat *iformat = NULL;

	// ֧��BT�ͱ����ļ�.
	if (m_source_ctx->type == MEDIA_TYPE_BT || m_source_ctx->type == MEDIA_TYPE_FILE)
	{
		m_io_buffer = (unsigned char*)malloc(IO_BUFFER_SIZE);
		if (!m_io_buffer)
		{
			std::cerr << "Create buffer failed!\n";
			goto FAILED_FLG;
		}

		// ����io������.
		m_avio_ctx = avio_alloc_context(m_io_buffer,
			IO_BUFFER_SIZE, 0, (void*)this, read_data, NULL, seek_data);
		if (!m_avio_ctx)
		{
			std::cerr << "Create io context failed!\n";
			goto FAILED_FLG;
		}
		m_avio_ctx->write_flag = 0;

		ret = av_probe_input_buffer(m_avio_ctx, &iformat, "", NULL, 0, 0);
		if (ret < 0)
		{
			std::cerr << "av_probe_input_buffer call failed!\n";
			goto FAILED_FLG;
		}

		// ������ý����.
		m_format_ctx->pb = m_avio_ctx;
		ret = avformat_open_input(&m_format_ctx, "", iformat, NULL);
		if (ret < 0)
		{
			std::cerr << "av_open_input_stream call failed!\n";
			goto FAILED_FLG;
		}
	}
	else
	{
		// �õ���Ӧ��url.
		char url[MAX_URI_PATH];
		if (m_source_ctx->type == MEDIA_TYPE_HTTP)
			strcpy(url, m_source_ctx->info.http.url);
		else if (m_source_ctx->type == MEDIA_TYPE_RTSP)
			strcpy(url, m_source_ctx->info.rtsp.url);
		else
			goto FAILED_FLG;

		// �մ�, ��������.
		if (strlen(url) == 0)
			goto FAILED_FLG;

		/* HTTP��RTSPֱ��ʹ��ffmpeg������.	*/
		int ret = avformat_open_input(&m_format_ctx, url, iformat, NULL);
		if (ret < 0)
		{
			std::cerr << "av_open_input_stream call failed!\n";
			goto FAILED_FLG;
		}
	}

	ret = avformat_find_stream_info(m_format_ctx, NULL);
	if (ret < 0)
		goto FAILED_FLG;

#ifdef _DEBUG
	av_dump_format(m_format_ctx, 0, NULL, 0);
#endif // _DEBUG

	// �ÿ�.
	memset(&m_base_info, 0, sizeof(m_base_info));

	// ���һЩ������Ϣ.
	m_base_info.has_video = query_index(AVMEDIA_TYPE_VIDEO, m_format_ctx);
	m_base_info.has_audio = query_index(AVMEDIA_TYPE_AUDIO, m_format_ctx);

	// �������Ƶ, �����Ƶ��һЩ������Ϣ.
	if (m_base_info.has_audio >= 0)
	{
		avrational_copy(m_format_ctx->streams[m_base_info.has_audio]->r_frame_rate, m_base_info.audio_frame_rate);
		avrational_copy(m_format_ctx->streams[m_base_info.has_audio]->time_base, m_base_info.audio_time_base);
		m_base_info.audio_start_time = m_format_ctx->streams[m_base_info.has_audio]->start_time;
	}

	// �������Ƶ, �����Ƶ��һЩ������Ϣ.
	if (m_base_info.has_video >= 0)
	{
		avrational_copy(m_format_ctx->streams[m_base_info.has_video]->r_frame_rate, m_base_info.video_frame_rate);
		avrational_copy(m_format_ctx->streams[m_base_info.has_video]->time_base, m_base_info.video_time_base);
		m_base_info.video_start_time = m_format_ctx->streams[m_base_info.has_video]->start_time;
	}

FAILED_FLG:
	if (m_format_ctx)
		avformat_close_input(&m_format_ctx);
	if (m_avio_ctx)
		av_free(m_avio_ctx);
	if (m_io_buffer)
		av_free(m_io_buffer);
	return false;
}

bool unkown_demux::read_packet(AVPacket *pkt)
{
	return false;
}

bool unkown_demux::seek_packet(int64_t timestamp)
{
	return false;
}

bool unkown_demux::stream_index(enum AVMediaType type, int &index)
{
	return false;
}

bool unkown_demux::query_avcodec_id(int index, enum AVCodecID &codec_id)
{
	return false;
}

void unkown_demux::close()
{
}

int unkown_demux::read_pause()
{
	return -1;
}

int unkown_demux::read_play()
{
	return -1;
}

int unkown_demux::query_index(enum AVMediaType type, AVFormatContext *ctx)
{
	unsigned int i;

	for (i = 0; (unsigned int) i < ctx->nb_streams; i++)
		if (ctx->streams[i]->codec->codec_type == type)
			return i;
	return -1;
}

media_base_info unkown_demux::base_info()
{
	return m_base_info;
}
