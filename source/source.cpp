#include "ins.h"
#include "defs.h"
#include "source.h"
#include "av_source.h"
#include "file_source.h"

#ifdef USE_TORRENT
#include <boost/any.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "torrent_source.h"
#include "libtorrent/interface.hpp"

#endif // USE_TORRENT


#ifdef  __cplusplus
extern "C" {
#endif

static
void ansi_wide(std::string &ansi, std::wstring &wide)
{
	if (ansi.size() == 0)
		return ;
	wide.resize(ansi.size());
	std::size_t size = ansi.size();
	mbstowcs((wchar_t*)wide.c_str(), ansi.c_str(), size);
}

EXPORT_API int file_init_source(void *ctx)
{
	source_context *sc = (source_context*)ctx;
	if (sc->media_size <= 0 || !sc->media->name)
		return -1;

	file_source *fs = NULL;
	sc->io_dev = (void*)(fs = new file_source());

	// new open_file_data ��file_source�ڲ������ڴ�.
	open_file_data *od = new open_file_data;
	od->filename = std::string(sc->media->name);
	od->is_multithread = false;

	return fs->open((void*)od) ? 0 : -1;
}

EXPORT_API int file_read_data(void *ctx, char* buff, int64_t offset, int buf_size)
{
	source_context *sc = (source_context*)ctx;
	file_source *fs = (file_source *)sc->io_dev;
	uint64_t read_size = 0;
	bool ret = true;

	ret = fs->read_data(buff, offset, buf_size, read_size);
	if (ret)
		return read_size;

	return -1;
}

EXPORT_API void file_close(void *ctx)
{
	source_context *sc = (source_context*)ctx;
	file_source *fs = (file_source *)sc->io_dev;
	fs->close();
}

EXPORT_API void file_destory(void *ctx)
{
	source_context *sc = (source_context*)ctx;
	file_source *fs = (file_source *)sc->io_dev;
	if (fs)
	{
		fs->close();
		delete fs;
		sc->io_dev = NULL;
	}
}

EXPORT_API int bt_init_source(void *ctx)
{
#ifdef USE_TORRENT
	source_context *sc = (source_context*)ctx;
	torrent_source *ts = new torrent_source();
	open_torrent_data *otd = new open_torrent_data;

	// ����torrent��������.
	otd->is_file = false;
	char* dst = new char[sc->torrent_len];
	otd->torrent_data.reset(dst);
	memcpy(dst, sc->torrent_data, sc->torrent_len);
	otd->data_size = sc->torrent_len;

	// �õ���ǰ·��, ����utf8����.
	std::wstring path;
	std::string ansi;
	setlocale(LC_ALL, "chs");
	if (!sc->save_path)
	{
		ansi = boost::filesystem::current_path().string();
		ansi_wide(ansi, path);
		libtorrent::wchar_utf8(path, ansi);
		otd->save_path = ansi;
	}
	else
	{
		ansi = sc->save_path;
		ansi_wide(ansi, path);
		libtorrent::wchar_utf8(path, ansi);
		otd->save_path = ansi;
	}
	sc->io_dev = (void*)ts;

	if (ts->open((void*)otd))
		return 0;

	return -1;
#else
	return -1;
#endif // USE_TORRENT
}

EXPORT_API int bt_media_info(void *ctx, char *name, int64_t *pos, int64_t *size)
{
#ifdef USE_TORRENT
	source_context *sc = (source_context*)ctx;
	torrent_source *ts =  (torrent_source*)sc->io_dev;

	if (!ts)
		return -1;

	std::vector<video_file_info> vfi = ts->video_list();
	if (*pos < 0 || *pos >= vfi.size())
		return -1;

	video_file_info &info = vfi.at(*pos);
	if (info.filename.length() > *size)
		return -1;

	strcpy(name, info.filename.c_str());
	*pos = info.base_offset;
	*size = info.data_size;

	return vfi.size();
#else
	return -1;
#endif // USE_TORRENT
}

EXPORT_API int bt_read_data(void *ctx, char* buff, int64_t offset, int buf_size)
{
#ifdef USE_TORRENT
	source_context *sc = (source_context*)ctx;
	torrent_source *ts =  (torrent_source*)sc->io_dev;
	boost::uint64_t readbytes = 0;

	if (!ts->read_data(buff, offset, buf_size, readbytes))
		return -1;

	return readbytes;
#else
	return -1;
#endif // USE_TORRENT
}

EXPORT_API void bt_close(void *ctx)
{
#ifdef USE_TORRENT
	source_context *sc = (source_context*)ctx;
	torrent_source *ts =  (torrent_source*)sc->io_dev;
	ts->close();
#endif // USE_TORRENT
}

EXPORT_API void bt_destory(void *ctx)
{
#ifdef USE_TORRENT
	source_context *sc = (source_context*)ctx;
	torrent_source *ts =  (torrent_source*)sc->io_dev;
	ts->close();
	delete ts;
#endif // USE_TORRENT
}

#ifdef  __cplusplus
}
#endif
