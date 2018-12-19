#include "stdafx.h"
#include "petchMethods.h"
#include "section.h"

PEDeatis* petchMethods::currentProcessDetails = nullptr;
void petchMethods::setCurrentProcessDetails(PEDeatis *currentProccess)
{
	if(nullptr == currentProcessDetails)
		currentProcessDetails = currentProccess;
}
void petchMethods::searcHAndReplace(std::string secName, BYTE * search, BYTE * replace, DWORD sANDrSize)
{
	Section sec = (*(petchMethods::currentProcessDetails))[secName];//currentProcessDetails
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
			//system("echo  Match >> C:\\users\\shmay\\nice.txt");
		startBytes++;
	}
	if(notFullpermission)
	{
		for(int i= 0;i<sec.pagesCount;i++)
			VirtualProtect((void*)((char*)secAddress + i * pageSize), pageSize,CurrentProtectionList[i],&junk);
		delete[] CurrentProtectionList;
	}
}

