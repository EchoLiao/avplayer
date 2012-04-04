#include "ins.h"
#include "file_source.h"

static const int BUFFER_SIZE = (1 << 21);
static const int AVG_READ_SIZE = (BUFFER_SIZE >> 1);

file_source::file_source()
   : m_file(NULL)
   , m_circle_buffer(NULL)
   , m_buffer_size(BUFFER_SIZE)
   , m_file_size(0)
   , m_offset(0)
   , m_write_p(0)
   , m_read_p(0)
{

}

file_source::~file_source()
{
   close();
}

bool file_source::open(void* ctx)
{
   // ����ctx.
   m_open_data.reset((open_file_data*)ctx);

   // ���ļ�.
   if (!boost::filesystem2::exists(m_open_data->filename))
      return false;

   // ��������.
   m_circle_buffer = new char[m_buffer_size];
   if (!m_circle_buffer)
      return false;

   // ����ָ��.
   m_write_p = m_read_p = m_offset = 0;

   // ����ļ���С.
   m_file_size = boost::filesystem2::file_size(m_open_data->filename);

   // ���ļ�.
   m_file = fopen(m_open_data->filename.c_str(), "rb");
   if (!m_file)
      return false;

   return true;
}

bool file_source::read_data(char* data, boost::uint64_t offset, boost::uint64_t size, boost::uint64_t& read_size)
{
   static char read_buffer[AVG_READ_SIZE];
   // ���ݲ����Զ�����.
   boost::shared_ptr<boost::mutex::scoped_lock> lock;
   if (m_open_data->is_multithread)
      lock.reset(new boost::mutex::scoped_lock(m_mutex));

   read_size = 0;

   // ��ȡ����Խ��.
   if (offset >= m_file_size)
      return false;

	if (!m_file)
		return true;

   // ������ݶ�ȡλ���ڻ��巶Χ��, ��ֱ�Ӵӻ�������ȡ����.
   if (m_offset <= offset && offset < m_offset + (m_write_p - m_read_p))
   {
      // �����λ��.
      unsigned int p = offset - m_offset;
      m_read_p += p;
      // �ӻ����ж�ȡ����.
      read_size = get_data(data, size);
      // ����ƫ��λ��.
      m_offset = offset + read_size;
   }
   else
   {
      // ���ļ��ж�ȡ����.
      m_offset = offset;
      m_write_p = m_read_p = 0;

      // �Ƶ�ƫ��λ��.
      fseek(m_file, offset, SEEK_SET);
      // ��ʼ��ȡ����.
      int r = fread(m_circle_buffer, 1, AVG_READ_SIZE, m_file);
      m_write_p += r;
      // �������ݵ���ȡbuf.
      size = _min(AVG_READ_SIZE, size);
      memcpy(data, m_circle_buffer, size);
      // ���·������ݴ�С.
      read_size = size;
   }

   // ���С�ڻ��峤�ȵ�һ��, ����ļ���ȡһ�뻺�峤�ȵ�����
   // ������.
   offset = m_offset + (m_write_p - m_read_p);
   if (available_size() < AVG_READ_SIZE &&
      offset < m_file_size)
   {
      fseek(m_file, offset, SEEK_SET);
      int r = fread(read_buffer, 1, AVG_READ_SIZE, m_file);
      if (r > 0)
         put_data(read_buffer, r);
   }

   return true;
}

void file_source::close()
{
   if (m_file)
   {
      fclose(m_file);
      m_file = NULL;
   }

   if (m_circle_buffer)
   {
      delete[] m_circle_buffer;
      m_circle_buffer = NULL;
   }

   m_write_p = m_read_p = m_offset = 0;
}

unsigned int file_source::put_data(char* buffer, unsigned int len)
{
   unsigned int l;
   len = _min(len, m_buffer_size - m_write_p + m_read_p);
   /* first put the data starting from fifo->in to buffer end */
   l = _min(len, m_buffer_size - (m_write_p & (m_buffer_size - 1)));
   memcpy(m_circle_buffer + (m_write_p & (m_buffer_size - 1)), buffer, l);
   /* then put the rest (if any) at the beginning of the buffer */
   memcpy(m_circle_buffer, buffer + l, len - l);
   m_write_p += len;
   return len;
}

unsigned int file_source::get_data(char* buffer, unsigned int len)
{
   unsigned int l; 
   len = _min(len, m_write_p - m_read_p); 
   /* first get the data from fifo->out until the end of the buffer */ 
   l = _min(len, m_buffer_size - (m_read_p & (m_buffer_size - 1))); 
   memcpy(buffer, m_circle_buffer + (m_read_p & (m_buffer_size - 1)), l); 
   /* then get the rest (if any) from the beginning of the buffer */ 
   memcpy(buffer + l, m_circle_buffer, len - l); 
   m_read_p += len; 
   return len; 
}

inline unsigned int file_source::_max(unsigned int a, unsigned int b)
{
   return max(a, b);
}

inline unsigned int file_source::_min(unsigned int a, unsigned int b)
{
   return min(a, b);
}

