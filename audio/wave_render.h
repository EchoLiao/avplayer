//
// wave_render.h
// ~~~~~~~~~~~~~
//
// Copyright (c) 2011 Jack (jack.wgm@gmail.com)
//

#ifndef __WAVE_RENDER_H__
#define __WAVE_RENDER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "audio_render.h"

// namespace libavplayer {

class wave_render
   : public audio_render
{
public:
   wave_render();
   virtual ~wave_render();

public:
   // ��ʼ����Ƶ���.
   virtual bool init_audio(void* ctx, DWORD channels, DWORD bits_per_sample, DWORD sample_rate, int format);

   // ������Ƶ����.
   virtual int play_audio(uint8_t* data, uint32_t size);

   // ��Ƶ���ſ���, cmdΪCONTROL_��ʼ�ĺ궨��.
   virtual void audio_control(int cmd, void* arg);

   // ������Ƶ������.
   virtual void destory_audio();

private:
   static void __stdcall waveOutProc(HWAVEOUT hWaveOut,UINT uMsg,DWORD dwInstance,
      DWORD dwParam1,DWORD dwParam2);

private:
   // waveOut���.
   HWAVEOUT m_hwaveout;

   // �����С.
   int m_buffersize;

   // pointer to our ringbuffer memory.
   WAVEHDR* m_wave_blocks;

   // д��λ��.
   unsigned int m_buf_write;

   // ����λ��.
   volatile int m_buf_read;
};

// } // namespace libavplayer


#endif // __WAVE_RENDER_H__
