#pragma once
#ifndef __SLIDECONTAINER_HPP_2014_12_9
#define __SLIDECONTAINER_HPP_2014_12_9
#include "../DuiLib/UIlib.h"

#include "Define.hpp"


namespace DuiLib
{
	class CSlideBase
	{
	public:
		CSlideBase()
			: _m_bEnableMove(true)
			, _m_bEnableParentSlide(true)
		{}
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("move")) == 0)
				EnableMove(_tcscmp(pstrValue, _T("true")) == 0);
			else if (_tcscmp(pstrName, _T("parentslide")) == 0)
				EnableParentSlide(_tcscmp(pstrValue, _T("true")) == 0);
		}
		void EnableMove(bool _bMove){_m_bEnableMove = _bMove;}
		bool IsEnableMove(){return _m_bEnableMove;}
		void EnableParentSlide(bool _bEnable){_m_bEnableParentSlide = _bEnable;}
		bool IsEnableParentSlide(){return _m_bEnableParentSlide;}
	protected:
		bool		_m_bEnableMove;
		bool		_m_bEnableParentSlide;
	};

	class CSlideListUI : public CListUI, public CSlideBase
	{
	public:
		CSlideListUI()
			: _m_nMoveToItem(-1)
		{}
		virtual ~CSlideListUI(){}
		LPCTSTR GetClass() const{return DUI_CTR_SLIDELIST;}
		LPVOID GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CSlideListUI*>(this);return CListUI::GetInterface(pstrName);}
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			CSlideBase::SetAttribute(pstrName, pstrValue);
			CListUI::SetAttribute(pstrName, pstrValue);
		}
		void SetPage(int _nIndex){_m_nMoveToItem = _nIndex;}
		void DoEvent(TEventUI& event)
		{
			switch (event.Type)
			{
			case UIEVENT_SLIDEVERTICAL:
				if (IsEnableMove() && IsEnabled() && GetList()->GetVerticalScrollBar() && GetList()->GetVerticalScrollBar()->IsVisible())
				{
					SIZE szPos = {0, GetScrollPos().cy + event.lParam};
					int nOldPos = GetScrollPos().cy;
					SetScrollPos(szPos);
					if (IsEnableParentSlide() && nOldPos == GetScrollPos().cy) break;
					return;
				}
				break;
			}
			CListUI::DoEvent(event);
		}
		// 绘制
		virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI*)
		{
			__super::DoPaint(hDC, rcPaint, nullptr);
			if (_m_nMoveToItem >= 0 && _m_nMoveToItem < GetCount())
			{
				SIZE szScroll = {0, 0};
				for (int i = 0; i < _m_nMoveToItem; ++i)
					szScroll.cy += CDuiRect(GetItemAt(i)->GetPos()).GetHeight();
				SetScrollPos(szScroll);
				_m_nMoveToItem = -1;
			}
			return true;
		}
	protected:
		int		_m_nMoveToItem;
	};

	class CListPureContainerElementUI : public CListContainerElementUI
	{
	public:
		CListPureContainerElementUI(){}
		virtual ~CListPureContainerElementUI(){}
		LPCTSTR GetClass() const{return DUI_CTR_LISTPURECONTAINERELEMENT;}
		LPVOID GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CListPureContainerElementUI*>(this);return __super::GetInterface(pstrName);}
		bool DoPaint(HDC hDC, const RECT& rcPaint, DuiLib::CControlUI*){ return CContainerUI::DoPaint(hDC, rcPaint, nullptr); }
	};

	class CSlideTileLayoutUI : public CTileLayoutUI, public CSlideBase
	{
	public:
		CSlideTileLayoutUI()
			: _m_nMoveToItem(-1)
		{}
		virtual ~CSlideTileLayoutUI(){}
		LPCTSTR GetClass() const{return DUI_CTR_SLIDETILELAYOUT;}
		LPVOID GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CSlideTileLayoutUI*>(this);return CTileLayoutUI::GetInterface(pstrName);}
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			CSlideBase::SetAttribute(pstrName, pstrValue);
			CTileLayoutUI::SetAttribute(pstrName, pstrValue);
		}
		void SetPage(int _nIndex){_m_nMoveToItem = _nIndex;}
		void DoEvent(TEventUI& event)
		{
			switch (event.Type)
			{
			case UIEVENT_SLIDEVERTICAL:
				if (IsEnableMove() && IsEnabled() && GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
				{
					SIZE szPos = {0, GetScrollPos().cy + event.lParam};
					int nOldPos = GetScrollPos().cy;
					SetScrollPos(szPos);
					if (IsEnableParentSlide() && nOldPos == GetScrollPos().cy) break;
					return;
				}
				break;
			}
			CTileLayoutUI::DoEvent(event);
		}
		// 绘制
		virtual bool DoPaint(HDC hDC, const RECT& rcPaint, DuiLib::CControlUI*)
		{
			__super::DoPaint(hDC, rcPaint, nullptr);
			if (_m_nMoveToItem >= 0 && _m_nMoveToItem < GetCount())
			{
				SIZE szScroll = {0, 0};
				for (int i = 0; i <= _m_nMoveToItem; ++i)
					szScroll.cy += CDuiRect(GetItemAt(i)->GetPos()).GetHeight();
				SetScrollPos(szScroll);
				_m_nMoveToItem = -1;
			}
			return true;
		}
		void SetPos(RECT rc)
		{
			__super::SetPos(rc);
		}
	protected:
		int		_m_nMoveToItem;
	};

	template<typename T>
	class CSlideContainerBase : public T, public CSlideBase
	{
	protected:
		enum ALIGN_POINT
		{
			TOP_LEFT,
			TOP_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_RIGHT,
			TOP_CENTER,
			BOTTOM_CENTER,
			LEFT_CENTER,
			RIGHT_CENTER,
			CENTER,
		};
	public:
		CSlideContainerBase()
			: _m_nXPos(0)
			, _m_nYPos(0)
			, _m_nThisPage(-1)
			, _m_nNextPage(-1)
			, _m_nDefautPage(0)
			, _m_nSpeed(100)
			, _m_nMaxDistanceToNext(50)
			, _m_eAlignPoint(TOP_LEFT)
			, _m_dwUpdateTime(0)
			, _m_bSlide(false)
			, _m_bAutoMove(true)
			, _m_bOverBorder(true)
			, _m_bMoveHor(true)
			, _m_bMoveVer(true)
		{
			ZeroMemory(&_m_rcOver, sizeof(RECT));
		}
		virtual ~CSlideContainerBase(){}
		virtual UINT GetControlFlags() const{return T::GetControlFlags() | UIFLAG_ANIMATION;}
		virtual void DoInit()
		{
			__super::DoInit();
			if (GetCount())
				_m_nThisPage = _m_nDefautPage < GetCount() ? _m_nDefautPage : 0;
		}
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			CSlideBase::SetAttribute(pstrName, pstrValue);
			if (_tcscmp(pstrName, _T("speed")) == 0)
			{
				LPTSTR pstr = NULL;
				_m_nSpeed = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("selectedid")) == 0)
			{
				LPTSTR pstr = NULL;
				_m_nDefautPage = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("distance")) == 0)
			{
				LPTSTR pstr = NULL;
				_m_nMaxDistanceToNext = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("alignpoint")) == 0)
			{
				if (_tcscmp(pstrValue, _T("topleft")) == 0)
					_m_eAlignPoint = TOP_LEFT;
				else if (_tcscmp(pstrValue, _T("topright")) == 0)
					_m_eAlignPoint = TOP_RIGHT;
				else if (_tcscmp(pstrValue, _T("topcenter")) == 0)
					_m_eAlignPoint = TOP_CENTER;
				else if (_tcscmp(pstrValue, _T("bottomleft")) == 0)
					_m_eAlignPoint = BOTTOM_LEFT;
				else if (_tcscmp(pstrValue, _T("bottomright")) == 0)
					_m_eAlignPoint = BOTTOM_RIGHT;
				else if (_tcscmp(pstrValue, _T("bottomcenter")) == 0)
					_m_eAlignPoint = BOTTOM_CENTER;
				else if (_tcscmp(pstrValue, _T("leftcenter")) == 0)
					_m_eAlignPoint = LEFT_CENTER;
				else if (_tcscmp(pstrValue, _T("rightcenter")) == 0)
					_m_eAlignPoint = RIGHT_CENTER;
				else if (_tcscmp(pstrValue, _T("center")) == 0)
					_m_eAlignPoint = CENTER;
			}
			else if (_tcscmp(pstrName, _T("over")) == 0)
			{
				RECT rcOver = { 0 };
				LPTSTR pstr = NULL;
				rcOver.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rcOver.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
				rcOver.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
				rcOver.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
				_m_rcOver = rcOver;
			}
			else if (_tcscmp(pstrName, _T("automove")) == 0)
			{
				_m_bAutoMove = _tcscmp(pstrValue, _T("true")) == 0;
			}
			else if (_tcscmp(pstrName, _T("overborder")) == 0)
			{
				_m_bOverBorder = _tcscmp(pstrValue, _T("true")) == 0;
			}
			else if (_tcscmp(pstrName, _T("movehor")) == 0)
			{
				_m_bMoveHor = _tcscmp(pstrValue, _T("true")) == 0;
			}
			else if (_tcscmp(pstrName, _T("movever")) == 0)
			{
				_m_bMoveVer = _tcscmp(pstrValue, _T("true")) == 0;
			}
			else
				T::SetAttribute(pstrName, pstrValue);
		}
		// 移动到子页面
		virtual void SelectItem(int _nIndex)
		{
			if (_nIndex >= GetCount()) _nIndex = GetCount() - 1;
			if (_nIndex < 0) _nIndex = 0;
			_m_nNextPage = _nIndex;
		}
		virtual int  GetSelectItem(){return _m_nThisPage;}
		// 设置页面 和 SelectItem 不同的是不会经过动画，而是直接变化到当前面
		virtual void SetPage(int _nIndex)
		{
			if (_nIndex >= GetCount()) _nIndex = GetCount() - 1;
			if (_nIndex < 0) _nIndex = 0;
			_m_nNextPage = _nIndex;
			_AutoMove(-1);
			_m_nDefautPage = -1;
			_AutoMove(-1);
			NeedUpdate();
		}
		// 获取当前页面的序号
		virtual int	 GetCurSel()
		{
			return _m_nThisPage;
		}
		// 设置位置
		virtual void SetPos(RECT rc)
		{
			T::SetPos(rc);
			for (int i = 0; i < GetCount(); ++i)
			{
				CControlUI* pContrl = GetItemAt(i);
				CDuiRect rcContrl(pContrl->GetPos());
				rcContrl.Offset(_m_nXPos, _m_nYPos);
				pContrl->SetPos(rcContrl);
			}
		}
		// 绘制
		virtual bool DoPaint(HDC hDC, const RECT& rcPaint, DuiLib::CControlUI*)
		{
			if (_m_nDefautPage >= 0 && _m_nDefautPage < GetCount())
			{
				_m_nNextPage = _m_nDefautPage;
				_AutoMove(-1);
				_m_nDefautPage = -1;
			}
// 			else if (_m_nNextPage >= 0 && _m_nNextPage < GetCount())
// 			{
// 				DWORD dwTime = ::GetTickCount();
// 				if (dwTime - _m_dwUpdateTime > 10)
// 				{
// 					_AutoMove(_m_nSpeed);
// 					_m_dwUpdateTime = dwTime;
// 				}
// 				NeedUpdate();
// 			}
			return T::DoPaint(hDC, rcPaint, nullptr);
		}
		// 事件
		virtual void DoEvent(TEventUI& event)
		{
			switch (event.Type)
			{
			case UIEVENT_SLIDEBEGIN:
				if (!IsEnableMove() || !IsEnabled()) break;
				_m_bSlide = _Move(	_m_bMoveHor ? -static_cast<int>(event.wParam) : 0, 
									_m_bMoveVer ? -static_cast<int>(event.lParam) : 0, false);
				if (!_m_bSlide && IsEnableParentSlide()) break;
				return;
			case UIEVENT_SLIDEHORIZONTAL:
				if (IsEnableMove() && IsEnabled() && _m_bMoveHor)
				{
					if (!_m_bSlide && IsEnableParentSlide()) break;
					_Move(	_m_bMoveHor ? -static_cast<int>(event.wParam) : 0, 
							_m_bMoveVer ? -static_cast<int>(event.lParam) : 0);
					NeedUpdate();
					return;
				}
				break;
			case UIEVENT_SLIDEVERTICAL:
				if (IsEnabled() && _m_bMoveVer)
				{
					if (!_m_bSlide && IsEnableParentSlide()) break;
					_Move(	_m_bMoveHor ? -static_cast<int>(event.wParam) : 0, 
							_m_bMoveVer ? -static_cast<int>(event.lParam) : 0);
					NeedUpdate();
					return;
				}
				break;
			case UIEVENT_SLIDEEND:
				if (!IsEnableMove() || !IsEnabled()) break;
				_MoveOver();
				if (!_m_bSlide && IsEnableParentSlide()) break;
				_m_bSlide = false;
				return;
			case UIEVENT_ANIMATION:
				_DoAnimation();
				return;
			}
			T::DoEvent(event);
		}
	// 属性操作
	public:
		virtual void SetSpeed(int _nSpeed){_m_nSpeed = _nSpeed;}
		virtual int  GetSpeed(){return _m_nSpeed;}
		virtual void SetAutoMove(bool _bAutoMove){_m_bAutoMove = _bAutoMove;}
		virtual bool IsAutoMove(){return _m_bAutoMove;}
		virtual void SetMaxDistanceToNext(int _nMaxDistanceToNext){_m_nMaxDistanceToNext = _nMaxDistanceToNext;}
		virtual int  GetMaxDistanceToNext(){return _m_nMaxDistanceToNext;}
	protected:
		// 动画消息
		void _DoAnimation()
		{
			DWORD dwNow = ::GetTickCount();
			if (dwNow - _m_dwUpdateTime < 10) return;
			_m_dwUpdateTime = dwNow;
			if (_m_nNextPage >= 0 && _m_nNextPage < GetCount())
			{
				_AutoMove(_m_nSpeed);
			}
		}
		// 自动移动
		void _AutoMove(int _nSpeed)
		{
			if (!_m_bAutoMove && _m_nDefautPage == -1)
				_m_nNextPage = -1;
			if (_m_nNextPage >= 0)
			{
				CDuiRect rcPage = GetItemAt(_m_nNextPage)->GetPos();
				CDuiRect rcClient = GetPos();
				int nxMoveLen = 0;
				int nyMoveLen = 0;
				if (_AlignPoint(rcPage, rcClient, nxMoveLen, nyMoveLen))
				{
					if (GetManager() && _m_nThisPage != _m_nNextPage)
					{
						_m_nThisPage = _m_nNextPage;
						GetManager()->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED, _m_nNextPage);
					}
					_m_nNextPage = -1;
					return;
				}
				int nX = nxMoveLen;
				int nY = nyMoveLen;
				if (_nSpeed > 0)
				{
					nX = abs(nxMoveLen) > _nSpeed ? nxMoveLen > 0 ? _nSpeed : -_nSpeed : nxMoveLen;
					nY = abs(nyMoveLen) > _nSpeed ? nyMoveLen > 0 ? _nSpeed : -_nSpeed : nyMoveLen;
				}
				_Move(nX, nY);
			}
		}
		// 偏移容器中的项
		bool _Move(int _nXPos, int _nYPos, bool _bMove = true)
		{
			if (GetCount() == 0) return false;
			// 根据设置的越界大小_m_rcOver, 计算在越界的情况下应该移动多少距离
			RECT rcBegin(GetItemAt(0)->GetPos());
			RECT rcEnd(GetItemAt(GetCount() - 1)->GetPos());
			RECT rcContainer(GetPos());
			// 横向是否超过最左边
			if (_nXPos > 0)
			{
				int nXPos = 0, nYPos = 0;
				_AlignPoint(rcBegin, rcContainer, nXPos, nYPos);
				if (nXPos + _m_rcOver.left < _nXPos) _nXPos = nXPos + _m_rcOver.left;
			}
			// 横向是否超过最右边
			else if (_nXPos < 0)
			{
				int nXPos = 0, nYPos = 0;
				_AlignPoint(rcEnd, rcContainer, nXPos, nYPos);
				if (nXPos - _m_rcOver.right > _nXPos) _nXPos = nXPos - _m_rcOver.right;
			}
			// 纵向是否超过最上边
			if (_nYPos > 0)
			{
				int nXPos = 0, nYPos = 0;
				_AlignPoint(rcBegin, rcContainer, nXPos, nYPos);
				if (nYPos + _m_rcOver.top < _nYPos) _nYPos = nYPos + _m_rcOver.top;
			}
			// 纵向是否超过最下边
			else if (_nYPos < 0)
			{
				int nXPos = 0, nYPos = 0;
				_AlignPoint(rcEnd, rcContainer, nXPos, nYPos);
				if (nYPos - _m_rcOver.bottom > _nYPos) _nYPos = nYPos - _m_rcOver.bottom;
			}
			// 判断是否越过边界
			if (!_m_bOverBorder && GetCount() != 0)
			{
				RECT rcPos = GetPos();
				RECT rcFirst = GetItemAt(0)->GetPos();
				RECT rcLast = GetItemAt(GetCount() - 1)->GetPos();
				if (_nXPos != 0)
				{
					if (rcFirst.left + _nXPos > rcPos.left)
						_nXPos = rcPos.left - rcFirst.left;
					else if (rcLast.right + _nXPos < rcPos.right)
						_nXPos = rcPos.right - rcLast.right;
				}
				if (_nYPos != 0)
				{
					if (rcFirst.top + _nYPos > rcPos.top)
						_nYPos = rcPos.top - rcFirst.top;
					else if (rcLast.bottom + _nYPos < rcPos.bottom)
						_nYPos = rcPos.bottom - rcLast.bottom;
				}
			}
			if (!_nXPos && !_nYPos) return false;
			if (_bMove)
			{
				_m_nXPos += _nXPos;
				_m_nYPos += _nYPos;
				SetPos(GetPos());
			}
			return true;
		}
		// 移动结束
		void _MoveOver()
		{
			_m_nNextPage = _NextPageWhileMove(_m_nMaxDistanceToNext);
			NeedUpdate();
		}
		// 如果页面移动超过多少距离则翻页
		int  _NextPageWhileMove(int _nMaxMoveDistance)
		{
			if (_m_nThisPage >= 0)
			{
				CControlUI* pContrl = GetItemAt(_m_nThisPage);
				CDuiRect rcClient = GetPos();
				CDuiRect rcPage= pContrl->GetPos();
				int nX = 0, nY = 0;
				_AlignPoint(rcPage, rcClient, nX, nY);
				if (abs(nX) > _nMaxMoveDistance)
				{
					if (nX < 0)
						return _m_nThisPage <= 0 ? _m_nThisPage : _m_nThisPage - 1;
					else if (nX > 0)
						return _m_nThisPage >= GetCount() - 1 ? _m_nThisPage : _m_nThisPage + 1;
				}
				else if (abs(nY) > _nMaxMoveDistance)
				{
					if (nY < 0)
						return _m_nThisPage <= 0 ? _m_nThisPage : _m_nThisPage - 1;
					else if (nY > 0)
						return _m_nThisPage >= GetCount() - 1 ? _m_nThisPage : _m_nThisPage + 1;
				}
			}
			return _m_nThisPage;
		}
		// 对齐点
		bool _AlignPoint(const CDuiRect& _rcPage, const CDuiRect& _rcClient, int& _nxMoveLen, int& _nyMoveLen)
		{
			switch (_m_eAlignPoint)
			{
			case TOP_LEFT:
				if (_rcPage.top == _rcClient.top && _rcPage.left == _rcClient.left)
					return true;
				_nxMoveLen = _rcClient.left - _rcPage.left;
				_nyMoveLen = _rcClient.top - _rcPage.top;
				return false;
			case TOP_RIGHT:
				if (_rcPage.top == _rcClient.top && _rcPage.right == _rcClient.right)
					return true;
				_nxMoveLen = _rcClient.right - _rcPage.right;
				_nyMoveLen = _rcClient.top - _rcPage.top;
				return false;
			case TOP_CENTER:
				if (_rcPage.top == _rcClient.top && _rcPage.left + _rcPage.GetWidth() / 2 == _rcClient.left + _rcClient.GetWidth() / 2)
					return true;
				_nxMoveLen = (_rcClient.left + _rcClient.GetWidth() / 2) - (_rcPage.left + _rcPage.GetWidth() / 2);
				_nyMoveLen = _rcClient.top - _rcPage.top;
				return false;
			case BOTTOM_LEFT:
				if (_rcPage.bottom == _rcClient.bottom && _rcPage.left == _rcClient.left)
					return true;
				_nxMoveLen = _rcClient.left - _rcPage.left;
				_nyMoveLen = _rcClient.bottom - _rcPage.bottom;
				return false;
			case BOTTOM_RIGHT:
				if (_rcPage.bottom == _rcClient.bottom && _rcPage.right == _rcClient.right)
					return true;
				_nxMoveLen = _rcClient.right - _rcPage.right;
				_nyMoveLen = _rcClient.bottom - _rcPage.bottom;
				return false;
			case BOTTOM_CENTER:
				if (_rcPage.bottom == _rcClient.bottom && _rcPage.left + _rcPage.GetWidth() / 2 == _rcClient.left + _rcClient.GetWidth() / 2)
					return true;
				_nxMoveLen = (_rcClient.left + _rcClient.GetWidth() / 2) - (_rcPage.left + _rcPage.GetWidth() / 2);
				_nyMoveLen = _rcClient.bottom - _rcPage.bottom;
				return false;
			case LEFT_CENTER:
				if (_rcPage.left == _rcClient.left && _rcPage.top + _rcPage.GetHeight() / 2 == _rcClient.top + _rcClient.GetHeight() / 2)
					return true;
				_nxMoveLen = _rcClient.left - _rcPage.left;
				_nyMoveLen = (_rcClient.top + _rcClient.GetHeight() / 2) - (_rcPage.top + _rcPage.GetHeight() / 2);
				return false;
			case RIGHT_CENTER:
				if (_rcPage.right == _rcClient.right && _rcPage.top + _rcPage.GetHeight() / 2 == _rcClient.top + _rcClient.GetHeight() / 2)
					return true;
				_nxMoveLen = _rcClient.right - _rcPage.right;
				_nyMoveLen = (_rcClient.top + _rcClient.GetHeight() / 2) - (_rcPage.top + _rcPage.GetHeight() / 2);
				return false;
			case CENTER:
				if (_rcPage.left + _rcPage.GetWidth() / 2 == _rcClient.left + _rcClient.GetWidth() / 2 && _rcPage.top + _rcPage.GetHeight() / 2 == _rcClient.top + _rcClient.GetHeight() / 2)
					return true;
				_nxMoveLen = (_rcClient.left + _rcClient.GetWidth() / 2) - (_rcPage.left + _rcPage.GetWidth() / 2);
				_nyMoveLen = (_rcClient.top + _rcClient.GetHeight() / 2) - (_rcPage.top + _rcPage.GetHeight() / 2);
				return false;
			}
			return false;
		}
	protected:
		int					_m_nXPos;				// 保存容器页面的X偏移量
		int					_m_nYPos;				// 保存容器页面的Y偏移量
		int					_m_nThisPage;			// 保存当前显示的页面
		int					_m_nNextPage;			// 保存下一页面
		int					_m_nSpeed;				// 自动移动的速度
		int					_m_nDefautPage;			// 默认的页面
		bool				_m_bAutoMove;			// 是否自动移动到合适的位置
		int					_m_nMaxDistanceToNext;	// 移动多少距离后翻页
		ALIGN_POINT			_m_eAlignPoint;			// 自动移动时对齐到哪一个点
		DWORD				_m_dwUpdateTime;		// 上次更新时间
		RECT				_m_rcOver;				// 允许越过对齐点的像素
		bool				_m_bOverBorder;			// 是否允许越过容器器边界
		bool				_m_bSlide;				// 是否开始滑动页面
		bool				_m_bMoveHor;			// 允许水平移动
		bool				_m_bMoveVer;			// 允许垂直移动
	};

	class CSlideHorizontalLayoutUI : public CSlideContainerBase<CHorizontalLayoutUI>
	{
	public:
		CSlideHorizontalLayoutUI(){_m_bMoveVer = false;}
		virtual ~CSlideHorizontalLayoutUI(){}
		LPCTSTR GetClass() const{return DUI_CTR_SLIDEHORIZONTALLAYOUT;}
		LPVOID GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CSlideHorizontalLayoutUI*>(this);return __super::GetInterface(pstrName);}
	};

	class CSlideVerticalLayoutUI : public CSlideContainerBase<CVerticalLayoutUI>
	{
	public:
		CSlideVerticalLayoutUI(){_m_bMoveHor = false;}
		virtual ~CSlideVerticalLayoutUI(){}
		LPCTSTR GetClass() const{return DUI_CTR_SLIDEVERTICALLAYOUT;}
		LPVOID GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CSlideVerticalLayoutUI*>(this);return __super::GetInterface(pstrName);}
	protected:
		virtual bool _IsEnableSlideHorizontal(){return false;}
	};	

	class CSlideContainerItemUI : public CContainerUI
	{
	public:
		CSlideContainerItemUI()
			: _m_bEnableMove(true)
			, _m_nXPos(0)
			, _m_nYPos(0)
		{
			_m_rcEnableMove.left = 1;
			_m_rcEnableMove.right = 1;
			_m_rcEnableMove.top = 1;
			_m_rcEnableMove.bottom = 1;
		}
		virtual ~CSlideContainerItemUI(){}
		LPCTSTR GetClass() const{return DUI_CTR_SLIDECONTAINERITEM;}
		LPVOID GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CSlideContainerItemUI*>(this);return __super::GetInterface(pstrName);}
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("move")) == 0)
				_m_bEnableMove = _tcscmp(pstrValue, _T("true")) == 0;
			else if (_tcscmp(pstrName, _T("moveleft")) == 0)
				_m_rcEnableMove.left = _tcscmp(pstrValue, _T("true")) == 0 ? 1 : 0;
			else if (_tcscmp(pstrName, _T("moveright")) == 0)
				_m_rcEnableMove.right = _tcscmp(pstrValue, _T("true")) == 0 ? 1 : 0;
			else if (_tcscmp(pstrName, _T("moveup")) == 0)
				_m_rcEnableMove.top = _tcscmp(pstrValue, _T("true")) == 0 ? 1 : 0;
			else if (_tcscmp(pstrName, _T("movedown")) == 0)
				_m_rcEnableMove.bottom = _tcscmp(pstrValue, _T("true")) == 0 ? 1 : 0;
			else
				__super::SetAttribute(pstrName, pstrValue);
		}
		void DoEvent(TEventUI& event)
		{
			if (event.Type > UIEVENT_SLIDE__FIRST && event.Type < UIEVENT_SLIDE__LAST)
			{
				if (!_m_bEnableMove) return;
				switch (event.Type)
				{
				case UIEVENT_SLIDEBEGIN:
					_m_nXPos = 0;
					_m_nYPos = 0;
					break;
				case UIEVENT_SLIDEHORIZONTAL:
					{
						int nPos = _m_nXPos + -static_cast<int>(event.wParam);
						if (	_m_rcEnableMove.left == 0 && nPos < 0
							||	_m_rcEnableMove.right == 0 && nPos > 0)
						{
							event.wParam = _m_nXPos;
							_m_nXPos = 0;
							break;
						}
						_m_nXPos = nPos;
					}
					break;
				case UIEVENT_SLIDEVERTICAL:
					{
						int nPos = _m_nYPos + -static_cast<int>(event.lParam);
						if (	_m_rcEnableMove.top == 0 && nPos < 0
							||	_m_rcEnableMove.bottom == 0 && nPos > 0)
						{
							event.wParam = _m_nYPos;
							_m_nYPos = 0;
							break;
						}
						_m_nYPos = nPos;
					}
					break;
				}
			}
			__super::DoEvent(event);
		}
	protected:
		RECT	_m_rcEnableMove;
		bool	_m_bEnableMove;
		int		_m_nXPos;
		int		_m_nYPos;
	};
}

#endif