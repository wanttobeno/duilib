#include "../duilib/UIlib.h" 
#include "UIFadeButton.h"
#include "BaseWin.h"

using namespace DuiLib;
class CDuiFrameWnd : public CBaseWin
{
public:
	CDuiFrameWnd()
	{

	}
	virtual CDuiString GetSkinFile()
	{
		return _T("FadeButton.xml");
	}
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("FadeButtonDemo");
	}
public:
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcscmp(pstrClass, _T("FadeButton")) == 0)
			return new CFadeButtonUI;
		return NULL;
	}
};
