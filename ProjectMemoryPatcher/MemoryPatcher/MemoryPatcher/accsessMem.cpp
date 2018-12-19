#include"accessMem.h"
#include"stafx.h"
char* __stdcall getFirstSectionAddress()
{
	DWORD mzHeader = (DWORD)GetModuleHandle(NULL);
	_IMAGE_DOS_HEADER* imgDosHeader = (_IMAGE_DOS_HEADER*)mzHeader;
	_IMAGE_NT_HEADERS* imgNTHeader = (_IMAGE_NT_HEADERS*)((char*)imgDosHeader + imgDosHeader->e_lfanew);
	_IMAGE_SECTION_HEADER* fistSecaddr = (IMAGE_SECTION_HEADER*)((char*)imgNTHeader + sizeof(_IMAGE_NT_HEADERS));
	_IMAGE_OPTIONAL_HEADER* optHeader = &(imgNTHeader->OptionalHeader);
	DWORD optionalHeaderBase = optHeader->ImageBase;
	_IMAGE_FILE_HEADER* imgFileHeader = &(imgNTHeader->FileHeader);

	char* firsSecName = (char*)(fistSecaddr->Name);
	//fistSecaddr->Characteristics
	printf("nt %d\n", imgNTHeader);
	int iseXecutable = (fistSecaddr->Characteristics ^ 0x20000000);
	int isReadable = (fistSecaddr->Characteristics ^ 0x40000000);
	int isWriteable = (fistSecaddr->Characteristics ^ 0x80000000);
	printf("sizeof ntHeader %d\n", sizeof(_IMAGE_NT_HEADERS));
	_IMAGE_SECTION_HEADER* imgTxtHeader = (_IMAGE_SECTION_HEADER*)((char*)fistSecaddr + sizeof(_IMAGE_SECTION_HEADER));
	//printf("first sec name %s   num of sec %d R = %d  W  %d  X  %d 2^2 %d", firsSecName, imgTxtHeader->NumberOfSections,isReadable,isWriteable,iseXecutable,2^2);
	bool iseXecutable2 = (bool)(imgTxtHeader->Characteristics & 0x20000000);
	bool isReadable2 = (bool)(imgTxtHeader->Characteristics & 0x40000000);
	//bool isWriteable2 = (bool)(imgTxtHeader->Characteristics & 0x80000000);
	//imgTxtHeader->Characteristics += 0x80000000;
	bool isWriteable2 = (bool)(imgTxtHeader->Characteristics & 0x80000000);

	printf("txth name %s charctistic %d R %u W %u X %u" , imgTxtHeader->Name, imgTxtHeader->Characteristics,isReadable2,isWriteable2,iseXecutable);
	_IMAGE_SECTION_HEADER* imgRdataSecH = (_IMAGE_SECTION_HEADER*)((char*)imgTxtHeader + 1*sizeof(_IMAGE_SECTION_HEADER));
	DWORD RdataAddress = imgRdataSecH->VirtualAddress;
	char*someStr = (char*)(RdataAddress + 0xC38 + optionalHeaderBase);
	printf("name %s size %d str %s secAlignment %d",imgRdataSecH->Name,imgRdataSecH->Misc.VirtualSize, someStr,optHeader->SectionAlignment);
	return (char*)NULL;
}