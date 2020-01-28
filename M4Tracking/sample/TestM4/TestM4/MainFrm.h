// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ClientArea.h"
#include <stdint.h>


struct M4SIZE64F
{
	double cx;
	double cy;
};


struct SRecordParam
{
	FILE* fp;
	M4SIZE64F MarkerPitch;
	CRITICAL_SECTION Section;
};

class CMainFrame
	: public CFrameWindowImpl<CMainFrame>
	, public CUpdateUI<CMainFrame>
	, public CMessageFilter
	, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDC_TESTM4)

	CClientArea			m_ClientArea;
	SIZE				m_ScreenSize;
	RECT				m_ScreenRect;
	HBRUSH				m_hBrush[M4SDK_TARGET_COUNT];
	M4SIZE64F			m_View_Target_Radius;
	M4SIZE64F			m_View_Ratio;
	SRecordParam		m_RecordParam;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_GAME_START, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_GAME_STOP, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_RECORD_START, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_RECORD_STOP, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP_EX(CMainFrame)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
		MSG_WM_TIMER(OnTimer)
		COMMAND_ID_HANDLER_EX(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER_EX(ID_GAME_START, OnGameStart)
		COMMAND_ID_HANDLER_EX(ID_GAME_STOP, OnGameStop)
		COMMAND_ID_HANDLER_EX(ID_RECORD_START, OnRecordStart)
		COMMAND_ID_HANDLER_EX(ID_RECORD_STOP, OnRecordStop)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	void OnTimer(UINT_PTR nIDEvent);
	void OnPaint(CDCHandle dc);
	void OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu);

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/);

	void OnGameStart(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnGameStop(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnRecordStart(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnRecordStop(UINT uNotifyCode, int nID, CWindow wndCtl);

	static void PredictionStatic(ULONG TargetIndex, LPM4TARGETSTATUS pFilterIn, LPM4TARGETSTATUS pFilterOut, LPVOID pArg);
};
