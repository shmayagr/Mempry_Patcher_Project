#include"stdafx.h"
#include"PEDeatails.h"
PEDeatis::PEDeatis(HMODULE mz)
{
	MZHeaderAddr = mz;
	_IMAGE_DOS_HEADER* imgDosHeader = (_IMAGE_DOS_HEADER*)(MZHeaderAddr);
	_IMAGE_NT_HEADERS* imgNTHeaders = (_IMAGE_NT_HEADERS*)((char*)imgDosHeader + imgDosHeader->e_lfanew);
	imgFileHeader = &(imgNTHeaders->FileHeader);
	imgOptHeader = &(imgNTHeaders->OptionalHeader);
	secCount = imgFileHeader->NumberOfSections;
	firstSecAddrH = (DWORD)((char*)imgNTHeaders + sizeof(_IMAGE_NT_HEADERS));
	VirtualAddrbase = imgOptHeader->ImageBase;
	pageSize = imgOptHeader->SectionAlignment;
	initVals();
}

void PEDeatis::initVals()
{
	IMAGE_SECTION_HEADER* currSecH;
	char currSecName[9];
	//Section currSec;
	for(int i = 0;i<secCount;i++)
	{
		currSecH = (_IMAGE_SECTION_HEADER*)((char*)firstSecAddrH + i * sizeof(_IMAGE_SECTION_HEADER));
		memcpy(currSecName, currSecH->Name, 8);
		currSecName[8] = '\0';
		imgSecHMap[currSecName] = currSecH;
		Section currSec;
		strcpy_s(currSec.name, currSecName);
		currSec.virtualAddressAbs = VirtualAddrbase + currSecH->VirtualAddress;
		currSec.secSize = currSecH->Misc.VirtualSize;
		currSec.secAlignedSize = currSec.secSize + imgOptHeader->SectionAlignment - currSec.secSize%imgOptHeader->SectionAlignment;
		currSec.RWX[R] = currSecH->Characteristics & IMAGE_SCN_MEM_READ;
		currSec.RWX[W] = currSecH->Characteristics & IMAGE_SCN_MEM_WRITE;
		currSec.RWX[X] = currSecH->Characteristics & IMAGE_SCN_MEM_EXECUTE;
		currSec.pagesCount = currSec.secAlignedSize / pageSize;
		secMap[currSecName] = currSec;
	}
}

_IMAGE_FILE_HEADER * PEDeatis::getimgFileHeader()
{
	return imgFileHeader;
}

_IMAGE_OPTIONAL_HEADER * PEDeatis::getOptHeader()
{
	return imgOptHeader;
}

std::map< std::string,  _IMAGE_SECTION_HEADER*> PEDeatis::getImgSecMap()
{
	return imgSecHMap;
}

std::map<std::string, Section> PEDeatis::getSecMap()
{
	return secMap;
}

Section PEDeatis::operator[](std::string secName)
{
	return secMap[secName];
}

DWORD PEDeatis::getPageAddressByAbsAddr(std::string secName, DWORD absAddress)
{
	Section sec = secMap[secName];
	DWORD secAddress = sec.virtualAddressAbs;
	int offset = absAddress - secAddress;
	int pageNum = offset / pageSize;
	DWORD result = secAddress + pageNum * pageSize;
	return result;
}

DWORD PEDeatis::getPageAddressByOffset(std::string secName, DWORD offset)
{
	DWORD secAddr = secMap[secName].virtualAddressAbs;
	return getPageAddressByAbsAddr(secName, secAddr + offset);
}

DWORD PEDeatis::getPageSize()
{
	return pageSize;
}







