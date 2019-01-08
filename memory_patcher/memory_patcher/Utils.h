#pragma once
#include<string>
#include"stdafx.h"
int encodeHex(std::string hexStr);
bool patternCompare(BYTE* bytes, std::string hexStr);
void patternSet(BYTE* dst, std::string hexStr);