//
// avplayer_impl.h
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2011 Jack (jack.wgm@gmail.com)
//

#ifndef __AVPLAYER_IMPL_H__
#define __AVPLAYER_IMPL_H__

#include "avplay.h"
#include "audio_out.h"
#include "video_out.h"
#include "source.h"

#pragma once

class avplayer_impl
{
public:
	avplayer_impl(void);
	~avplayer_impl(void);

public:
	// ��������, Ҳ����ʹ��subclasswindow���ӵ�һ��ָ���Ĵ���.
	HWND create_window(LPCTSTR player_name);

	// ���ٴ���, ֻ�ܳ�������create_window�����Ĵ���.
	BOOL destory_window();

	// ���໯һ�����ڵĴ���, in_process������ʾ�����Ƿ���ͬһ������.
	BOOL subclasswindow(HWND hwnd, BOOL in_process);

public:
	// ��һ��ý���ļ�, movie���ļ���, media_type������MEDIA_TYPE_FILE,
	// Ҳ������MEDIA_TYPE_BT, ע��, �������ֻ���ļ�, ����������.
	// ���´��ļ�ǰ, ����ر�֮ǰ��ý���ļ�, ������ܲ����ڴ�й©!
	// ����, �ڲ���ǰ, avplayer����ӵ��һ������.
	BOOL open(LPCTSTR movie, int media_type, int video_out_type = 0);

	// ��������Ϊindex���ļ�, index��ʾ�ڲ����б��е�
	// λ�ü���, ��0��ʼ����, index��Ҫ���ڲ��Ŷ��ļ���bt
	// �ļ�, �����ļ����ſ���ʹ��ֱ��Ĭ��Ϊ0������Ҫ��д
	// ����.
	BOOL play(int index = 0);

	// ��ͣ����.
	BOOL pause();

	// ��������.
	BOOL resume();

	// ֹͣ����.
	BOOL stop();

	// �ر�ý��, ����򿪵���һ��bt�ļ�, ��ô
	// �����bt�ļ��е�������Ƶ�ļ������ر�.
	BOOL close();

	// seek��ĳ��ʱ�䲥��, ��λ��.
	void seek_to(double sec);

	// ȫ���л�.
	BOOL full_screen(BOOL fullscreen);

	// ���ص�ǰ����ʱ��.
	double curr_play_time();

	// ��ǰ������Ƶ��ʱ��, ��λ��.
	double duration();

	// ��ǰ������Ƶ�ĸ�, ��λ����.
	int video_width();

	// ��ǰ������Ƶ�Ŀ�, ��λ����.
	int video_height();

	// ���ص�ǰ�����б�, key��Ӧ���Ǵ򿪵�ý���ļ���.
	// value�Ǵ򿪵�ý���ļ��µ���Ƶ�ļ�.
	// ����˵��һ��bt�����ļ���Ϊavtest.torrent, ����
	// ��avtest.torrent�����������2����Ƶ�ļ�, ����Ϊ:
	// av1.mp4, av2.mp4, ��ô����б�Ӧ��Ϊ:
	// avtest.torrent->av1.mp4
	// avtest.torrent->av2.mp4
	std::map<std::string, std::string>& play_list();

	// ���ص�ǰ���ھ��.
	HWND GetWnd();

private:
	// ���ڻ������.
	void fill_rectange(HWND hWnd, HDC hdc, RECT win_rect, RECT client_rect);
	void win_paint(HWND hwnd, HDC hdc);

private:
	// ���ڹ���.
	static LRESULT CALLBACK static_win_wnd_proc(HWND, UINT, WPARAM, LPARAM);
	LRESULT win_wnd_proc(HWND, UINT, WPARAM, LPARAM);

	// ��������صĺ���.
	void init_file_source(media_source *ms);
	void init_torrent_source(media_source *ms);
	void init_audio(audio_render *ao);
	void init_video(video_render *vo);

private:
	// window���.
	HWND m_hwnd;
	HINSTANCE m_hinstance;
	HBRUSH m_brbackground;
	WNDPROC m_old_win_proc;

	// ���������.
	avplay *m_avplay;
	audio_render *m_audio;
	video_render *m_video;
	media_source *m_source;

	// ý���ļ���Ϣ.
	std::map<std::string, std::string> m_media_list;
	int m_cur_index;

	// ��Ƶ���.
	int m_video_width;
	int m_video_height;

	// ȫ��ѡ��.
	BOOL m_full_screen;
	DWORD m_wnd_style;
};

#endif // __AVPLAYER_IMPL_H__
