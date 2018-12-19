#pragma once
#include"stafx.h"
#define R 0
#define W 1
#define X 2
struct Section
{
	char name[9];
	DWORD virtualAddressAbs;
	DWORD secSize;
	DWORD secAlignedSize;
	DWORD pagesCount;
	bool RWX[3];
};
