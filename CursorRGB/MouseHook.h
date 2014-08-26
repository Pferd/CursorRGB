#ifndef _MOUSE_HOOK
#define _MOUSE_HOOK

#include "Windows.h"
#include <tchar.h> 
#include "Defines.h"

#include "talk/base/logging.h"

class MouseHook{

public:
	 MouseHook();
	~MouseHook();
	 MouseHook* GetInstance();
	 HHOOK	GetMSHookHndl(){return hMouseHook_;}
	 HANDLE GetMouseHookThread(){return hMouseThread_;}
	 DWORD	GetMouseHookThreadID(){return dwMouseThreadID_;}
	 VOID		SetWindowID(DWORD windowThreadID){dwUIThreadID_ = windowThreadID;}
	 VOID		SetWindowHandle(HWND windowHandle){windowHandle_ = windowHandle;}
	 VOID		start();
private:	 
	 static MouseHook* createInstance();
	 bool setupHook();
	 static LRESULT CALLBACK lowMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	 static DWORD WINAPI mouseThreadFunc( LPVOID lpParam);
	 void cleanup();
	 void OnMouseMessage(MSLLHOOKSTRUCT*);

	 // Member Variables //
	 HHOOK hMouseHook_;
	 static HWND windowHandle_;
	 HANDLE hMouseThread_;
	 DWORD	dwMouseThreadID_;
	 HINSTANCE hInstance_;
	 static DWORD dwUIThreadID_; // bad hack!
	 static MouseHook* pMouseHook_;
};

#endif