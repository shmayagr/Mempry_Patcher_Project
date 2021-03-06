// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include"PEDeatails.h"
#include"petchMethods.h"
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		HMODULE currentProccessHandle = GetModuleHandle(NULL);
		PEDeatis *currentProcessDet = new PEDeatis(currentProccessHandle);
		petchMethods::setCurrentProcessDetails(currentProcessDet);
		//BYTE search[] = { 0x08,0x85,0xC0,0x74,0x20,0x68 };
		//BYTE replace[] = { 0x08,0x85,0xC0,0x75,0x20,0x68 };
		BYTE search[] = { 0x01,0x00,0x00,0x75,0x10,0x68 };
		BYTE replace[] = { 0x01,0x00,0x00,0x74,0x10,0x68 };
		//petchMethods::setCurrentProcessDetails(currentProcessDet);
		petchMethods::searcHAndReplace(".text", search, replace, sizeof(search));

		delete currentProcessDet;
	}
	break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

