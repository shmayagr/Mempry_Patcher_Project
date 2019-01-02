#include "Utils.h"
#include "stdafx.h"
#include "petchMethods.h"
#include "section.h"

//PEDeatis* petchMethods::currentProcessDetails = nullptr;

petchMethods::petchMethods(PEDeatis ped)
{
	currentProcessDetails = ped;
}

petchMethods::~petchMethods()
{
	for(int i = 0; i< currentProcessDetails.getimgFileHeader()->NumberOfSections;i++)
	{
		recoverAllSectionPagesProtect(currentProcessDetails[i].name);
	}
}
void petchMethods::searcHAndReplace(std::string secName, BYTE * search, BYTE * replace, DWORD sANDrSize)
{
	Section sec = (currentProcessDetails)[secName];//currentProcessDetails
	DWORD secAddress = sec.virtualAddressAbs;
	DWORD endSecaddress = secAddress + sec.secAlignedSize;
	bool notFullpermission = false;
	DWORD pageSize = currentProcessDetails.getPageSize();
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

BOOL petchMethods::setProtect(DWORD page, DWORD protection, DWORD* old)
{
	return VirtualProtect((void*)page, currentProcessDetails.getPageSize(), protection, old);
}

BOOL petchMethods::setFullProtect(DWORD page)
{
	DWORD currentProtection;
	BOOL success = setProtect(page, PAGE_EXECUTE_READWRITE, &currentProtection);
	if (!success)
		return success;
	if(savedPagesProtect.end() == savedPagesProtect.find(page))
		savedPagesProtect[page] = currentProtection;
	return success;
}

BOOL petchMethods::recoverPageProtection(DWORD page)
{
	if (savedPagesProtect.find(page) == savedPagesProtect.end())
		return setProtect(page, savedPagesProtect[page], &dontCareProtect);
	return true;
}

BOOL petchMethods::setAllSectionPagesFullProtect(std::string secName)
{
	BOOL success = true;
	
	Section sec = currentProcessDetails[secName];
	for(int i = 0;i<sec.pagesCount;i++ )
		if(!setFullProtect(sec.virtualAddressAbs + i * currentProcessDetails.getPageSize()))
			success = false;
	return success;
}

BOOL petchMethods::recoverAllSectionPagesProtect(std::string secName)
{
	BOOL success = true;
	Section sec = currentProcessDetails[secName];
	for (int i = 0;i < sec.pagesCount;i++)
		if (!recoverPageProtection(sec.virtualAddressAbs + i * currentProcessDetails.getPageSize()))
			success = false;
	return success;
}






