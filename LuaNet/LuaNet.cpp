// LuaNet.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../Library/Lua/LuaInterpreter/LuaInterpreter.h"

#pragma comment(lib, "LuaInterpreter.lib")

int main()
{
	auto pLuaInter = std::make_unique<LuaInterpreter>();

	pLuaInter->DoFile("Init.lua");

	printf("%d %d %d", sizeof(unsigned char), sizeof(unsigned short), sizeof(unsigned long));

	pLuaInter->Run();

    return 0;
}

