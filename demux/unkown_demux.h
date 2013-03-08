//
// unkown_demux.h
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2011 Jack (jack.wgm@gmail.com)
//

#ifndef __UNKOWN_DEMUX_H__
#define __UNKOWN_DEMUX_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "demuxer.h"
#include "globals.h"

struct unkown_demux_data
{
	std::string file_name;				// �ļ���.
	source_context **source_ctx;		// ָ���ⲿ�����source_contextָ��, �ڲ��ɲ���, �����ⲿ�ͷ�.
};

class unkown_demux : public demuxer
{
public:
	unkown_demux(void);
	virtual ~unkown_demux(void);

public:
	// ��demuxer, ����Ϊany, �Դ����������.
	virtual bool open(boost::any ctx);

	// ��ȡһ��packet��pkt��.
	// ����true��ʾ�ɹ�.
	virtual bool read_packet(AVPacket *pkt);

	// seek_packet ������seek��ָ����timestampλ��.
	// timestamp ʱ���.
	virtual bool seek_packet(int64_t timestamp);

	// stream_index �õ�ָ��AVMediaType���͵�index.
	// index �Ƿ��ص�ָ��AVMediaType���͵�index.
	// ����true��ʾ�ɹ�.
	virtual bool stream_index(enum AVMediaType type, int &index);

	// query_avcodec_id ��ѯָ��index��codec��idֵ.
	// ָ����index.
	// ָ����index��codec_id.
	// �ɹ�����true.
	virtual bool query_avcodec_id(int index, enum AVCodecID &codec_id);

	// ��ȡ��ͣ, ��ҪΪRTSP��������ý��Э��.
	virtual int read_pause();

	// ͬ��, �ָ�����.
	virtual int read_play();

	// �ر�.
	virtual void close();

	// �Ƿ���ֹ.
	inline bool is_abort() { return m_abort; }

	// �����Ƶ�Ļ�����Ϣ.
	media_base_info base_info();

protected:

	// ��ֹ����ص�.
	static int decode_interrupt_cb(void *ctx);

	// ��source�ж�ȡ����.
	static int read_data(void *opaque, uint8_t *buf, int buf_size);

	// ��source��д������.
	static int write_data(void *opaque, uint8_t *buf, int buf_size);

	// ��source�н���seek.
	static int64_t seek_data(void *opaque, int64_t offset, int whence);

	// ��ѯָ����index.
	int query_index(enum AVMediaType type, AVFormatContext *ctx);

	// ����AVRational.
	inline void avrational_copy(AVRational &src, AVRational &dst)
	{
		dst.den = src.den;
		dst.num = src.num;
	}


protected:
	// ������Ϣ.
	unkown_demux_data m_unkown_demux_data;

	// ʹ��FFmpeg��AVFormatContext����ȡAVPacket.
	AVFormatContext *m_format_ctx;

	// ����IO������ָ��.
	AVIOContext *m_avio_ctx;

	// source_contextָ��.
	source_context *m_source_ctx;

	// ��Ƶ�Ļ�����Ϣ.
	media_base_info m_base_info;

	// ���ݻ���.
	unsigned char *m_io_buffer;

	// �Ƿ���ֹ.
	bool m_abort;
};

#endif // __UNKOWN_DEMUX_H__
