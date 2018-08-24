#include "BaseWin.h"
#include <typeinfo>

CBaseWin::CBaseWin()
{
	m_pBaseNotify = NULL;
}


CBaseWin::~CBaseWin()
{
	int n = 0;
	if (m_pBaseNotify)
	{
		delete m_pBaseNotify;
		m_pBaseNotify = NULL;
	}
}


LPCTSTR CBaseWin::GetWindowClassName() const
{
	return _T("BaseWin");
}

// 消息路由
// 添加消息类型，方便调试
void CBaseWin::Notify(TNotifyUI& msg)
{
#ifdef _DEBUG
	if (msg.sType == _T("click"))
	{
		int n = 0;
	}
	else if (msg.sType == _T("selectchanged"))
	{
		int n = 0;
	}
	else if (msg.sType == _T("setfocus"))
	{
		int n = 0;
	}
	else if (msg.sType == _T("killfocus"))
	{
		int n = 0;
	}
	else if (msg.sType == _T("return"))
	{
		int n = 0;
	}
#endif // _DEBUG
	if (m_pBaseNotify)
	{
		bool bHandle = false;
		m_pBaseNotify->Notify(msg, bHandle);
		if (!bHandle)
			__super::Notify(msg);  // 确保消息被回调
	}
	else
		__super::Notify(msg);  // 确保消息被回调
}

CDuiString CBaseWin::GetSkinFolder()
{
	return _T("Skin");
}

CDuiString CBaseWin::GetSkinFile()
{
	return L"";
}

HWND CBaseWin::GetHWnd() const
{
	return m_hWnd;
}

HWND  CBaseWin::GetParent()
{
	if (m_hWnd)
		return ::GetParent(m_hWnd);
	else
		return NULL;
}

//void CBaseWin::SetInternVisible(bool bVisible)
//{
//	__super::SetInternVisible(bVisible);
//	::ShowWindow(m_hWnd, bVisible);
//}

//void CBaseWin::SetPos(RECT rc)
//{
//	__super::SetPos(rc);
//	::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
//}