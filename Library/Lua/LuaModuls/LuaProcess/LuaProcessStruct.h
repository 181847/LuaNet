#pragma once
#include "../Library/MyTools/LuaTools.h"
#include "../Library/MyTools/UsefulDataType.h"
#include <Windows.h>

namespace Lua
{
	struct LuaThread {
		HANDLE	ThreadHandle;
		DWORD	ThreadID;
	};
};
