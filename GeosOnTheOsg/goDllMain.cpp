#include <Windows.h>
#include "detours.h"

#pragma comment(lib, "./detours.lib")

HMODULE (WINAPI *TrueLoadLibraryA)(__in LPCSTR lpFileName) = LoadLibraryA;
HMODULE (WINAPI *TrueLoadLibraryW)(__in LPCWSTR lpFileName) = LoadLibraryW;


HMODULE WINAPI MyLoadLibraryA(__in LPCSTR lpFileName)
{
	HMODULE hModule = TrueLoadLibraryA(lpFileName);
	void* proMain = GetProcAddress(hModule, "DataProviderPluginMain");
	bool bCheck = false;


	if(proMain)
	{
		size_t len = strlen(lpFileName);
		LPCSTR cCheck = lpFileName + len +1;
		int iC = strcmp(cCheck, "<:>CheckPlugin");

		if(iC == 0)
		{
			return hModule;
		}
		else
		{
			FreeLibrary(hModule);
		}
	}

	return hModule;
}

HMODULE WINAPI MyLoadLibraryW(__in LPCWSTR lpFileName)
{
	HMODULE hModule = TrueLoadLibraryW(lpFileName);
	void* proMain = GetProcAddress(hModule, "DataProviderPluginMain");
	bool bCheck = false;

	if(proMain)
	{
		size_t len = wcslen(lpFileName);
		LPCWSTR cCheck = lpFileName + len +1;
		int iC = wcscmp(cCheck, L"<:>CheckPlugin");

		if(iC == 0)
		{
			return hModule;
		}
		else
		{
			FreeLibrary(hModule);
		}
	}

	return hModule;
	return hModule;
}

BOOLEAN WINAPI DllMain( IN HINSTANCE hDllHandle, 
	IN DWORD     nReason, 
	IN LPVOID    Reserved )
{
	//LONG error;
	//(void)hinst;
	//(void)reserved;

	if (nReason == DLL_PROCESS_ATTACH) {
		//DetourRestoreAfterWith();

		//printf("simple.dll: Starting.\n");
		//fflush(stdout);

		//DetourTransactionBegin();
		//DetourUpdateThread(GetCurrentThread());
		//DetourAttach(&(PVOID&)TrueLoadLibraryA, MyLoadLibraryA);
		//DetourAttach(&(PVOID&)TrueLoadLibraryW, MyLoadLibraryW);
		//error = DetourTransactionCommit();

		/*if (error == NO_ERROR) {
		printf("simple.dll: Detoured SleepEx().\n");
		}
		else {
		printf("simple.dll: Error detouring SleepEx(): %d\n", error);
		}*/
	}
	else if (nReason == DLL_PROCESS_DETACH) {
		//DetourTransactionBegin();
		//DetourUpdateThread(GetCurrentThread());
		//DetourDetach(&(PVOID&)TrueLoadLibraryA, MyLoadLibraryA);
		//DetourDetach(&(PVOID&)TrueLoadLibraryW, MyLoadLibraryW);
		//error = DetourTransactionCommit();

		/*printf("simple.dll: Removed SleepEx() (result=%d), slept %d ticks.\n",
			error, dwSlept);
		fflush(stdout);*/
	}
	return TRUE;
}