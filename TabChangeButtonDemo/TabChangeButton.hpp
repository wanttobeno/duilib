#pragma once
#ifndef __TABCHANGEBUTTON_HPP_2015_01_13
#define __TABCHANGEBUTTON_HPP_2015_01_13

#include "../DuiLib/UIlib.h"

#include "Define.hpp"

#include "slidecontainer.hpp"

namespace DuiLib
{
	class CTabChangeBase
	{
	public:
		CTabChangeBase() : _m_nSelectedID(0) {}
		void	SetTab(LPCTSTR _lpTab){_m_strTab = _lpTab;}
		LPCTSTR GetTab(){return _m_strTab;}
		void	SetSelectedID(int _nID){_m_nSelectedID = _nID;}
		int		GetSelectedID(){return _m_nSelectedID;}
	protected:
		CDuiString			_m_strTab;
		int					_m_nSelectedID;
	};
	// 根据设置更新Tab
	class CTabChangeButtonUI : public CButtonUI, public CTabChangeBase
	{
	public:
		CTabChangeButtonUI(){}
		virtual ~CTabChangeButtonUI(){}
		LPCTSTR GetClass() const{return DUI_CTR_TABCHAGEBUTTON;}
		LPVOID GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CTabChangeButtonUI*>(this);return __super::GetInterface(pstrName);}
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("tab")) == 0)
				_m_strTab = pstrValue;
			if (_tcscmp(pstrName, _T("selectedid")) == 0)
				_m_nSelectedID = xk_cast<int>(pstrValue);
			else
				__super::SetAttribute(pstrName, pstrValue);
		}
		virtual void	DoEvent(TEventUI& event)
		{
			switch (event.Type)
			{
			case UIEVENT_BUTTONUP:
				if (!_m_strTab.IsEmpty())
				{
					CTabLayoutUI* pTab = static_cast<CTabLayoutUI*>(GetManager()->FindControl(_m_strTab)->GetInterface(DUI_CTR_TABLAYOUT));
					if (pTab)
						pTab->SelectItem(_m_nSelectedID);
					return;
				}
				break;
			}
			__super::DoEvent(event);
		}
	};
	// 根据设置更新Tab，并且Tab变化时自动选择或取消选择当前Option
	class CTabChangeOptionUI : public COptionUI, public INotifyUI, public CTabChangeBase
	{
	public:
		CTabChangeOptionUI(){}
		virtual ~CTabChangeOptionUI(){}
		LPCTSTR GetClass() const{return DUI_CTR_TABCHAGEOPTION;}
		LPVOID GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CTabChangeOptionUI*>(this);return __super::GetInterface(pstrName);}
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("tab")) == 0)
				_m_strTab = pstrValue;
			if (_tcscmp(pstrName, _T("selectedid")) == 0)
				_m_nSelectedID = xk_cast<int>(pstrValue);
			else
				__super::SetAttribute(pstrName, pstrValue);
		}
		virtual void DoInit()
		{
			GetManager()->AddNotifier(this);
			__super::DoInit();
		}
		virtual void DoEvent(TEventUI& event)
		{
			switch (event.Type)
			{
			case UIEVENT_BUTTONUP:
				if (!_m_strTab.IsEmpty() && IsMouseEnabled() && IsEnabled() && IsVisible())
				{
					CTabLayoutUI* pTab = static_cast<CTabLayoutUI*>(GetManager()->FindControl(_m_strTab)->GetInterface(DUI_CTR_TABLAYOUT));
					if (pTab)
						pTab->SelectItem(_m_nSelectedID);
					CSlideHorizontalLayoutUI* pSlideH = static_cast<CSlideHorizontalLayoutUI*>(GetManager()->FindControl(_m_strTab)->GetInterface(DUI_CTR_SLIDEHORIZONTALLAYOUT));
					if (pSlideH)
						pSlideH->SelectItem(_m_nSelectedID);
					CSlideVerticalLayoutUI* pSlideV = static_cast<CSlideVerticalLayoutUI*>(GetManager()->FindControl(_m_strTab)->GetInterface(DUI_CTR_SLIDEVERTICALLAYOUT));
					if (pSlideV)
						pSlideV->SelectItem(_m_nSelectedID);
					return;
				}
				break;
			}
			__super::DoEvent(event);
		}
	protected:
		virtual void Notify(TNotifyUI& msg)
		{
			if (_m_strTab.IsEmpty()) return;
			if (msg.sType == DUI_MSGTYPE_TABSELECT && msg.pSender->GetName() == _m_strTab)
				Selected(msg.wParam == _m_nSelectedID);
		}
	};
}

#endif