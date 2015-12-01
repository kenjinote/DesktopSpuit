#define _WIN32_WINNT 0x0601
#define UNICODE
#include<windows.h>
#include"resource.h"
TCHAR szClassName[]=TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HWND hStatic,hEdit1,hEdit2;
	static BOOL bCapture=FALSE;
	static HCURSOR hCursor;
	static COLORREF color,colorLast=GetSysColor(COLOR_3DFACE);
	static HDC hdcScreen;
	static HFONT hFont;
	TCHAR szBuffer[256];
	switch(msg)
	{
	case WM_CREATE:
		hdcScreen=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
		CreateWindow(TEXT("STATIC"),TEXT("RGB(10進):"),
			WS_VISIBLE|WS_CHILD|SS_RIGHT|SS_CENTERIMAGE,
			74,10,100,28,hWnd,0,((LPCREATESTRUCT)lParam)->hInstance,0);
		hEdit1=CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("EDIT"),0,
			WS_VISIBLE|WS_CHILD|WS_TABSTOP|ES_READONLY,
			174,10,128,28,hWnd,0,((LPCREATESTRUCT)lParam)->hInstance,0);
		CreateWindow(TEXT("STATIC"),TEXT("RGB(16進):"),
			WS_VISIBLE|WS_CHILD|SS_RIGHT|SS_CENTERIMAGE,
			74,50,100,28,hWnd,0,((LPCREATESTRUCT)lParam)->hInstance,0);
		hEdit2=CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("EDIT"),0,
			WS_VISIBLE|WS_CHILD|WS_TABSTOP|ES_READONLY,
			174,50,128,28,hWnd,0,((LPCREATESTRUCT)lParam)->hInstance,0);
		hStatic=CreateWindow(TEXT("STATIC"),0,
			WS_VISIBLE|WS_CHILD|WS_BORDER|SS_NOTIFY,
			10,12,64,64,hWnd,(HMENU)100,((LPCREATESTRUCT)lParam)->hInstance,0);
		hCursor=LoadCursor(((LPCREATESTRUCT)(lParam))->hInstance,
			MAKEINTRESOURCE(IDC_CURSOR1));
		break;
	case WM_DISPLAYCHANGE:
		DeleteDC(hdcScreen);
		hdcScreen=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
		break;
	case WM_COMMAND:
		if(LOWORD(wParam)==100)
		{
			bCapture=TRUE;
			SetCursor(hCursor);
			SetCapture(hWnd);
		}
		else if(LOWORD(wParam)==IDCANCEL)
		{
			DestroyWindow(hWnd);
		}
		break;
	case WM_MOUSEMOVE:
		if(bCapture)
		{
			POINT point;
			GetCursorPos(&point);
			color=GetPixel(hdcScreen,point.x,point.y);
			if(color!=colorLast)
			{
				colorLast=color;
				wsprintf(szBuffer,TEXT("%d,%d,%d"),
					GetRValue(color),GetGValue(color),GetBValue(color));
				SetWindowText(hEdit1,szBuffer);
				wsprintf(szBuffer,TEXT("#%02X%02X%02X"),
					GetRValue(color),GetGValue(color),GetBValue(color));
				SetWindowText(hEdit2,szBuffer);
				InvalidateRect(hStatic,NULL,0);
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		if((HWND)lParam==hStatic)
		{
			SetDCBrushColor((HDC)wParam,colorLast);
			return(INT_PTR)GetStockObject(DC_BRUSH);
		}
		break;
	case WM_LBUTTONUP:
		if(bCapture)
		{
			ReleaseCapture();
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			bCapture=FALSE;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		DeleteDC(hdcScreen);
		PostQuitMessage(0);
		break;
	default:
		return(DefDlgProc(hWnd,msg,wParam,lParam));
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = { 0, WndProc, 0, DLGWINDOWEXTRA, hInstance,
		0, LoadCursor(0, IDC_ARROW), 0, 0, szClassName };
	RegisterClass(&wndclass);
	RECT rect;
	SetRect(&rect, 0, 0, 310, 88);
	AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0);
	HWND hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		szClassName,
		TEXT("スポイトツール"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, 0,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0,
		0,
		hInstance,
		0);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
