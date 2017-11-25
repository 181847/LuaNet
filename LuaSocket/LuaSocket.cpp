// LuaSocket.cpp : 定义 DLL 应用程序的导出函数。
//
#include "LuaSocket.h"

#ifdef __cplusplus
extern "C"
{
#endif
int LUASOCKET_API luaopen_LuaSocket(lua_State * L)
{
	luaL_newmetatable(L, "Lua.Socket");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_newlib(L, LuaSocketLib);
	return 1;
}
#ifdef __cplusplus
}
#endif

int lua_Startup(lua_State * L)
{
	auto v1 = luaL_checkinteger(L, 1);
	auto v2 = luaL_checkinteger(L, 2);
	
	WORD wVersion = MAKEWORD(v1, v2);
	WSADATA wsadata;
	int error = WSAStartup(wVersion, &wsadata);

	if (error || wsadata.wVersion != wVersion)
	{
		return doWhenFailed(L, "WSAStartup Failed");
	}
	else
	{
		// success 
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_Cleanup(lua_State * L)
{
	int error = WSACleanup();
	if (error)
	{
		return doWhenFailed(L, "WSACleanup Failed");
	}
	lua_pushboolean(L, true);
	return 1;
}

int lua_newSocket(lua_State * L)
{
	auto * pSocket = 
		reinterpret_cast<SOCKET *>(lua_newuserdata(L, sizeof(SOCKET)));

	SOCKET& sock = *pSocket;

	int af = static_cast<int>(luaL_checkinteger(L, 1));
	int type = static_cast<int>(luaL_checkinteger(L, 2));
	int protocol = static_cast<int>(luaL_checkinteger(L, 3));

	sock = socket(af, type, protocol);

	if (sock == INVALID_SOCKET)
	{
		return doWhenFailed(L, "socket creation failed");
	}
	else
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int doWhenFailed(lua_State * L, const char * message)
{

	lua_pushboolean(L, false);
	lua_pushstring(L, message);
	lua_pushinteger(L, WSAGetLastError());
	return 3;
}
