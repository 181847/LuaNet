#pragma once

#include "../Library/MyTools/LuaTools.h"
#include "../Library/MyTools/Formater.h"
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

// get netData by userdata
#define CHECK_USERDATA_NETDATA(L)\
	(reinterpret_cast<NetData*>(luaL_checkudata(L, 1, "Lua.Socket.NetData")))

// get netData by userdata
#define CHECK_USERDATA_NETDATA_INDEX(L, index)\
	(reinterpret_cast<NetData*>(luaL_checkudata(L, index, "Lua.Socket.NetData")))

// get ipdata by userdata
#define CHECK_USERDATA_IPDATA(L)\
	(reinterpret_cast<IPData*>(luaL_checkudata(L, 1, "Lua.Socket.IPData")))

// get ipdata by userdata
#define CHECK_USERDATA_IPDATA_INDEX(L, index)\
	(reinterpret_cast<IPData*>(luaL_checkudata(L, index, "Lua.Socket.IPData")))

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
static int lua_accept		(lua_State * L);

// connecte to a server.
static int lua_connect		(lua_State * L);

// send data.
static int lua_send			(lua_State * L);

// receive data
static int lua_recv			(lua_State * L);

// send data.
static int lua_sendto		(lua_State * L);

// receive data
static int lua_recvfrom		(lua_State * L);

// set the socket to receive all ip package
static int lua_recvAll		(lua_State * L);

// *********************************  NetData ********************************
// create a NetData
static int lua_newNetData		(lua_State * L);

// return the size of NetData
static int lua_sizeOfNetData	(lua_State * L);

// convert a string to a meshdata
static int lua_NetDataToString	(lua_State * L);

// fill a meshdata with the string
static int lua_fillNetData		(lua_State * L);

// print the data in hex type
static int lua_showInHexData	(lua_State * L);


// *********************************  IPParser ********************************
// create a IPParser to get or set data in a NetData
// here must pass a NetData in and the Parser will copy
// the memory.
static int lua_newIPData(lua_State * L);

// return the ip address of the ippackage
static int lua_IPDataSourceIP	(lua_State * L);

// return the ip address of the ippackage
static int lua_IPDataSetSourceIP(lua_State * L);

// THIS FUNCTION IS NOT IN THE LUA,
// IT IS JUST USED IN THE C.
static int doWhenFailed(lua_State *L, const char * message);

static const struct luaL_Reg LuaSocketLib[] =
{
	{ "Startup",		lua_Startup},
	{ "Cleanup",		lua_Cleanup},
	{ "newSocket",		lua_newSocket},
	{ "closeSocket",	lua_closeSocket },
	{ "newAddress",		lua_newAddress},
	{ "newNetData",		lua_newNetData },
	{ "bind",			lua_bind },
	{ "listen",			lua_listen },
	{ "accept",			lua_accept },
	{ "connect",		lua_connect },
	{ "send",			lua_send },
	{ "recv",			lua_recv},
	{ "sendto",			lua_sendto },
	{ "recvfrom",		lua_recvfrom },
	{ "newIPData",		lua_newIPData },
	{ NULL,				NULL}
};

static const struct luaL_Reg LuaNetDataFunctions[] =
{
	{ "size",			lua_sizeOfNetData},
	{ "toString",		lua_NetDataToString },
	{ "fill",			lua_fillNetData },
	{ NULL,				NULL }
};


static const struct luaL_Reg LuaSocketMethods[] =
{
	{ "recvAll",		lua_recvAll },
	{ "__gc",			lua_closeSocket },
	{ NULL,				NULL }
};


static const struct luaL_Reg LuaIPDataMethods[] =
{
	{ "sourceIP",		lua_IPDataSourceIP },
	{ "setSourceIP",	lua_IPDataSetSourceIP },
	{ NULL,				NULL }
};
