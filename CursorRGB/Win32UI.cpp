#include "Win32UI.h"
#include <iostream>
#include <sstream>


#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
	( std::ostringstream() << std::dec << x ) ).str()

HWND Win32UI::hStaticText_ = NULL;
DWORD Win32UI::dwMouseHookThreadId_ = 0;
HDC		Win32UI::hScreenDC_ = NULL;


Win32UI::Win32UI():
	dwWindowThreadID_(0){

}

Win32UI::~Win32UI(){

}

BOOLEAN Win32UI::start(){

	
	if(!setupWindow())
		return FALSE;

	if(!constructWindow())
		return FALSE;

	return TRUE;
}

void Win32UI::cleanup(){


}

void Win32UI::messageHandler(){

	/* start message loop */
	MSG msg;

	while (GetMessage(&msg, hWindow_, 0, 0))
	{

			TranslateMessage(&msg); 
			DispatchMessage(&msg);
	
	}
}
bool Win32UI::setupWindow(){

	wcex_.cbSize				 = sizeof(WNDCLASSEX);
	wcex_.style          = CS_HREDRAW | CS_VREDRAW;
	wcex_.lpfnWndProc    = Win32UI::wndProc;
	wcex_.cbClsExtra     = 0;
	wcex_.cbWndExtra     = 0;
	wcex_.hInstance      = GetModuleHandle(NULL);
	wcex_.hIcon          = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APPLICATION));
	wcex_.hCursor        = LoadCursor(GetModuleHandle(NULL), IDC_ARROW);
	wcex_.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex_.lpszMenuName   = NULL;
	wcex_.lpszClassName  = szWindowClass;
	wcex_.hIconSm        = LoadIcon(wcex_.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex_))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Cursor RGB App"),
			NULL);

		return FALSE;
	}

	return TRUE;
}

bool Win32UI::constructWindow(){

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hWindow_ = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		300, 100,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
		);

	if (!hWindow_)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Cursor RGB"),
			NULL);

		return FALSE;
	}

	if(hStaticText_ == NULL)
		hStaticText_ =  CreateWindow(L"Static",
		L"Type in the edit box to change the title",
		WS_CHILD | WS_VISIBLE | SS_LEFT, 
		25,25, 640,20,
		hWindow_, NULL, GetModuleHandle(NULL), NULL);


	if (!hStaticText_){

		MessageBox(NULL,
			_T("Call to Create Static Text failed!"),
			_T("Cursor RGB"),
			NULL);

		return FALSE;

	}



	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWindow_, SW_SHOW);

	UpdateWindow(hWindow_);

	return TRUE;
}




//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK Win32UI::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255)); // white color

	switch (message)
	{

	case WM_CTLCOLORSTATIC:
		{
			if (hStaticText_ == (HWND)lParam)
			{
				HDC hdcStatic = (HDC) wParam;
				SetTextColor(hdcStatic, RGB(0,0,0));
				SetBkColor(hdcStatic, RGB(255,255,255));
				return (INT_PTR)hBrush;
			}
		}

	case WM_DESTROY:
	
		if(dwMouseHookThreadId_!=0)
			PostThreadMessage(dwMouseHookThreadId_, WM_STOP_HOOK, WM_COMMAND, NULL);
		else
			LOG(LS_WARNING)<<"UNABLE TO SEND STOP HOOK MSG, NULL Thread ID";

		PostQuitMessage(0);
		break;

	case WM_MOUSE_MESSAGE:
	{
			POINT* point = reinterpret_cast<POINT*>(lParam);
			LOG(LS_VERBOSE)<<"+WM_MOUSE_MESSAGE"<<", x: "<<point->x<<", y: "<<point->y;
			
			std::wstring stemp;
			LPCWSTR sw;
			COLORREF color;
			BYTE R, G, B;

			hScreenDC_ = GetDC(NULL);
			// TBD: Perform sanity check for point, should not be
			// greater than the screen resoulution and less than 0.
			color = GetPixel(hScreenDC_, point->x, point->y);
			R = GetRValue(color);
			G = GetGValue(color);
			B = GetBValue(color);

			char rgbString[20];
			sprintf(rgbString,"R:%d, G:%d, B:%d",R,G,B);
			std::string pointStr(rgbString);

			LOG(LS_VERBOSE)<<"WINDOW point: "<<pointStr;
			stemp = std::wstring(pointStr.begin(), pointStr.end());
			sw = stemp.c_str();
			SetWindowText(hStaticText_, sw);
			ReleaseDC(NULL, hScreenDC_);
		}
			break;
	
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}