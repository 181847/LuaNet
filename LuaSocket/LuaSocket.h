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

// get socket by userdata
#define CHECK_USERDATA_SOCKET(L) \
	(reinterpret_cast<SOCKET*>(luaL_checkudata(L, 1, "Lua.Socket.Socket")))

// get socket by userdata
#define CHECK_USERDATA_SOCKET_INDEX(L, index) \
	(reinterpret_cast<SOCKET*>(luaL_checkudata(L, index, "Lua.Socket.Socket")))

// get address by userdata
#define CHECK_USERDATA_ADDRESS(L) \
	(reinterpret_cast<SOCKADDR_IN*>(luaL_checkudata(L, 1, "Lua.Socket.Address")))

// get address by userdata
#define CHECK_USERDATA_ADDRESS_INDEX(L, index) \
	(reinterpret_cast<SOCKADDR_IN*>(luaL_checkudata(L, index, "Lua.Socket.Address")))

// to call WSAStartup()
static int lua_Startup		(lua_State * L);

// to call WSACleanup()
static int lua_Cleanup		(lua_State * L);

// create a socket
static int lua_newSocket	(lua_State * L);

// close a socket
static int lua_closeSocket	(lua_State * L);

// create a address
static int lua_newAddress	(lua_State * L);

// bind a socket to an address
static int lua_bind			(lua_State * L);

// place a socket in the state of ready to listen a client to connection.
static int lua_listen		(lua_State * L);

// accept a socket from a client, be aware that this operation 
// will block the process, be sure that a client will connect to it,
// or you have to close the Application to stop waiting.
static int lua_accept(lua_State * L);



// THIS FUNCTION IS NOT IN THE LIBRARY,
// IT IS JUST USED IN THE C.
static int doWhenFailed(lua_State *L, const char * message);

static const struct luaL_Reg LuaSocketLib[] =
{
	{ "Startup",		lua_Startup},
	{ "Cleanup",		lua_Cleanup},
	{ "newSocket",		lua_newSocket},
	{ "closeSocket",	lua_closeSocket },
	{ "newAddress",		lua_newAddress},
	{ "bind",			lua_bind },
	{ "listen",			lua_listen },
	{ "accept",			lua_accept },
	{NULL, NULL}
};
