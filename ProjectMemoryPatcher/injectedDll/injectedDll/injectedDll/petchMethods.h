#pragma once
#include "stafx.h"
//class Section;
#include"section.h"
//class PEDetails;
#include "PEDeatails.h"
class petchMethods
{
	
	DWORD dontCareProtect;
	PEDeatis currentProcessDetails;
	std::map<DWORD, DWORD> savedPagesProtect;
public:
	
	petchMethods(HMODULE mz) :currentProcessDetails(mz) {}// : currentProcessDetails(mz) {}
	~petchMethods();
	void searcHAndReplace(std::string, BYTE*, BYTE*,DWORD);
	BOOL setProtect(DWORD,DWORD,DWORD*);
	BOOL setFullProtect(DWORD);
	BOOL recoverPageProtection(DWORD);
	BOOL setAllSectionPagesFullProtect(std::string);
	BOOL recoverAllSectionPagesProtect(std::string);
};
