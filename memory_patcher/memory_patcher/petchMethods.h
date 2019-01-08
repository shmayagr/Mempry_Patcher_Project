#pragma once
#include "stdafx.h"
//class Section;
#include"section.h"
//class PEDetails;
#include "PEDeatails.h"
struct WaitThraedArgs
{
	LPVOID objectPointer;
	DWORD addressAbs;
	std::string search;
	std::string replace;
};
class petchMethods
{
	
	DWORD dontCareProtect;
	PEDeatis currentProcessDetails;
	std::map<DWORD, DWORD> savedPagesProtect;
	std::vector<WaitThraedArgs> VthreadArgs;
	std::vector<HANDLE> VrunningThreads;
public:

	petchMethods(HMODULE mz) :currentProcessDetails(mz) {}// : currentProcessDetails(mz) {}
	~petchMethods();
	LPVOID getNewThreadArgsP(DWORD addressAbs, std::string search, std::string replace);
	//void searchAndReplace()
	//void searcHAndReplace(std::string, BYTE*, BYTE*, DWORD);
	void petchByOffset(DWORD offset, std::string replace, std::string search);
	void petchOffsetInSec(DWORD offset, std::string name, std::string replace, std::string search);
	static void patchWaitForOffsetChanged(LPVOID argsStruct);
	BOOL setProtect(DWORD, DWORD, DWORD*);
	BOOL setFullProtect(DWORD);
	BOOL recoverPageProtection(DWORD);
	BOOL setAllSectionPagesFullProtect(std::string);
	BOOL recoverAllSectionPagesProtect(std::string);
	size_t searchAndReplaceAddRange(DWORD addrBase,DWORD range,std::string search,std::string replace, DWORD rangeSize, int matchesCount,int matchStartIndex);
	//void search
};
