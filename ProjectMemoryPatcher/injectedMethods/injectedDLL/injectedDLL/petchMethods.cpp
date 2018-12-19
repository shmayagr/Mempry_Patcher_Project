#include "petchMethods.h"
#include"section.h"

PEDeatis* petchMethods::currentProcessDetails = nullptr;
void petchMethods::setCurrentProcessDetails(PEDeatis *currentProccess)
{
	if(nullptr == currentProcessDetails)
		currentProcessDetails = currentProccess;
}
void petchMethods::searcHAndReplace(std::string secName, BYTE * search, BYTE * replace, DWORD sANDrSize)
{
	Section sec = petchMethods::currentProcessDetails->operator[][secName];//currentProcessDetails
	DWORD secAddress = sec.virtualAddressAbs;
	DWORD endSecaddress = secAddress + sec.secAlignedSize;
	bool notFullpermission = false;
	DWORD pageSize = currentProcessDetails->getPageSize();
	DWORD* CurrentProtectionList = nullptr; //= new DWORD[sec.pagesCount];
	DWORD junk;
	if(!(sec.RWX[R]&&sec.RWX[W]&&sec.RWX[X]))
	{
		notFullpermission = true;
		CurrentProtectionList = new DWORD[sec.pagesCount];
		for(int i = 0;i<sec.pagesCount;i++)
			VirtualProtect((void*)((char*)secAddress + i * pageSize), pageSize, PAGE_EXECUTE_READWRITE, &CurrentProtectionList[i]);
	}
	int startBytes = 0;
	while((secAddress + sANDrSize)<endSecaddress)
	{
		if (!memcmp((void*)((BYTE*)secAddress + startBytes), search, sANDrSize))
			memcpy((void*)((BYTE*)secAddress + startBytes), replace, sANDrSize);
	}
	if(notFullpermission)
	{
		for(int i= 0;i<sec.pagesCount;i++)
			VirtualProtect((void*)((char*)secAddress + i * pageSize), pageSize,CurrentProtectionList[i],&junk);
		delete[] CurrentProtectionList;
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		HMODULE currentProccessHandle = GetModuleHandle(NULL);
		PEDeatis *currentProcessDet = new PEDeatis(currentProccessHandle);
		petchMethods::setCurrentProcessDetails(currentProcessDet);
		BYTE search[] = { 0x08,0x85,0xC0,0x74,0x20,0x68 };
		BYTE replace[] = { 0x08,0x85,0xC0,0x75,0x20,0x68 };
		//petchMethods::setCurrentProcessDetails(currentProcessDet);
		petchMethods::searcHAndReplace("SEC0", search, replace, sizeof(search));
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
	return true;
}
