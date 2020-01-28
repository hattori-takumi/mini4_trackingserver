// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainFrm.h"

#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"
#include "UdpSocket.h"
#include "IpEndpointName.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#define TIMER_ID	(0)


static const COLORREF m_TargetColor[] = {
	RGB(255, 255,   0),
	RGB(255,   0, 255),
	RGB(255,   0,   0),
	RGB(  0, 255, 255),
	RGB(  0, 255,   0),
	RGB(  0,   0, 255),
};
static const int m_cTargetColor = sizeof(m_TargetColor) / sizeof(m_TargetColor[0]);

void sendOSCMessage(double x1, double y1, double x2, double y2)
{
	// Set IPAddress and Port
	const std::string ipAddress = "127.0.0.1";
	const int port = 50000;

	UdpTransmitSocket transmitSocket(IpEndpointName(ipAddress.c_str(), port));
	//Buffer
	char buffer[6144];
	osc::OutboundPacketStream p(buffer, 6144);
	p << osc::BeginBundleImmediate
		//Head
		<< osc::BeginMessage("/machine1") << x1 << y1 << osc::EndMessage
		<< osc::BeginMessage("/machine2") << x2 << y2 << osc::EndMessage
		<< osc::EndBundle;
	transmitSocket.Send(p.Data(), p.Size());
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}


BOOL CMainFrame::OnIdle()
{
	return FALSE;
}


void CMainFrame::OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu)
{
	SetMsgHandled(FALSE);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_ScreenSize.cx = 1024;
	m_ScreenSize.cy = 768;
	m_ClientArea.Initialize(m_hWnd, m_hWndStatusBar, m_ScreenSize);
	m_ScreenRect.left = 0;
	m_ScreenRect.top = 0;
	m_ScreenRect.right = m_ScreenSize.cx;
	m_ScreenRect.bottom = m_ScreenSize.cy;

	ZeroMemory(&m_RecordParam, sizeof(m_RecordParam));
	M4SDK_GetMarkerPitch(&m_RecordParam.MarkerPitch.cx, &m_RecordParam.MarkerPitch.cy);
	InitializeCriticalSection(&m_RecordParam.Section);

	m_View_Ratio.cx = m_ScreenSize.cx / m_RecordParam.MarkerPitch.cx / M4SDK_CAMERA_COUNT;
	m_View_Ratio.cy = m_ScreenSize.cy / m_RecordParam.MarkerPitch.cy;
	m_View_Target_Radius.cx = 20 * m_View_Ratio.cx;
	m_View_Target_Radius.cy = 20 * m_View_Ratio.cy;

	for (int iTarget = 0; iTarget < M4SDK_TARGET_COUNT; iTarget++)
	{
		m_hBrush[iTarget] = CreateSolidBrush(m_TargetColor[iTarget % m_cTargetColor]);
	}

	SendNotifyMessage(WM_COMMAND, ID_GAME_STOP);
	SendNotifyMessage(WM_COMMAND, ID_RECORD_STOP);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	// 16msごとに画面を更新する 
	SetTimer(TIMER_ID, 16);
	return 0;
}


void CMainFrame::OnDestroy(void)
{
	KillTimer(TIMER_ID);

	SendNotifyMessage(WM_COMMAND, ID_GAME_STOP);
	SendNotifyMessage(WM_COMMAND, ID_RECORD_STOP);

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	DeleteCriticalSection(&m_RecordParam.Section);

	SetMsgHandled(FALSE);
}


void CMainFrame::OnPaint(CDCHandle)
{
	m_ClientArea.Update();
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	M4TARGETSTATUS status[M4SDK_TARGET_COUNT];

	HPEN hPenWhite = (HPEN)GetStockObject(WHITE_PEN);
	HBRUSH hBrushNull = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hBrushWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// クライアント領域を更新する 
	m_ClientArea.ClearImage();

	double x1 = 0;
	double y1 = 0;
	double x2 = 0;
	double y2 = 0;

	for (int iTarget = 0; iTarget < M4SDK_TARGET_COUNT; iTarget++)
	{
		LPM4TARGETSTATUS pStatus = &status[iTarget];

		if (M4SDK_GetTargetStatus(iTarget, pStatus, 0) != M4Result_OK)
			continue;

		if (pStatus->State == M4TargetState_Tracking)
		{
			m_ClientArea.DrawCircle(
				pStatus->RawPosition.x * m_View_Ratio.cx, pStatus->RawPosition.y * m_View_Ratio.cy,
				m_View_Target_Radius.cx, m_View_Target_Radius.cy,
				hPenWhite, hBrushNull);
			m_ClientArea.DrawCircle(
				pStatus->Position.x * m_View_Ratio.cx, pStatus->Position.y * m_View_Ratio.cy,
				m_View_Target_Radius.cx, m_View_Target_Radius.cy,
				hPenWhite, m_hBrush[iTarget]);
		}

		if (iTarget == 0)
		{
			x1 = pStatus->Position.x;
			y1 = pStatus->Position.y;
			if (M4SDK_TARGET_COUNT == 0)
				sendOSCMessage(x1, y1, NULL, NULL);
			continue;
		}
		else if (iTarget == 1)
		{
			x2 = pStatus->Position.x;
			y2 = pStatus->Position.y;
			sendOSCMessage(x1, y1, x2, y2);
		}

	}
	Invalidate(FALSE);
}


LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}


void CMainFrame::OnGameStart(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	M4SDK_StartTracking();
	UISetCheck(ID_GAME_START, TRUE);
	UISetCheck(ID_GAME_STOP, FALSE);
	UIEnable(ID_GAME_START, FALSE);
	UIEnable(ID_GAME_STOP, TRUE);
}


void CMainFrame::OnGameStop(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	M4SDK_StopTracking();
	UISetCheck(ID_GAME_START, FALSE);
	UISetCheck(ID_GAME_STOP, TRUE);
	UIEnable(ID_GAME_START, TRUE);
	UIEnable(ID_GAME_STOP, FALSE);
}


void CMainFrame::OnRecordStart(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	WCHAR szFilePath[MAX_PATH];

	EnterCriticalSection(&m_RecordParam.Section);
	if (m_RecordParam.fp == NULL)
	{
		if (M4SDK_GetCourseFile(0, szFilePath, MAX_PATH) != M4Result_OK)
			fopen_s(&m_RecordParam.fp, "M4Record.bin", "wb");
		else
			_wfopen_s(&m_RecordParam.fp, szFilePath, L"wb");
	}
	LeaveCriticalSection(&m_RecordParam.Section);

	M4SDK_SetPredictionFilter(PredictionStatic, &m_RecordParam);

	UISetCheck(ID_RECORD_START, TRUE);
	UISetCheck(ID_RECORD_STOP, FALSE);
	UIEnable(ID_RECORD_START, FALSE);
	UIEnable(ID_RECORD_STOP, TRUE);
}


void CMainFrame::OnRecordStop(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	M4SDK_SetPredictionFilter(NULL, NULL);

	EnterCriticalSection(&m_RecordParam.Section);
	if (m_RecordParam.fp)
	{
		fclose(m_RecordParam.fp);
		m_RecordParam.fp = NULL;
	}
	LeaveCriticalSection(&m_RecordParam.Section);

	UISetCheck(ID_RECORD_START, FALSE);
	UISetCheck(ID_RECORD_STOP, TRUE);
	UIEnable(ID_RECORD_START, TRUE);
	UIEnable(ID_RECORD_STOP, FALSE);
}


void CMainFrame::PredictionStatic(ULONG TargetIndex, LPM4TARGETSTATUS pFilterIn, LPM4TARGETSTATUS pFilterOut, LPVOID pArg)
{
	SRecordParam* pParam = (SRecordParam*)pArg;
	if (pParam)
	{
		EnterCriticalSection(&pParam->Section);
		if (pParam->fp)
		{
			uint32_t padding = 0;
			fwrite(&pFilterIn->TimeStamp, sizeof(pFilterIn->TimeStamp), 1, pParam->fp);
			fwrite(&pFilterIn->State, sizeof(pFilterIn->State), 1, pParam->fp);
			fwrite(&padding, sizeof(padding), 1, pParam->fp);
			fwrite(&pFilterIn->RawPosition, sizeof(pFilterIn->RawPosition), 1, pParam->fp);
		}
		LeaveCriticalSection(&pParam->Section);
	}
}
