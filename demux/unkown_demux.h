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

struct unkown_demux_data
{
	std::string file_name;
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

	// �ر�.
	virtual void close();

};

#endif // __UNKOWN_DEMUX_H__
