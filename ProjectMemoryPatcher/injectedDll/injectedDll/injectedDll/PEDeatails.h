#pragma once
#include"section.h"
#include"stafx.h"
#include<map>
struct Section;
class PEDeatis
{
	HMODULE MZHeaderAddr;
	_IMAGE_OPTIONAL_HEADER* imgOptHeader;
	_IMAGE_FILE_HEADER* imgFileHeader;
	DWORD VirtualAddrbase;
	DWORD secCount;
	DWORD pageSize;
	std::map<std::string, _IMAGE_SECTION_HEADER*>imgSecHMap;
	std::map<std::string, Section> secMap;
	DWORD firstSecAddrH;
public:
	PEDeatis(HMODULE);
	void initVals();
	_IMAGE_FILE_HEADER* getimgFileHeader();
	_IMAGE_OPTIONAL_HEADER* getOptHeader();
	//DWORD getFirstSecAddrH();
	std::map< std::string, _IMAGE_SECTION_HEADER*> getImgSecMap();
    std::map< std::string, Section>getSecMap();
	Section operator[](std::string);
	DWORD getPageAddressByAbsAddr(std::string secName, DWORD absAddress);
	DWORD getPageAddressByOffset(std::string secName, DWORD offset);
	DWORD getPageSize();
};
