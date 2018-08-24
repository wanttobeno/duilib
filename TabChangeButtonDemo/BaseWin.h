#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;

// 消息基类，所有的duilib窗口的事件处理类，派生这个消息
class CBaseNotify
{
public:
	virtual ~CBaseNotify(){};
	// 消息响应
	virtual void Notify(TNotifyUI& msg,bool& bHandle) = 0;
};

class CBaseWin :
	public WindowImplBase
//	, public CControlUI
{
protected:
	CBaseWin();
public:
	virtual ~CBaseWin();
public:
	// 消息路由
	virtual void Notify(TNotifyUI& msg);
	// 默认设置为 skin
	virtual CDuiString GetSkinFolder();
	// 指向xml路径，子类必须重写
	virtual CDuiString GetSkinFile() = 0;
	// 指向窗口类名，子类必须重写
	virtual LPCTSTR GetWindowClassName() const = 0;
	HWND GetHWnd() const;
	// 获取父窗口句柄
	HWND GetParent();
public:
	// 由duilib控制Win32窗口显示隐藏
	//virtual void SetInternVisible(bool bVisible = true);
	// 由duilib控制Win32窗口显示位置
	//virtual void SetPos(RECT rc);
protected:
	// 消息路由，由子类创建，删除
	CBaseNotify*	m_pBaseNotify;
};

