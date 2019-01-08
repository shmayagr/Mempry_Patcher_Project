#include "stdafx.h"
#include "Utils.h"
#include<sstream>
int encodeHex(std::string hexStr)
{
	for (int i = 0;i < hexStr.size();i++)
	{
		char current = tolower(hexStr[i]);
		if (!(current < 'g' && current >= 'a' || current >= '0' && current <= '9'))
			return -1;
	}
	std::stringstream strConvertor;
	strConvertor << hexStr;
	int res;
	strConvertor >> std::hex >> res;
	return res;
}
bool patternCompare(BYTE * bytes, std::string hexStr)
{
	if (hexStr.size() % 2 != 0)
		hexStr = "0" + hexStr;
	for (int i = 0; i < hexStr.size();i += 2)
	{
		int currentVal = encodeHex(hexStr.substr(i, 2));
		if (currentVal >= 0 && *(bytes + i/2) != (BYTE)currentVal) {
			return false;
		}
	}
	return true;
}

void patternSet(BYTE * dst, std::string hexStr)
{
	if (hexStr.size() % 2 != 0)
		hexStr = "0" + hexStr;
	int size = hexStr.size();
	std::string sizestr = std::to_string(size);
	for (int i = 0; i < hexStr.size();i += 2)
	{
		int currentVal = encodeHex(hexStr.substr(i, 2));
		if (currentVal >= 0)
			memcpy((void*)(dst + i/2), (void*)&currentVal, sizeof(BYTE));
	}
}

