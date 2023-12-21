#include <iostream>
#include <Windows.h>



BOOL WINAPI Dllmain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{

	if(dwReason == DLL_PROCESS_ATTACH)
	{
	  printf("rayo");

	};

	if(dwReason == DLL_PROCESS_DETACH)
	{
	  printf("adios");
	};

  return TRUE;
}
