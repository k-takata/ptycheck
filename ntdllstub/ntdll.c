/* Stub for ntdll.dll.
 * Needed for WinXP. */

#include <windows.h>

__declspec(dllexport)
DWORD WINAPI
NtWaitForSingleObject(HANDLE h, BOOLEAN b, PLARGE_INTEGER p)
{
	return 0;
}

__declspec(dllexport)
VOID WINAPI
RtlSetLastWin32Error(DWORD e)
{
}
