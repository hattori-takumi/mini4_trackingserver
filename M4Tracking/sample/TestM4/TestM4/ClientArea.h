#pragma once


class CClientArea
{
protected:

	HWND				m_hWnd;
	HWND				m_hWndStatusBar;
	CDC					m_hDC;
	HBITMAP				m_hBitmap;
	SIZE				m_BitmapSize;

	void SetClientAreaSize(int cx, int cy);

public:

	CClientArea();
	virtual ~CClientArea();

	void Initialize(HWND hWnd, HWND hWndStatusBar, const SIZE& BitmapSize);
	void Update(void);

	void ClearImage(void);
	void SetBitmap(HBITMAP hBitmap, const RECT& BitmapArea);
	void SetPixels(int cPoints, LPPOINT pPoints, COLORREF Color);
	void DrawCircle(double x, double y, double cx, double cy, HPEN hPen, HBRUSH hBrush);
	void DrawCircle(const POINT& Center, const SIZE& Radius, HPEN hPen, HBRUSH hBrush);
	void DrawRectangle(const RECT& Rect, HPEN hPen, HBRUSH hBrush);
	void PrintText(LPRECT pRect, UINT dwFormat, LPCSTR pszFormat, ...);
};
