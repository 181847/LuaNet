#pragma once
#include "LuaProcessStruct.h"

#pragma comment(lib, "Lua.lib")


#ifdef LUAPROCESS_EXPORTS
#define LUAPROCESS_API DLL_EXPORT_API
#elif LUAPROCESS_IMPORTS
#define LUAPROCESS_API DLL_IMPORT_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	int LUAPROCESS_API luaopen_LuaProcess(lua_State * L);
#ifdef __cplusplus
}
#endif

static const char * LuaMutexMetatableName = "Lua.Process.Mutex";

// get thread by userdata
#define CHECK_USERDATA_THREAD(L) \
	(reinterpret_cast<Lua::LuaThread*>(luaL_checkudata(L, 1, "Lua.Process.Thread")))

// get thread by userdata
#define CHECK_USERDATA_THREAD_INDEX(L, index) \
	(reinterpret_cast<Lua::LuaThread*>(luaL_checkudata(L, index, "Lua.Process.Thread")))

#define CHECK_USERDATA_MUTEX(L)\
	CHECK_USERDATA(L, HANDLE, LuaMutexMetatableName)

#define CHECK_USERDATA_MUTEX_INDEX(L, index)\
	CHECK_USERDATA_INDEX(L, index, HANDLE, LuaMutexMetatableName)

static int lua_newThread	(lua_State * L);

static int lua_resume		(lua_State * L);

static int lua_newMutex		(lua_State * L);

static int lua_lockMutex	(lua_State * L);

static int lua_unlockMutex	(lua_State * L);

static int lua_gcMutex		(lua_State * L);

static const struct luaL_Reg LuaProcessLib[]=
{
	{ "newThread",		lua_newThread },
	{ "newMutex",		lua_newMutex },
	{ NULL,				NULL }
};

static const struct luaL_Reg LuaThreadMethods[] =
{
	{ "resume",			lua_resume },
	{ NULL,				NULL }
};

static const struct luaL_Reg LuaMutexMethods[] =
{
	{ "lock",			lua_lockMutex },
	{ "unlock",			lua_unlockMutex },
	{ "__gc",			lua_gcMutex },
	{ NULL,				NULL }
};

DWORD WINAPI StartupLuaExecute(void * pLua_state);

int doWhenFailed(lua_State * L, const char * msg);