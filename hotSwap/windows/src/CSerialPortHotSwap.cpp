#include "CSerialPortHotSwap.h"

#include <tchar.h>
#include <dbt.h>
#include <process.h>//_beginthreadex

#define CLASS_NAME _T("itas109SerialPortChangeWnd")

using namespace itas109;

// TODO: static not best
sigslot::signal2<int, std::string> CSerialPortHotSwap::portChangeSignal;

std::string wstringToString(const std::wstring &wstr)
{
	// https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
	if (wstr.empty())
	{
		return std::string();
	}

	int size = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string ret = std::string(size, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &ret[0], size, NULL, NULL);//CP_UTF8

	return ret;
}

CSerialPortHotSwap::CSerialPortHotSwap()
{
	_beginthreadex(NULL, 0, ThrdFunc, (LPVOID)this, 0, NULL);
}


CSerialPortHotSwap::~CSerialPortHotSwap()
{
}

BOOL CSerialPortHotSwap::initMessageOnlyWindows()
{
	//https://docs.microsoft.com/zh-cn/windows/win32/devio/registering-for-device-notification

	WNDCLASSEX wndClass = { 0 };

	HINSTANCE hInstance = NULL;
	hInstance = GetModuleHandle(NULL);

	if (hInstance == NULL)
	{
		return FALSE;
	}

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(_WinProcCallback);
	wndClass.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));

	if (!RegisterClassEx(&wndClass))
	{
		return FALSE;
	}

	hWnd = CreateWindowEx(0, CLASS_NAME, 0, 0, 0, 0, 0, 0, 0/*HWND_MESSAGE*/, 0, hInstance, 0);

	if (hWnd == NULL)
	{
		return FALSE;

	}

	MessagePump();

	return TRUE;
}

LRESULT CALLBACK CSerialPortHotSwap::_WinProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRet = 1;

	// CSerialPortHotSwap *p_base = (CSerialPortHotSwap*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_DEVICECHANGE:
	{
		PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;

		if (lpdb && lpdb->dbch_devicetype == DBT_DEVTYP_PORT)
		{
			PDEV_BROADCAST_PORT pDevInf = (PDEV_BROADCAST_PORT)lpdb;
			switch (wParam)
			{
			case DBT_DEVICEARRIVAL:
#ifdef UNICODE
				portChangeSignal._emit(1, wstringToString(pDevInf->dbcp_name));
#else
				portChangeSignal._emit(1, pDevInf->dbcp_name);
#endif // UNICODE

				break;
			case DBT_DEVICEREMOVECOMPLETE:
#ifdef UNICODE
				portChangeSignal._emit(0, wstringToString(pDevInf->dbcp_name));
#else
				portChangeSignal._emit(0, pDevInf->dbcp_name);
#endif // UNICODE
				break;
			default:
				break;
			}
		}
	}
		break;
	default:
		// Send all other messages on to the default windows handler.
		lRet = DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return lRet;
}

void CSerialPortHotSwap::MessagePump()
{
	MSG msg;
	int retVal;

	while ((retVal = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (retVal == -1)
		{
			break;
		}
		else
		{
			//TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

unsigned int __stdcall CSerialPortHotSwap::ThrdFunc(LPVOID lpParam)
{
	CSerialPortHotSwap * p_main = (CSerialPortHotSwap *)lpParam;
	p_main->initMessageOnlyWindows();

	return 1;
}

std::string CSerialPortHotSwap::getVersion()
{
    return "CSerialPortHotSwap V1.0.0.200510";
}
