#pragma once
#ifndef __CONTAINEREX_HPP_2015_01_04
#define __CONTAINEREX_HPP_2015_01_04

#include "../DuiLib/UIlib.h"

#define MAX max

namespace DuiLib
{
	template<typename T>
	class CContainerExBase : public T
	{
	public:
		CContainerExBase()
			: _m_nX(-1)
			, _m_nY(-1)
		{
			ZeroMemory(&_m_szSizeSet, sizeof(SIZE));
		}
		virtual ~CContainerExBase(){}
		LPCTSTR GetClass() const{CDuiString strClass(__super::GetClass()); return strClass + _T("Ex");}
		LPVOID GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CContainerExBase<T>*>(this);return __super::GetInterface(pstrName);}
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("width")) == 0 && _tcschr(pstrValue, '%') != nullptr)
				_m_nX = xk_cast<int>(pstrValue);
			else if (_tcscmp(pstrName, _T("height")) == 0 && _tcschr(pstrValue, '%') != nullptr)
				_m_nY = xk_cast<int>(pstrValue);
			else if (_tcscmp(pstrName, _T("sizeset")) == 0)
			{
				SIZE szSizeSet = { 0 };
				LPTSTR pstr = NULL;
				szSizeSet.cx = _tcstol(pstrValue, &pstr, 10);	ASSERT(pstr);    
				szSizeSet.cy = _tcstol(pstr + 1, &pstr, 10);	ASSERT(pstr);    
				_m_szSizeSet = szSizeSet;
			}
			else
				__super::SetAttribute(pstrName, pstrValue);
		}
		virtual SIZE EstimateSize(SIZE szAvailable)
		{
			DuiLib::CContainerUI* pParent = static_cast<DuiLib::CContainerUI*>(GetParent()->GetInterface(DUI_CTR_CONTAINER));
			if (pParent)
			{
				RECT rcParentInset(pParent->GetInset());
				RECT rcParent(GetParent()->GetPos());
				if (_m_nX != -1)
					m_cxyFixed.cx = static_cast<int>(static_cast<double>(rcParent.right - rcParent.left - rcParentInset.left - rcParentInset.right) * (static_cast<double>(_m_nX) / 100));
				if (_m_nY != -1)
					m_cxyFixed.cy = static_cast<int>(static_cast<double>(rcParent.bottom - rcParent.top - rcParentInset.top - rcParentInset.bottom) * (static_cast<double>(_m_nY) / 100));
				m_cxyFixed.cx += _m_szSizeSet.cx;
				m_cxyFixed.cy += _m_szSizeSet.cy;
			}
			return __super::EstimateSize(szAvailable);
		}
		virtual int GetFixedWidth() const
		{
			int nWidth = 0;
			if (_m_nX != -1)
			{
				DuiLib::CContainerUI* pParent = static_cast<DuiLib::CContainerUI*>(GetParent()->GetInterface(DUI_CTR_CONTAINER));
				if (pParent)
				{
					RECT rcParentInset(pParent->GetInset());
					RECT rcParent(GetParent()->GetPos());
					nWidth = static_cast<int>(static_cast<double>(rcParent.right - rcParent.left - rcParentInset.left - rcParentInset.right) * (static_cast<double>(_m_nX) / 100));
				}
			}
			else
				nWidth = __super::GetFixedWidth();
			return nWidth + _m_szSizeSet.cx;
		}
		virtual int GetFixedHeight() const
		{
			int nHeight = 0;
			if (_m_nY != -1)
			{
				DuiLib::CContainerUI* pParent = static_cast<DuiLib::CContainerUI*>(GetParent()->GetInterface(DUI_CTR_CONTAINER));
				if (pParent)
				{
					RECT rcParentInset(pParent->GetInset());
					RECT rcParent(GetParent()->GetPos());
					nHeight = static_cast<int>(static_cast<double>(rcParent.bottom - rcParent.top - rcParentInset.top - rcParentInset.bottom) * (static_cast<double>(_m_nY) / 100));
				}
			}
			else
				nHeight = __super::GetFixedHeight();
			return nHeight + _m_szSizeSet.cy;
		}
	protected:
		int			_m_nX;
		int			_m_nY;
		SIZE		_m_szSizeSet;		// 对大小的再修饰，在原有大小外再增加宽与高的加运算
	};

	class CPercentHorizontalLayoutUI : public CHorizontalLayoutUI
	{
	public:
		CPercentHorizontalLayoutUI(){}
		virtual ~CPercentHorizontalLayoutUI(){}
		LPCTSTR			GetClass() const{return DUI_CTR_PERCENTHORIZONTALLAYOUT;}
		LPVOID			GetInterface(LPCTSTR pstrName){if( _tcscmp(pstrName, GetClass()) == 0 ) return static_cast<CPercentHorizontalLayoutUI*>(this);return __super::GetInterface(pstrName);}
		void			SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("percent")) == 0)
			{
				int nTemp = 0;
				LPTSTR pstr = (LPTSTR)pstrValue;
				LPCTSTR pstrEnd = pstrValue + _tcslen(pstrValue);
				while (pstr < pstrEnd)
				{
					_m_vecPercent.push_back(_tcstol(pstr, &pstr, 10));	ASSERT(pstr);
					++pstr;
				}
			}
			else
				__super::SetAttribute(pstrName, pstrValue);
		}
		void			SetPos(RECT rc)
		{
			CControlUI::SetPos(rc);
			rc = m_rcItem;

			// Adjust for inset
			rc.left += m_rcInset.left;
			rc.top += m_rcInset.top;
			rc.right -= m_rcInset.right;
			rc.bottom -= m_rcInset.bottom;

			if( m_items.GetSize() == 0) {
				ProcessScrollBar(rc, 0, 0);
				return;
			}

			if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
			if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

			// Determine the width of elements that are sizeable
			SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
			if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
				szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();

			int nAdjustables = 0;
			int cxFixed = 0;
			int nEstimateNum = 0;
			for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
				if( !pControl->IsVisible() ) continue;
				if( pControl->IsFloat() ) continue;
				SIZE sz = _EstimateItemSize(it1, szAvailable, szAvailable);
				if( sz.cx == 0 ) {
					nAdjustables++;
				}
				else {
					if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
					if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
				}
				cxFixed += sz.cx +  pControl->GetPadding().left + pControl->GetPadding().right;
				nEstimateNum++;
			}
			cxFixed += (nEstimateNum - 1) * m_iChildPadding;

			int cxExpand = 0;
			int cxNeeded = 0;
			if( nAdjustables > 0 ) cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);
			// Position the elements
			SIZE szRemaining = szAvailable;
			int iPosX = rc.left;
			if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
				iPosX -= m_pHorizontalScrollBar->GetScrollPos();
			}
			int iAdjustable = 0;
			int cxFixedRemaining = cxFixed;
			for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
				if( !pControl->IsVisible() ) continue;
				if( pControl->IsFloat() ) {
					SetFloatPos(it2);
					continue;
				}
				RECT rcPadding = pControl->GetPadding();
				szRemaining.cx -= rcPadding.left;
				SIZE sz = _EstimateItemSize(it2, szAvailable, szRemaining);
				if( sz.cx == 0 ) {
					iAdjustable++;
					sz.cx = cxExpand;
					// Distribute remaining to last element (usually round-off left-overs)
					if( iAdjustable == nAdjustables ) {
						sz.cx = MAX(0, szRemaining.cx - rcPadding.right - cxFixedRemaining);
					}
					if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
					if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
				}
				else {
					if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
					if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

					cxFixedRemaining -= sz.cx;
				}

				sz.cy = pControl->GetFixedHeight();
				if( sz.cy == 0 ) sz.cy = rc.bottom - rc.top - rcPadding.top - rcPadding.bottom;
				if( sz.cy < 0 ) sz.cy = 0;
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();

				RECT rcCtrl = { iPosX + rcPadding.left, rc.top + rcPadding.top, iPosX + sz.cx + rcPadding.left + rcPadding.right, rc.top + rcPadding.top + sz.cy};
				pControl->SetPos(rcCtrl);
				iPosX += sz.cx + m_iChildPadding + rcPadding.left + rcPadding.right;
				cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
				szRemaining.cx -= sz.cx + m_iChildPadding + rcPadding.right;
			}
			cxNeeded += (nEstimateNum - 1) * m_iChildPadding;

			// Process the scrollbar
			ProcessScrollBar(rc, cxNeeded, 0);
		}
	protected:
		SIZE			_EstimateItemSize(int _nIndex, const SIZE& _szAvailable, const SIZE& _szRemaining)
		{
			SIZE sz={0};
			if (_nIndex < 0 || _nIndex >= GetCount()) return sz;
			CControlUI* pItem = GetItemAt(_nIndex);
			sz = pItem->EstimateSize(_szRemaining);
			if (_nIndex < static_cast<int>(_m_vecPercent.size()))
				sz.cx = _szAvailable.cx * _m_vecPercent[_nIndex] / 100;
			return sz;
		}
	protected:
		std::vector<int>		_m_vecPercent;
	};

	typedef CContainerExBase<CVerticalLayoutUI>				CVerticalLayoutExUI;
	typedef CContainerExBase<CHorizontalLayoutUI>			CHorizontalLayoutExUI;
	typedef CContainerExBase<CSlideContainerItemUI>			CSlideContainerItemExUI;
	typedef CContainerExBase<CPercentHorizontalLayoutUI>	CPercentHorizontalLayoutUIEx;
}

#endif