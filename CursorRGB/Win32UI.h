#ifndef _WIN32UI
#define _WIN32UI

#include "Windows.h"
#include <tchar.h>

#include "Defines.h"
#include "talk/base/logging.h"

class Win32UI{

	public:
	
	  Win32UI();
	 ~Win32UI();
	 
	  bool	 setupWindow();
		bool	 constructWindow();
		static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static DWORD WINAPI windowThreadFunc( LPVOID lpParam );
		void   messageHandler();
		DWORD  GetWindowThreadID(){return dwWindowThreadID_;}
		HANDLE GetThreadHandle(){return hWindowThread_;}	
		void   cleanup();
		void	 start();
		void   onPaintMessage(HWND hWnd, POINT point);
		HWND	 GetWindowHandle(){return hWindow_;}
		void   SetMouseHookThreadID(DWORD dwMouseHookThreadId){dwWindowThreadID_ = dwMouseHookThreadId;}

private:
	 WNDCLASSEX wcex_;
	 HWND hWindow_;
	 HANDLE hWindowThread_;
	 DWORD dwWindowThreadID_;
	 HDC hScreenDC_;
	 static HWND hStaticText_;
	 static DWORD dwMouseHookThreadId_;
};

#endif