#pragma once
#ifndef __DEFAINE_HPP_2014_03_20
#define __DEFAINE_HPP_2014_03_20


#include <tchar.h>
#include <string>
#include <sstream> // wstringstream


#define DUI_CTR_MUTIBTNLAYOUT					(_T("MutiBtnLayout"))				// 多状态按钮容器
#define DUI_CTR_SLOCKBTNLAYOUT					(_T("SLockBtnLayout"))				// 自锁按钮容器
#define DUI_CTR_SRESETBTNLAYOUT					(_T("SResetBtnLayout"))				// 自复位按钮容器
#define DUI_CTR_HORSLIDEBTNLAYOUT				(_T("HorSlideBtnLayout"))			// 水平拖动按钮容器
#define DUI_CTR_VERSLIDEBTNLAYOUT				(_T("VerSlideBtnLayout"))			// 水平拖动按钮容器
#define DUI_CTR_BTNLAYOUTITEM					(_T("BtnLayoutItem"))				// 按钮窗口ITEM
#define DUI_CTR_METERPOINTER					(_T("MeterPointer"))				// 仪表指针
#define DUI_CTR_ALERT							(_T("AlertPage"))					// 警告窗口
#define DUI_CTR_LAYERED							(_T("Layered"))						// 分层窗口
#define DUI_CTR_CURVE							(_T("Curve"))						// 二维曲线表
#define DUI_CTR_SLIDELIST						(_T("SlideList"))					// 可拖动的列表
#define DUI_CTR_LISTPURECONTAINERELEMENT		(_T("ListPureContainerElement"))	// 纯静的容器列表记录，即不受列表中设置的背景颜色与图片的影响
#define DUI_CTR_SLIDETILELAYOUT					(_T("SlideTileLayout"))				// 可拖动的瓷砖容器
#define DUI_CTR_SLIDEHORIZONTALLAYOUT			(_T("SlideHorizontalLayout"))		// 可拖动的水平容器
#define DUI_CTR_SLIDEVERTICALLAYOUT				(_T("SlideVerticalLayout"))			// 可拖动的垂直容器
#define DUI_CTR_SLIDECONTAINERITEM				(_T("SlideContainerItem"))			// 可拖动容器的子容器，可以设置各方向的拖动限制
#define DUI_CTR_SLIDENUMBERLABEL				(_T("SlideNumberLabel"))			// 可拖动的数字显示
#define DUI_CTR_GIF								(_T("Gif"))							// GIF控件
#define DUI_CTR_GIFBUTTON						(_T("GifButton"))					// GIF按钮控件
#define DUI_CTR_VERTICALLAYOUTEX				(_T("VerticalLayoutEx"))			// 扩展的垂直容器
#define DUI_CTR_HORIZONTALLAYOUTEX				(_T("HorizontalLayoutEx"))			// 扩展的水平容器
#define DUI_CTR_SLIDECONTAINERITEMEX			(_T("SlideContainerItemEx"))		// 扩展的可拖动容器的子容器，可以设置各方向的拖动限制
#define DUI_CTR_PERCENTHORIZONTALLAYOUT			(_T("PercentHorizontalLayout"))		// 对下属控件进行百分比控制的容器
#define DUI_CTR_PERCENTHORIZONTALLAYOUTEX		(_T("PercentHorizontalLayoutEx"))	// 对下属控件进行百分比控制，并且本体可以百分比设置大小的容器
#define DUI_CTR_TABCHAGEBUTTON					(_T("TabChangeButton"))				// 切换TabLayout的Button
#define DUI_CTR_TABCHAGEOPTION					(_T("TabChangeOption"))				// 切换TabLayout的Option
#define DUI_CTR_LISTEX							(_T("ListEx"))						// 扩展的LIST 	
#define DUI_CTR_LISTEXCONTAINERELEMENT			(_T("ListExContainerElement"))		// 扩展的LIST

// Flags for CControlUI::GetControlFlags()
#define UIFLAG_DISLABLESLIDE    0x00100000		// 禁用新添加的滑动功能，使用原始的鼠标操作
#define UIFLAG_ANIMATION		0x00200000		// 接收动画定时器消息

namespace DuiLib
{
	typedef enum SLIDETYPE
	{
		SLIDETYPE_NULL,
		SLIDETYPE_HORIZONTAL,
		SLIDETYPE_VERTICAL,
	};
	typedef enum EVENTTYPE_UI_EX
	{
		UIEVENT_EX__FIRST = UIEVENT__LAST,
		UIEVENT_RBUTTONUP,
		UIEVENT_MBUTTONDOWN,
		UIEVENT_MBUTTONUP,
		UIEVENT_SLIDE__FIRST,
		UIEVENT_SLIDEBEGIN,
		UIEVENT_SLIDEHORIZONTAL,
		UIEVENT_SLIDEVERTICAL,
		UIEVENT_SLIDEEND,
		UIEVENT_SLIDE__LAST,
		UIEVENT_LBUTTONLONGPRESS,
		UIEVENT_ANIMATION,
		UIEVENT_EX__LAST,
	};
	template<typename T_DST, typename T_SRC>
	T_DST xk_cast(T_SRC _src)
	{
#	ifdef UNICODE
		std::wstringstream ss;
#	else
		std::stringstream ss;
#	endif
		ss << _src;
		T_DST dst;
		ss >> dst;
		return dst;
	}
}

#define  DUILIB_ATOI(str){}

#endif