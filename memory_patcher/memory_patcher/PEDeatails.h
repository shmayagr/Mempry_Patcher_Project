#pragma once

#include"stdafx.h"
#include"section.h"
#include<map>
#include<vector>
struct Section;
class PEDeatis
{
	HMODULE MZHeaderAddr;
	_IMAGE_OPTIONAL_HEADER* imgOptHeader;
	_IMAGE_FILE_HEADER* imgFileHeader;
	DWORD VirtualAddrbase;
	DWORD secCount;
	DWORD pageSize;
	DWORD imageBase;
	std::map<std::string, _IMAGE_SECTION_HEADER*>imgSecHMap;
	std::map<std::string, Section> secMap;
	std::vector<Section> SecVec;
	DWORD firstSecAddrH;
public:
	PEDeatis(HMODULE mz);
	PEDeatis() {}
	void initVals();
	_IMAGE_FILE_HEADER* getimgFileHeader();
	_IMAGE_OPTIONAL_HEADER* getOptHeader();
	//DWORD getFirstSecAddrH();
	
	std::map< std::string, _IMAGE_SECTION_HEADER*> getImgSecMap();
	std::map< std::string, Section>getSecMap();
	Section operator[](std::string);
	Section operator[](size_t);
	DWORD getPageAddressByAbsAddr(std::string secName, DWORD absAddress);
	DWORD getPageAddressByOffset(std::string secName, DWORD offset);
	DWORD getPageSize();
	Section getSecByabsAddr(DWORD addr);
	Section getSectionByoffset(DWORD offset);
	DWORD getImageBase();

};
