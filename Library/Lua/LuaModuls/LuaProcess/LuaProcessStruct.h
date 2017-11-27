#pragma once
#include "../../../MyTools/LuaTools.h"
#include "../../../MyTools/UsefulDataType.h"
#include <Windows.h>

namespace Lua
{
	struct LuaThread {
		HANDLE	ThreadHandle;
		DWORD	ThreadID;
	};
};
