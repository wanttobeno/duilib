// DuilibTest.cpp : 定义应用程序的入口点。
//

#include "DuiFrameWnd.h"

#ifdef _DEBUG
#pragma  comment(lib,"../lib/DuiLib_ud.lib")
#else
#pragma  comment(lib,"../lib/DuiLib_u.lib")
#endif // _DEBUG


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFrameWnd duiFrame;
	//duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	//duiFrame.Create(NULL, _T("渐变按钮"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	duiFrame.Create(NULL, L"渐变按钮", UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW,  0, 0, 500, 300);
	duiFrame.CenterWindow();
	duiFrame.ShowModal();
	return 0;
}