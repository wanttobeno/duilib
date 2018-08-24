#pragma once
#include "BaseWin.h"
#ifndef _DEBUG
#define  ZIP_SKIN 18
#endif // !_DEBUG


class CMainWnd :
	public CBaseWin
{
public:
	CMainWnd();
	virtual ~CMainWnd();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName()const;
	void InitWindow();


	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

#ifdef ZIP_SKIN
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResourceID() const;
#endif

	virtual void Notify(TNotifyUI& msg);


};

