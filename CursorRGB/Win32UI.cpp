#include "Win32UI.h"
#include <iostream>
#include <sstream>


#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
            ( std::ostringstream() << std::dec << x ) ).str()

Win32UI::Win32UI():
	hWindow_(NULL),
	hWindowThread_(NULL),
	hStaticText_(NULL),
	dwWindowThreadID_(0){

 }

Win32UI::~Win32UI(){

}

void Win32UI::start(){

	hWindowThread_ = CreateThread( 
					  NULL,                           // default security attributes
            0,                              // use default stack size  
            Win32UI::windowThreadFunc,      // thread function name
            this,										        // argument to thread function 
            0,                              // use default creation flags 
						&dwWindowThreadID_);            // returns the thread identifier 

	if(!hWindowThread_){
		   MessageBox(NULL,
            _T("Failed to create window UI thread"),
            _T("Cursor RGB App"),
            NULL);	
	}


}

void Win32UI::cleanup(){


}

DWORD WINAPI Win32UI::windowThreadFunc(LPVOID lpParam ){

  Win32UI* win32UI = (Win32UI*)lpParam;

	if(!win32UI->setupWindow())
		return 0;

	if(!win32UI->constructWindow())
		return 0;

	win32UI->messageHandler(); // Runs indefinately.
}


void Win32UI::onPaintMessage(HWND hWnd, POINT point){

	  LOG(LS_INFO)<<"onPaintMessage: "<<hWnd<<" "<<point.x<<point.y;
		std::string pointStr;
	  std::wstring stemp;
		LPCWSTR sw;

		pointStr = SSTR( "x: " << point.x << ", y: " << point.y);
		LOG(LS_INFO)<<"WINDOW point: "<<pointStr;

		stemp = std::wstring(pointStr.begin(), pointStr.end());
		sw = stemp.c_str();

		SetWindowText( hStaticText_, sw);
}

void Win32UI::messageHandler(){

	/* start message loop */
	MSG msg;
  
	while (GetMessage(&msg, NULL, 0, 0))
   {
			
		  if (msg.message == WM_MOUSE_MESSAGE)         
				{
					POINT* point = reinterpret_cast<POINT*>(msg.lParam);
					LOG(LS_INFO)<<"+WM_MOUSE_MESSAGE"<<", x: "<<point->x<<", y: "<<point->y;
					onPaintMessage(GetWindowHandle(), *point);
					delete point;
			}
       
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
    wcex_.hCursor        = LoadCursor(NULL, IDC_ARROW);
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


		hStaticText_ =  CreateWindow(L"Static",
                                      L"Type in the edit box to change the title",
																			WS_CHILD | WS_VISIBLE | SS_LEFT, 
																			40,40, 640,20,
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
   
		
    switch (message)
    {

		case WM_COMMAND:
			
				LOG(LS_INFO)<<"+WM_COMMAND";
				
				break;

    case WM_PAINT:
        
				
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}