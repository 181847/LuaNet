#include "LuaProcess.h"

int LUAPROCESS_API luaopen_LuaProcess(lua_State * L)
{
	luaL_newmetatable(L, "Lua.Process.Thread");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, LuaThreadMethods, 0);

	registeUserdataType(L, LuaMutexMetatableName, LuaMutexMethods);

	luaL_newlib(L, LuaProcessLib);
	return 1;
}

int lua_newThread(lua_State * L)
{
	// the first parameter must be the luafunction
	luaL_checktype(L, 1, LUA_TFUNCTION);
	// creat another stack, remember that don't lose the reference to the newState,
	// or the newState will be collected by the mainThread.
	lua_State * newState = lua_newthread(L);

	// get the function to the stack top, then pass the function to the newState.
	lua_pushvalue(L, 1);
	lua_xmove(L, newState, 1);

	// create a new userdata to store the thread information,
	// remember that the Lua::LuaThread is not a heap obj
	// so we don't need a pointContainer,
	// nor do we need a __gc method.
	auto * pThread = reinterpret_cast<Lua::LuaThread*>(
		lua_newuserdata(L, sizeof(Lua::LuaThread)));
	// clear memeory
	pThread->ThreadHandle = 0;
	pThread->ThreadID = 0;

	// creat the thread, but don't run it immediately.
	pThread->ThreadHandle = CreateThread(NULL, 0, 
		StartupLuaExecute, reinterpret_cast<void*>(newState), 
		CREATE_SUSPENDED, &pThread->ThreadID);

	if (pThread->ThreadHandle == NULL)
	{
		return doWhenFailed(L, "Thread Creation failed");
	}

	luaL_getmetatable(L, "Lua.Process.Thread");
	lua_setmetatable(L, -2);

	// creat a new table to contain the newState and threadInfo
	lua_newtable(L);


	// for now, the stack state is 
	// top->	newTable					-1
	//			Lua::LuaThread				-2
	//			newState					-3
	//			.....(maybe some no used argument)
	// btm->	funtion
	lua_pushvalue	(L, -3);
	lua_setfield	(L, -2, "LuaThread");


	// for now, the stack state is 
	// top->	newTable					-1
	//			Lua::LuaThread				-2
	//			newState					-3
	//			.....(maybe some no used argument)
	// btm->	funtion
	lua_pushvalue	(L, -2);
	lua_setfield	(L, -2, "SysThread");
	
	return 1;
}

int lua_resume(lua_State * L)
{
	auto * pLuaThread = CHECK_USERDATA_THREAD(L);
	DWORD error = ResumeThread(pLuaThread->ThreadHandle);
	if (error == -1)
	{
		return doWhenFailed(L, "Thread resumation failed");
	}
	else
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_newMutex(lua_State * L)
{
	auto pmutex = newUserdataInStack<HANDLE>(L, LuaMutexMetatableName);

	*pmutex = CreateMutex(NULL, false, NULL);

	if (*pmutex == NULL)
	{
		return doWhenFailed(L, "Mutex creation failed");
	}

	return 1;
}

int lua_lockMutex(lua_State * L)
{
	auto *pmutex = CHECK_USERDATA_MUTEX(L);
	DWORD waitTime = 0;
	if (lua_gettop(L) == 1)
	{
		waitTime = INFINITE;
	}
	else
	{
		waitTime = static_cast<DWORD>(luaL_checkinteger(L, 2));
	}

	DWORD result = WaitForSingleObject(*pmutex, waitTime);

	if (result == WAIT_FAILED)
	{
		return doWhenFailed(L, "Mutex Lock Failed");
	}
	else
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_unlockMutex(lua_State * L)
{
	auto *pmutex = CHECK_USERDATA_MUTEX(L);
	
	auto result = ReleaseMutex(*pmutex);

	if (result == 0)
	{
		return doWhenFailed(L, "Mutex Unlock Failed");
	}
	else
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_gcMutex(lua_State * L)
{
	auto * pmutex = CHECK_USERDATA_MUTEX(L);
	// if failed CloseHandle will return zero
	if (CloseHandle(*pmutex))
	{
		// success
		lua_pushboolean(L, true);
		return 1;
	}
	else
	{
		return doWhenFailed(L, "Close Mutex failed");
	}
}

DWORD WINAPI StartupLuaExecute(void * pLua_state)
{
	DEBUG_MESSAGE("thread resume start\n");
	auto * L = reinterpret_cast<lua_State*>(pLua_state);
	DEBUG_MESSAGE("stack size: %d\n", lua_gettop(L));
	lua_pcall(L, 0, 0, 0);

	DEBUG_MESSAGE("thread resume end\n");
	return 0;
}

int doWhenFailed(lua_State * L, const char * msg)
{
	lua_pushboolean(L, false);
	lua_pushstring(L, msg);
	lua_pushinteger(L, static_cast<lua_Integer>(GetLastError()));
	return 3;
}
