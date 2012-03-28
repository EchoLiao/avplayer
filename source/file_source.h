//
// file_source.h
// ~~~~~~~~~~~~~
//
// Copyright (c) 2011 Jack (jack.wgm@gmail.com)
//

#ifndef __FILE_SOURCE_H__
#define __FILE_SOURCE_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "av_source.h"

struct open_file_data 
{
   // �Ƿ��Ƕ��̷߳���, ������̷߳���
   // , ���ڲ��Զ�ʹ���������.
   bool is_multithread;

   // ���ļ���.
   std::string filename;
};

class file_source
   : public av_source
{
public:
   file_source();
   virtual ~file_source();

public:
   // ��.
   virtual bool open(void* ctx);

   // ��ȡ����.
   virtual bool read_data(char* data, boost::uint64_t offset, boost::uint64_t size, boost::uint64_t& read_size);

   // �ر�.
   virtual void close();

private:
   // ѭ�������������.
   unsigned int put_data(char* buffer, unsigned int len);
   unsigned int get_data(char* buffer, unsigned int len);
   inline unsigned int available_size() { return m_write_p - m_read_p; }
   inline unsigned int _max(unsigned int a, unsigned int b);
   inline unsigned int _min(unsigned int a, unsigned int b);

private:
   // �ļ��򿪽ṹ.
   boost::shared_ptr<open_file_data> m_open_data;

   // �ļ�ָ��.
   FILE* m_file;

   // �ļ���С.
   boost::uint64_t m_file_size;

   // ѭ������λ��.
   unsigned int m_offset;

   // ѭ��������.
   char* m_circle_buffer;

   // �����С.
   unsigned int m_buffer_size;

   // дָ���±�.
   unsigned int m_write_p;

   // ��ָ���±�.
   unsigned int m_read_p;

   // �̰߳�ȫ��.
   mutable boost::mutex m_mutex;
};

#endif // __FILE_SOURCE_H__

