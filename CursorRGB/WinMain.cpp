/// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "Defines.h"

#include "Win32UI.h"
#include "MouseHook.h"

#include "talk\base\stream.h"
#include "talk\base\pathutils.h"
#include "talk\base\logging.h"


talk_base::StreamInterface* InitLogStream(char *logPrefix);
BOOLEAN SetupLogging();

HINSTANCE hInst;


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
   
    /// Store instance handle in our global variable
	  hInst = hInstance; 
	
		/// Setup the Logging Framework (from WebRTC native stack).
		SetupLogging();
		
		/// setup the UI and Hook instance
		Win32UI win32UI;
		MouseHook mouseHook;
		
		/// start the win32UI
		win32UI.start();

		/// Exchage the information for the
		/// hook to pass the information.

		mouseHook.SetWindowID(win32UI.GetWindowThreadID());
		mouseHook.SetWindowHandle(win32UI.GetWindowHandle());
		
		/// Start the Hook thread, this will start pumping
		//// mouse messsages to windows UI thread.

		mouseHook.start();

		/// Wait for the window thread to terminate.
		/// Upon the window thread termination close
		/// the hook and exit. Destructors should take 
		/// care of resource de-allocation.

		WaitForSingleObject(win32UI.GetThreadHandle(), INFINITE);
		
		return 1;
}



talk_base::StreamInterface* InitLogStream(char *logPrefix)
{
   SYSTEMTIME sysTime;
   GetSystemTime(&sysTime);

   char fName[256];

   sprintf(fName, "%s%d_%d_%d%s", logPrefix, 
           sysTime.wHour+5,sysTime.wMinute+30,sysTime.wSecond,".log");

   talk_base::Pathname logfilepath = talk_base::Pathname(fName);
   talk_base::StreamInterface* logStream = NULL;
   
   logStream = (talk_base::StreamInterface* )talk_base::Filesystem::OpenFile( logfilepath, "w");

   talk_base::LogMessage::LogThreads();
   talk_base::LogMessage::LogTimestamps();

   talk_base::LoggingSeverity logLvl = talk_base::LS_INFO;
   talk_base::LogMessage::LogContext(logLvl);
   talk_base::LogMessage::LogThreads();
   talk_base::LogMessage::LogTimestamps();
   return logStream;
}


BOOLEAN SetupLogging(){
	
	 talk_base::LogMessage::LogToDebug(talk_base::LS_INFO);
	 talk_base::StreamInterface* stream = InitLogStream( "c:\\Logs\\cursorRGB" );

    if (stream) {

      talk_base::LogMessage::LogToStream(stream, talk_base::LS_INFO);
      LOG(LS_VERBOSE) << "Logging Init - VERBOSE";
      LOG(LS_INFO) << "Logging Init - INFO";
      stream->Flush();
			return TRUE;

    } else {

			MessageBox((HWND)hInst, L"Debug log setup ERROR",L"Failure", MB_ICONERROR);
      return FALSE;
    }
  
}
