// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include"PEDeatails.h"
#include"petchMethods.h"
petchMethods globalPatcher(GetModuleHandle(NULL));
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{

		//HMODULE currentProccessHandle = GetModuleHandle(NULL);
		//PEDeatis ped(currentProccessHandle);
		//petchMethods patcher(currentProccessHandle);

		//BYTE search[] = { 0x01,0x00,0x00,0x75,0x10,0x68 };
		//BYTE replace[] = { 0x01,0x00,0x00,0x74,0x10,0x68 };
		DWORD offset = 0x220;
		std::string search =  "0885C0742068??20";
		std::string replace = "??????75????????";
	
		globalPatcher.petchByOffset(offset, replace, search);
		//petchMethods::setCurrentProcessDetails(currentProcessDet);
	//	patcher.searcHAndReplace(".text", search, replace, sizeof(search));

		//delete currentProcessDet;
		
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

