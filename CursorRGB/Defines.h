#ifndef _DEFINES
#define	_DEFINES

#define PROJECT_TITLE "CURSOR RGB"
#define	WINDOW_TITLE "CURSOR RGB"

// The main window class name.
static TCHAR szWindowClass[] = _T(PROJECT_TITLE);

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T(WINDOW_TITLE);

#define		WM_WINDOW_INIT		WM_USER+1
#define		WM_WINDOW_DESTROY WM_USER+2
#define		WM_MOUSE_MESSAGE	WM_USER+3
#define		WM_STOP_HOOK		  WM_USER+4

#endif