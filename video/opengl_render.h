//
// opengl_render.h
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2011 Jack (jack.wgm@gmail.com)
//

#ifndef __OPENGL_RENDER_H__
#define __OPENGL_RENDER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "video_render.h"

class opengl_render
	: public video_render
{
public:
	opengl_render();
	virtual ~opengl_render();

public:
	// ��ʼrender.
	virtual bool init_render(void* ctx, int w, int h, int pix_fmt);

	// ��Ⱦһ֡.
	virtual bool render_one_frame(AVFrame* data, int pix_fmt);

	// ������С.
	virtual void re_size(int width, int height);

	// ���ÿ�߱�.
	virtual void aspect_ratio(int srcw, int srch, bool enable_aspect);

	// ����render.
	virtual void destory_render();

private:
	bool InitGL(GLvoid);
	GLvoid KillGLWindow(GLvoid);

private:
	// ������ɫ��������.
	HGLRC m_hglrc;

	// ���洰�ھ��.
	HWND m_hwnd;

	// OpenGL��Ⱦ��������.
	HDC m_hdc;

	// ת����ɫYUV��RGB��������ָ��.
	SwsContext* m_swsctx;

	// ��ǰ֡��RGB����.
	uint8_t* m_framebuffer;

	// ��ǰ��Ⱦ����.
	GLuint m_texture;

	// ��Ƶ��.
	int m_image_width;

	// ��Ƶ��.
	int m_image_height;

	// �Ƿ����ÿ�߱�.
	bool m_keep_aspect;

	// ��߱�.
	float m_window_aspect;

	// ���λ�ò���.
	RECT m_last_rect_client;

	// ��ǰ��.
	int m_current_width;

	// ��ǰ��.
	int m_current_height;
};

#endif // __OPENGL_RENDER_H__
