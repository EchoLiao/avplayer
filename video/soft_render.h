//
// soft_render.h
// ~~~~~~~~~~~~~
//
// Copyright (c) 2011 Jack (jack.wgm@gmail.com)
//

#ifndef __SOFT_RENDER_H__
#define __SOFT_RENDER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "video_render.h"

class soft_render
   : public video_render
{
public:
   soft_render();
   virtual ~soft_render();

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
   // ���ھ��.
   HWND m_hwnd;

   // ����dc.
   HDC m_hdc;

   // GDI+��ָ��.
   ULONG_PTR m_gdiplusToken;

   // ֡����.
   uint8_t* m_framebuffer;

   // ����ת��֡���ݸ�ʽ.
   SwsContext* m_swsctx;

   // ��Ƶ��.
   int m_image_width;

   // ��Ƶ��.
   int m_image_height;

   // �Ƿ����ÿ�߱�.
   bool m_keep_aspect;

   // ��߱�.
   float m_window_aspect;
};

#endif // __SOFT_RENDER_H__

