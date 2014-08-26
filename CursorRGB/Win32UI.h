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
		void   messageHandler();
		void   cleanup();
		BOOLEAN	start();
		HWND	 GetWindowHandle(){return hWindow_;}
		void   SetMouseHookThreadID(DWORD dwMouseHookThreadId){dwWindowThreadID_ = dwMouseHookThreadId;}

private:
	 WNDCLASSEX wcex_;
	 HWND hWindow_;
	 static HDC hScreenDC_; // makes it fast
	 static HWND hStaticText_;
	 static DWORD dwMouseHookThreadId_;
	 DWORD  dwWindowThreadID_;
};

#endif