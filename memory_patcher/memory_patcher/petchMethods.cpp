//#include "Utils.h"
#include "stdafx.h"
#include "petchMethods.h"
#include "section.h"
#include"Utils.h"
//PEDeatis* petchMethods::currentProcessDetails = nullptr;



petchMethods::~petchMethods()
{
	for (int i = 0; i < currentProcessDetails.getimgFileHeader()->NumberOfSections;i++)
	{
		recoverAllSectionPagesProtect(currentProcessDetails[i].name);
	}
	for (std::vector<HANDLE>::iterator it = VrunningThreads.begin();it != VrunningThreads.end();it++) 
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(*(it), 0))
			TerminateThread(*(it), 0);
	}
}
LPVOID petchMethods::getNewThreadArgsP(DWORD addressAbs, std::string search, std::string replace)
{
	WaitThraedArgs newThreadArgs;
	newThreadArgs.objectPointer = (LPVOID)this;
	newThreadArgs.addressAbs = addressAbs;
	newThreadArgs.search = search;
	newThreadArgs.replace = replace;
	VthreadArgs.push_back(newThreadArgs);
	return (LPVOID)&(VthreadArgs[VthreadArgs.size() - 1]);
}
//void petchMethods::searcHAndReplace(std::string secName, BYTE * search, BYTE * replace, DWORD sANDrSize)
//{
//	Section sec = (currentProcessDetails)[secName];//currentProcessDetails
//	DWORD secAddress = sec.virtualAddressAbs;
//	DWORD endSecaddress = secAddress + sec.secAlignedSize;
//	bool notFullpermission = false;
//	DWORD pageSize = currentProcessDetails.getPageSize();
//	DWORD* CurrentProtectionList = nullptr; //= new DWORD[sec.pagesCount];
//	DWORD junk;
//	if (!(sec.RWX[R] && sec.RWX[W] && sec.RWX[X]))
//	{
//		notFullpermission = true;
//		CurrentProtectionList = new DWORD[sec.pagesCount];
//		for (int i = 0;i < sec.pagesCount;i++)
//			VirtualProtect((void*)((char*)secAddress + i * pageSize), pageSize, PAGE_EXECUTE_READWRITE, &CurrentProtectionList[i]);
//	}
//	int startBytes = 0;
//	while ((secAddress + sANDrSize) < endSecaddress)
//	{
//		if (!memcmp((void*)((BYTE*)secAddress + startBytes), search, sANDrSize))
//			memcpy((void*)((BYTE*)secAddress + startBytes), replace, sANDrSize);
//		//system("echo  Match >> C:\\users\\shmay\\nice.txt");
//		startBytes++;
//	}
//	if (notFullpermission)
//	{
//		for (int i = 0;i < sec.pagesCount;i++)
//			VirtualProtect((void*)((char*)secAddress + i * pageSize), pageSize, CurrentProtectionList[i], &junk);
//		delete[] CurrentProtectionList;
//	}
//}

void petchMethods::petchByOffset(DWORD offset, std::string replace, std::string search = "")
{
	//MessageBox(NULL, replace.c_str(), search.c_str(), MB_OK);
	DWORD address = currentProcessDetails.getImageBase() + offset;
	Section sec = currentProcessDetails.getSectionByoffset(offset);
	if (!(sec.RWX[R] && sec.RWX[W] && sec.RWX[X])) 
	{
		DWORD page = currentProcessDetails.getPageAddressByAbsAddr(sec.name, address);
		setFullProtect(page);
		for(int i = 1;page + i*currentProcessDetails.getPageSize() < address + (replace.size() +1)/2;i++)
			setFullProtect(page + i* currentProcessDetails.getPageSize());
	}
	if(!search.empty())
	{
		LPVOID waitThreadArgs = getNewThreadArgsP(address, search, replace);
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)petchMethods::patchWaitForOffsetChanged, waitThreadArgs, NULL, NULL);
		VrunningThreads.push_back(hThread);
	}
	else
	{

		patternSet((BYTE*)address, replace);
	}
}

void petchMethods::petchOffsetInSec(DWORD offset, std::string name, std::string replace, std::string search = "")
{
	Section sec = currentProcessDetails[name];
	DWORD baseOffset = sec.offset + offset;
	petchByOffset(baseOffset, replace, search);
}

void petchMethods::patchWaitForOffsetChanged(LPVOID argsStruct)
{
	
	WaitThraedArgs* args = (WaitThraedArgs*)argsStruct;
	MessageBox(NULL, args->search.c_str(), "in thread", MB_OK);
	DWORD address = args->addressAbs;
	petchMethods* currentObj = (petchMethods*)args->objectPointer;
	while (!patternCompare((BYTE*)address, args->search));
	//MessageBox(NULL, "Found search", "in", MB_OK);
	patternSet((BYTE*)address, args->replace);
}

BOOL petchMethods::setProtect(DWORD page, DWORD protection, DWORD* old)
{
	//MessageBox(NULL, std::to_string(currentProcessDetails.getPageSize()).c_str(), "page size", MB_OK);
	return VirtualProtect((void*)page, currentProcessDetails.getPageSize(), protection, old);
}

BOOL petchMethods::setFullProtect(DWORD page)
{
	DWORD currentProtection;
	BOOL success = setProtect(page, PAGE_EXECUTE_READWRITE, &currentProtection);
	if (!success)
		return success;
	if (savedPagesProtect.end() == savedPagesProtect.find(page))
		savedPagesProtect[page] = currentProtection;
	return success;
}

BOOL petchMethods::recoverPageProtection(DWORD page)
{
	if (savedPagesProtect.find(page) != savedPagesProtect.end())
		return setProtect(page, savedPagesProtect[page], &dontCareProtect);
	savedPagesProtect.erase(page);
	return true;
}

BOOL petchMethods::setAllSectionPagesFullProtect(std::string secName)
{
	BOOL success = true;

	Section sec = currentProcessDetails[secName];
	for (int i = 0;i < sec.pagesCount;i++)
		if (!setFullProtect(sec.virtualAddressAbs + i * currentProcessDetails.getPageSize()))
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

size_t petchMethods::searchAndReplaceAddRange(DWORD addrBase,DWORD range ,std::string search, std::string replace, DWORD rangeSize, int matchesCount = -1, int matchStartIndex =-1)
{
	int from = matchStartIndex;
	int matches = matchesCount;
	size_t matchesFound = 0;
	DWORD offset = 0;
	while (addrBase + search.size() < addrBase + range && addrBase + replace.size() < addrBase + range) 
	{
		
	}
}











