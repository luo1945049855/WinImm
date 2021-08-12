// dllmain.cpp : Defines the entry point for the DLL application.
#include "UIWnd.h"

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      UIWnd::RegisterUIWndClass(hModule);
      CStatusWnd::RegisterClassW(hModule);
      CCompWnd::RegisterClassW(hModule);
      break;
    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      UIWnd::UnRegisterUIWndClass(hModule);
	  CStatusWnd::UnRegisterClass(hModule);
	  CCompWnd::UnRegisterClass(hModule);
      break;
  }
  return TRUE;
}
