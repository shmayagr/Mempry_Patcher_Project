#include "stdafx.h"
#include "Utils.h"

int encodeHex(std::string hexStr)
{
	for (std::string::iterator it = hexStr.begin();it != hexStr.begin();it++)
	{
		char current = tolower(*(it));
		if (!(current < 'g' && current >= 'a' || current >= '0' && current <= '9'))
			return -1;
	}
	return std::stoi(hexStr, 0, 16);
}

bool patternCompare(BYTE * bytes, std::string hexStr)
{
	if (hexStr.size() % 2 != 0)
		hexStr = "0" + hexStr;
	for (int i = 0; i < hexStr.size();i += 2)
	{
		int currentVal = encodeHex(hexStr.substr(i, 2));
		if (currentVal >= 0 && *(bytes + i) != (BYTE)currentVal)
			return false;
	}
	return true;
}

void patternSet(BYTE * dst, std::string hexStr)
{
	if (hexStr.size() % 2 != 0)
		hexStr = "0" + hexStr;
	for (int i = 0; i < hexStr.size();i += 2)
	{
		int currentVal = encodeHex(hexStr.substr(i, 2));
		if (currentVal >= 0)
			memcpy((void*)(dst + i), (void*)&currentVal, sizeof(BYTE));
	}

}

