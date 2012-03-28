//
// ddraw_render.h
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2011 Jack (jack.wgm@gmail.com)
//

#ifndef __DDRAW_RENDER_H__
#define __DDRAW_RENDER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "video_render.h"

typedef struct directx_fourcc
{
	char*  img_format_name;      // human readable name
	uint32_t img_format;         // as MPlayer image format
	uint32_t drv_caps;           // what hw supports with this format
	DDPIXELFORMAT ddpf_overlay;  // as Directx Sourface description
} directx_fourcc;

extern directx_fourcc dx_fourcc[];
#define DDPF_NUM_FORMATS (sizeof(dx_fourcc) / sizeof(dx_fourcc[0]))

class ddraw_render
	: public video_render
{
public:
	ddraw_render();
	virtual ~ddraw_render();

public:
	// ��ʼrender.
	virtual bool init_render(void* ctx, int w, int h, int pix_fmt);

	// ��Ⱦһ֡.
	virtual bool render_one_frame(AVFrame* data, int pix_fmt);

	// ������С.
	virtual void re_size(int width, int height);

	// ���ÿ�߱�.
	virtual void aspect_ratio(int srcw, int srch, bool enable_aspect);

	// �Ƿ�ʹ��overlayģʽ��Ⱦ.
	virtual bool use_overlay() { return m_support_double_buffer; }

	// ����render.
	virtual void destory_render();

private:
	void check_overlay();
	bool create_overlay_face(uint32_t fmt, bool must_overlay);
	bool manage_display();
	void* copy_yuv_pic(void * dst, const void * src, int bytesPerLine, int height, int dstStride, int srcStride);
	bool draw_slice(uint8_t* dst, uint8_t* src[], int stride[], int dstStride, int w, int h, int x, int y);

private:
	// ddrawָ��.
	LPDIRECTDRAW7 m_ddraw;

	// �����.
	LPDIRECTDRAWSURFACE7 m_main_face;

	// �������.
	LPDIRECTDRAWSURFACE7 m_back_face;

	// overlay���.
	LPDIRECTDRAWSURFACE7 m_overlay_face;

	// �ü���.
	LPDIRECTDRAWCLIPPER m_clipper;

	// DDRAW�Ƿ�֧��overlay.
	bool m_support_overlay;

	// DDRAW�Ƿ�֧��doublebuffer.
	bool m_support_double_buffer;

	// DDRAW�Ƿ�֧��fourcc.
	bool m_can_blit_fourcc;

	// colorkey for our surface.
	DWORD m_destcolorkey;

	// windowcolor == colorkey
	DWORD m_windowcolor;

	// ת����ɫ������ָ��.
	SwsContext* m_swsctx;

	// ת����ɫbuffer.
	uint8_t* m_swsbuffer;

	// ��ǰ������ظ�ʽ.
	DWORD m_pix_fmt;

	// ��Ƶ�������ظ�ʽ.
	int m_video_fmt;

	// ��ǰ��ʾ��������.
	RECT m_monitor_rect;

	// ��ǰ����.
	RECT m_last_rect;

	// ���ֿ��.
	bool m_keep_aspect;

	// ��߱�.
	float m_window_aspect;

	// ��.
	int m_image_width;

	// ��.
	int m_image_height;

	// ���ھ��.
	HWND m_hWnd;
};

#endif // __DDRAW_RENDER_H__

