//
// avplayer.h
// ~~~~~~~~~~
//
// Copyright (c) 2011 Jack (jack.wgm@gmail.com)
//

#ifndef __AVPLAYER_H__
#define __AVPLAYER_H__

#ifdef API_EXPORTS
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

#pragma once

// ��ý������.
#define MEDIA_TYPE_FILE	0
#define MEDIA_TYPE_BT	1

class avplayer_impl;
// avplayer��װ��.
class EXPORT_API avplayer
{
public:
	avplayer(void);
	~avplayer(void);

public:
	// ��������, Ҳ����ʹ��subclasswindow���ӵ�һ��ָ���Ĵ���.
	HWND create_window(LPCTSTR player_name);

	// ���ٴ���, ֻ�ܳ�������create_window�����Ĵ���.
	BOOL destory_window();

	// ���໯һ�����ڵĴ���, in_process������ʾ�����Ƿ���ͬһ������.
	BOOL subclasswindow(HWND hwnd, BOOL in_process = TRUE);

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

	// ���ص�ǰ�����б��е�ý���ļ���.
	int media_count();

	// ���ز����б�indexλ�õ�ý���ļ���.
	// ����nameӦ�����ⲿ�����ڴ�, ͨ��size������������
	// �ڴ��С. �ɹ�����0, ����-1��ʾʧ��, ���ش���0��ʾ
	// name������ڴ治��, ����ֵΪindex��Ӧ���ļ�������.
	int query_media_name(int index, char *name, int size);

	// ���ص�ǰ���ھ��.
	HWND get_wnd();

private:
	avplayer_impl *m_impl;
};

#endif // __AVPLAYER_H__

