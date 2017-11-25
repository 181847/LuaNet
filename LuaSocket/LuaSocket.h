#pragma once

#include "../Library/MyTools/LuaTools.h"
#include "LuaSocketStruct.h"

#pragma comment(lib, "Lua.lib")

#ifdef LUASOCKET_EXPORTS
#define LUASOCKET_API DLL_EXPORT_API
#elif LUASOCKET_IMPORTS
#define LUASOCKET_API DLL_IMPORT_API
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	int LUASOCKET_API luaopen_LuaSocket(lua_State* L);

#ifdef __cplusplus
}
#endif

// to call WSAStartup()
static int lua_Startup		(lua_State * L);

// to call WSACleanup()
static int lua_Cleanup		(lua_State * L);

// create a socket
static int lua_newSocket	(lua_State * L);

// create a address
static int lua_newAddress	(lua_State * L);


// THIS FUNCTION IS NOT IN THE LIBRARY,
// IT IS JUST USED IN THE C.
static int doWhenFailed(lua_State *L, const char * message);

static const struct luaL_Reg LuaSocketLib[] =
{
	{"Startup", lua_Startup},
	{"Cleanup", lua_Cleanup},
	{"newSocket", lua_newSocket},
	{"newAddress", lua_newAddress},
	{NULL, NULL}
};
