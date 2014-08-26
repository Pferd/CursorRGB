#include "MouseHook.h"

MouseHook* MouseHook::pMouseHook_ = NULL;
DWORD			 MouseHook::dwUIThreadID_ = 0;
HWND			 MouseHook::windowHandle_ = NULL;


MouseHook::MouseHook()
	:hMouseHook_(NULL),
	hMouseThread_(NULL),
	dwMouseThreadID_(0),
	hInstance_(GetModuleHandle(NULL))
{

}

VOID MouseHook::start(){

	hMouseThread_ = CreateThread( 
		NULL,                           // default security attributes
		0,                              // use default stack size  
		MouseHook::mouseThreadFunc,     // thread function name
		NULL,										        // argument to thread function 
		0,                              // use default creation flags 
		&dwMouseThreadID_);             // returns the thread identifier 

	if(!hMouseThread_){
		MessageBox(NULL,
			_T("Failed to start Mouse Hook Thread"),
			_T("Cursor RGB App"),
			NULL);	
	}

}

MouseHook* MouseHook::createInstance(){

	if(MouseHook::pMouseHook_== NULL)
		pMouseHook_ = new MouseHook();
	else
		return pMouseHook_;
}

MouseHook* MouseHook::GetInstance(){

	if(MouseHook::pMouseHook_!= NULL)
		return MouseHook::pMouseHook_;
	else
		return NULL;

}

MouseHook::~MouseHook(){

	if(pMouseHook_!=NULL)
		delete pMouseHook_;

	if(hMouseThread_!=NULL)
		CloseHandle(hMouseThread_);

}

DWORD WINAPI MouseHook::mouseThreadFunc(LPVOID lpParam ){

	MouseHook* pMouseHook_ = createInstance();

	bool ret = pMouseHook_->setupHook();

	if(!ret)
		return 0;

	/* start message loop */
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{   
		if (msg.message == WM_STOP_HOOK)         
		{
			LOG(LS_INFO)<<"+Got stop hook Req:";
			pMouseHook_->cleanup();
			return 0;
		}

		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}


}

LRESULT CALLBACK MouseHook::lowMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	MSLLHOOKSTRUCT  *pMouseMsg = (MSLLHOOKSTRUCT *)lParam;
	pMouseMsg->flags = wParam;


	if(pMouseHook_== NULL) // No Point in going down, give it to the hook chain.
		return ::CallNextHookEx(pMouseHook_->GetMSHookHndl(), nCode, wParam, lParam);

	// call the on mouse message only on mouse move event.
	if(wParam==WM_MOUSEMOVE){
		LOG(LS_INFO)<<"+MouseMove fired";
		pMouseHook_->OnMouseMessage(pMouseMsg);
	}

	if (nCode < 0 || nCode == HC_NOREMOVE){
		return ::CallNextHookEx(pMouseHook_->GetMSHookHndl(), nCode, wParam, lParam);
	}

	if (lParam & 0x40000000)  
	{
		return ::CallNextHookEx(pMouseHook_->GetMSHookHndl(), nCode, wParam, lParam);
	}

	if((pMouseMsg->flags & LLKHF_INJECTED)){
		return ::CallNextHookEx(pMouseHook_->GetMSHookHndl(), nCode, wParam, lParam);
	}

	return ::CallNextHookEx(pMouseHook_->GetMSHookHndl(), nCode, wParam, lParam);
}


void MouseHook::OnMouseMessage(MSLLHOOKSTRUCT* pMouseHookStruct){

	POINT* pPoint = new POINT(); // release it? // pass ownership.
	memcpy(pPoint, &pMouseHookStruct->pt, sizeof(POINT));

	LOG(LS_INFO)<<"POINT onMouseMessage: "<<pPoint->x<<", "<<pPoint->y<<", to Thread: "<<dwUIThreadID_;
	// post message, so it donest clog the call back handler.
	// Its okay, if the UI is little delayed.
	LOG(LS_INFO)<<"WINDOW HANDLE"<<windowHandle_;
	SendMessage(windowHandle_, WM_MOUSE_MESSAGE, NULL, (LPARAM)pPoint);
}

bool MouseHook::setupHook(){

	hMouseHook_ = SetWindowsHookEx
		(WH_MOUSE_LL, 
		lowMouseHookProc,  
		hInstance_, NULL);

	if(!hMouseHook_)
		return false;
	else
		return true;
}

void MouseHook::cleanup(){

	UnhookWindowsHookEx(hMouseHook_);

}						