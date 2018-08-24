#include <windows.h>
#include <GdiPlus.h>
#include "MainWnd.h"

#ifdef _DEBUG
#   pragma comment(lib, "..\\UnicodeDebug\\DuiLib_Static_ud.lib")
#else
#   pragma comment(lib,	"..\\UnicodeRelease\\DuiLib_static_u.lib")
#endif


ULONG_PTR m_gdiplusToken;
int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	Gdiplus::GdiplusStartupInput StartupInput;
	GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);
	CPaintManagerUI::SetInstance(hInstance);
	CMainWnd mainWnd;
	mainWnd.Create(NULL, L"36kr客户端", UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW);
	mainWnd.CenterWindow();
	mainWnd.ShowModal();

	// 避免错误，交给系统完成
	//Gdiplus::GdiplusShutdown(m_gdiplusToken);
	return 0;

	AllocConsole();
	freopen("conout$", "w", stdout);
	return 0;
}