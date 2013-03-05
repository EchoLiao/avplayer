#pragma once

#include <iostream>
#include "ins.h"

#include "av_source.h"
#include "libyk.h"


// yk�е���Ƶ��Ϣ.
struct yk_video_file_info 
{
	int index;						// id.
	std::string title;				// ��Ƶ�ļ���.
	std::string source;				// ��ƵԴ��ַ
	uint64_t data_size;		// ��Ƶ��С.
	uint64_t base_offset;	// ��Ƶ��yk�е�ƫ��.
	int status;						// ��ǰ����״̬.
};

class yk_source
	: public av_source
{
public:
	yk_source(void);
	virtual ~yk_source(void);

public:
	// ��.
	virtual bool open(void* ctx);

	// ��ȡ����.
	virtual bool read_data(char* data, uint64_t offset, size_t size, size_t& read_size);

	// seek����, �˴�����true, ��ʾ���ݲ���, ��Ҫ����.
	virtual bool read_seek(uint64_t offset, int whence);

	// �ر�.
	virtual void close();

	// ���û��õ�ǰ���ŵ���Ƶ�ļ�.
	virtual bool set_current_video(int index);
	virtual bool get_current_video(yk_video_file_info& vfi) const;

	// ��ǰ��Ƶ�б�.
	virtual std::vector<yk_video_file_info> video_list() const;

	// ���ö�ȡ����.
	virtual void reset();

	// ����url.
	bool parse_url(const std::string &url);


private:
	libyk::youku m_yk_video;
	bool m_abort;
	bool m_reset;
};
