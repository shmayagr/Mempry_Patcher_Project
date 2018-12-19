#pragma once
#include "stafx.h"
//class Section;
#include"section.h"
//class PEDetails;
#include "PEDeatails.h"
class petchMethods
{
	//int b;
	static PEDeatis* currentProcessDetails;
public:
	//petchMethods(PEDeatis);
	static void setCurrentProcessDetails(PEDeatis*);
	static void searcHAndReplace(std::string, BYTE*, BYTE*,DWORD);
};
