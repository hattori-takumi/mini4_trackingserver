#include "stdafx.h"
#include "ClientArea.h"


CClientArea::CClientArea()
	: m_hWnd(NULL)
	, m_hWndStatusBar(NULL)
	, m_hBitmap(NULL)
	, m_BitmapSize()
{
}


CClientArea::~CClientArea()
{
}


void CClientArea::Initialize(HWND hWnd, HWND hWndStatusBar, const SIZE& BitmapSize)
{
	m_hWnd = hWnd;
	m_hWndStatusBar = hWndStatusBar;
	m_BitmapSize = BitmapSize;
	SetClientAreaSize(m_BitmapSize.cx, m_BitmapSize.cy);

	m_hDC.CreateCompatibleDC();
	m_hBitmap = CreateCompatibleBitmap(::GetDC(NULL), m_BitmapSize.cx, m_BitmapSize.cy);
	DeleteObject(m_hDC.SelectBitmap(m_hBitmap));
	m_hDC.SelectStockBrush(NULL_BRUSH);
	m_hDC.SetBkMode(TRANSPARENT);

	SetTextColor(m_hDC, RGB(255, 255, 255));
	SetBkColor(m_hDC, RGB(0, 0, 0));
}


void CClientArea::ClearImage(void)
{
	HBRUSH hBrushPrev = m_hDC.SelectStockBrush(BLACK_BRUSH);
	m_hDC.PatBlt(0, 0, m_BitmapSize.cx, m_BitmapSize.cy, PATCOPY);
	m_hDC.SelectBrush(hBrushPrev);
}


void CClientArea::SetBitmap(HBITMAP hBitmap, const RECT& BitmapArea)
{
	HDC hDC = CreateCompatibleDC(NULL);
	HBITMAP hBitmapPrev = (HBITMAP)SelectObject(hDC, hBitmap);
	m_hDC.StretchBlt(0, 0, m_BitmapSize.cx, m_BitmapSize.cy, hDC, BitmapArea.left, BitmapArea.top, BitmapArea.right - BitmapArea.left, BitmapArea.bottom - BitmapArea.top, SRCCOPY);
	SelectObject(hDC, hBitmapPrev);
	DeleteDC(hDC);
}


void CClientArea::DrawCircle(double x, double y, double cx, double cy, HPEN hPen, HBRUSH hBrush)
{
	HPEN hPenPrev = (HPEN)SelectObject(m_hDC, hPen);
	HBRUSH hBrushPrev = m_hDC.SelectBrush(hBrush);
	int x1 = (int)(x - cx);
	int y1 = (int)(y - cy);
	int x2 = (int)(x + cx + 1);
	int y2 = (int)(y + cy + 1);
	m_hDC.Ellipse(x1, y1, x2, y2);
	m_hDC.SelectPen(hPenPrev);
	m_hDC.SelectBrush(hBrushPrev);
}


void CClientArea::DrawCircle(const POINT& Center, const SIZE& Radius, HPEN hPen, HBRUSH hBrush)
{
	HPEN hPenPrev = m_hDC.SelectPen(hPen);
	HBRUSH hBrushPrev = m_hDC.SelectBrush(hBrush);
	int x1 = Center.x - Radius.cx;
	int y1 = Center.y - Radius.cy;
	int x2 = Center.x + Radius.cx + 1;
	int y2 = Center.y + Radius.cy + 1;
	m_hDC.Ellipse(x1, y1, x2, y2);
	m_hDC.SelectPen(hPenPrev);
	m_hDC.SelectBrush(hBrushPrev);
}


void CClientArea::DrawRectangle(const RECT& Rect, HPEN hPen, HBRUSH hBrush)
{
	HPEN hPenPrev = m_hDC.SelectPen(hPen);
	HBRUSH hBrushPrev = m_hDC.SelectBrush(hBrush);
	m_hDC.Rectangle(&Rect);
	m_hDC.SelectPen(hPenPrev);
	m_hDC.SelectBrush(hBrushPrev);
}


void CClientArea::SetPixels(int cPoints, LPPOINT pPoints, COLORREF Color)
{
	for (int i = 0; i < cPoints; i++)
		SetPixel(m_hDC, pPoints[i].x, pPoints[i].y, Color);
}


void CClientArea::PrintText(LPRECT pRect, UINT dwFormat, LPCSTR pszFormat, ...)
{
	const int lenBuf = 512;
	char szBuf[lenBuf];

	va_list	argp;
	va_start(argp, pszFormat);
	vsprintf_s(szBuf, lenBuf, pszFormat, argp);
	va_end(argp);

	DrawTextA(m_hDC, szBuf, -1, pRect, dwFormat);
}


void CClientArea::SetClientAreaSize(int cx, int cy)
{
	SIZE sizeStatusBar = { 0, 0 };
	if (::IsWindowVisible(m_hWndStatusBar))
	{
		RECT rcStatusBar;
		GetWindowRect(m_hWndStatusBar, &rcStatusBar);
		sizeStatusBar.cx = rcStatusBar.right - rcStatusBar.left;
		sizeStatusBar.cy = rcStatusBar.bottom - rcStatusBar.top;
	}

	RECT rcFrameWindow;
	{
		BOOL bMainMenu = TRUE;
		LONG lMainStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		LONG lMainStyleEx = GetWindowLong(m_hWnd, GWL_EXSTYLE);

		rcFrameWindow.left = 0;
		rcFrameWindow.top = 0;
		rcFrameWindow.right = cx;
		rcFrameWindow.bottom = cy + sizeStatusBar.cy;
		AdjustWindowRectEx(&rcFrameWindow, lMainStyle, bMainMenu, lMainStyleEx);
	}

	int x = rcFrameWindow.left;
	int y = rcFrameWindow.top;
	int w = rcFrameWindow.right - rcFrameWindow.left;
	int h = rcFrameWindow.bottom - rcFrameWindow.top;
	SetWindowPos(m_hWnd, NULL, x, y, w, h, SWP_NOMOVE | SWP_NOZORDER);
}


void CClientArea::Update(void)
{
	if (m_hBitmap)
	{
		RECT rcClient;
		GetClientRect(m_hWnd, &rcClient);

		CPaintDC dc(m_hWnd);
		dc.StretchBlt(0, 0, rcClient.right, rcClient.bottom, m_hDC, 0, 0, m_BitmapSize.cx, m_BitmapSize.cy, SRCCOPY);
	}
}
