// ������뽫λ������ָ��ƽ̨֮ǰ��ƽ̨��ΪĿ�꣬���޸����ж��塣
// �йز�ͬƽ̨��Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ����ֵ����Ϊ�ʵ���ֵ����ָ���� Windows Me ����߰汾��ΪĿ�ꡣ
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
#endif

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

// Windows ͷ�ļ�:
#include <windows.h>
#include <ShellAPI.h>

// C ����ʱͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <boost/filesystem.hpp>	// for boost::filesystem3::extension

#include "avplayer.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
							  HINSTANCE hPrevInstance,
							  LPTSTR    lpCmdLine,
							  int       nCmdShow)
{
	setlocale(LC_ALL, "chs");

	MSG msg;
	int ret = NULL;
	WCHAR filename[MAX_PATH];

	if (_tcslen(lpCmdLine) == 0)
		return -1;

	_tcscpy(filename, lpCmdLine);

	// ����������.
	avplayer win;

	// ��������������.
	if (win.create_window(_T("main")) == NULL)
		return -1;

#if 0
	// ���Խ����ⲥ�Ŵ��ڲ���, Ҳ����ʹ���ⲿ�����Ĵ���.
	HWND hWnd = (HWND)0x0002133C;
	if (!win.subclasswindow(hWnd, FALSE))
		return -1;
#endif

	// �����ļ���չ���ж��Ƿ�Ϊbt�����ļ�, �����bt�����ļ�
	// �����bt��ʽ���ز�����.
	std::string ext = boost::filesystem3::extension(filename);
	if (ext == ".torrent")
	{
		if (!win.open(filename, MEDIA_TYPE_BT))
			return -1;
	}
	else
	{
		if (!win.open(filename, MEDIA_TYPE_FILE))
			return -1;
	}

	// �򿪳ɹ�, ��ʼ����.
	win.play();

	// ��Ϣѭ��.
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			else
			{
				TranslateMessage(&msg);	// Translate The Message
				DispatchMessage(&msg);	// Dispatch The Message
			}
		}
		else
		{
			Sleep(10); // do some...
		}
	}

	// �رղ�����.
	win.close();

	return 0;
}
