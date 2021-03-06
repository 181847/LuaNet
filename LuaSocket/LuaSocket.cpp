// LuaSocket.cpp : 定义 DLL 应用程序的导出函数。
//
#include "LuaSocket.h"

#ifdef __cplusplus
extern "C"
{
#endif
int LUASOCKET_API luaopen_LuaSocket(lua_State * L)
{
	// this for the socket object
	luaL_newmetatable(L, "Lua.Socket.Socket");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, LuaSocketMethods, 0);

	// this for the addr_in object
	luaL_newmetatable(L, "Lua.Socket.Address");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	// this for the NetData
	luaL_newmetatable(L, "Lua.Socket.NetData");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, LuaNetDataFunctions, 0);

	registeUserdataType(L, "Lua.Socket.IPData", LuaIPDataMethods);

	// family table
	lua_newtable(L);
	// IPV4
	lua_pushinteger(L, AF_INET);
	lua_setfield(L, -2, "IPV4");

	// type table
	lua_newtable(L);
	// RAW TYPE
	lua_pushinteger(L, SOCK_RAW);
	lua_setfield(L, -2, "RAW");
	lua_pushinteger(L, SOCK_STREAM);
	lua_setfield(L, -2, "TCP");
	lua_pushinteger(L, SOCK_DGRAM);
	lua_setfield(L, -2, "UDP");
	
	// protocal table
	lua_newtable(L);
	// TCP
	lua_pushinteger(L, IPPROTO_TCP);
	lua_setfield(L, -2, "TCP");
	// UDP
	lua_pushinteger(L, IPPROTO_UDP);
	lua_setfield(L, -2, "UDP");
	// RAW
	lua_pushinteger(L, IPPROTO_RAW);
	lua_setfield(L, -2, "RAW");
	// ICMP
	lua_pushinteger(L, IPPROTO_ICMP);
	lua_setfield(L, -2, "ICMP");
	// ICMP
	lua_pushinteger(L, IPPROTO_NONE);
	lua_setfield(L, -2, "NONE");

	luaL_newlib(L, LuaSocketLib);

	// the stack state
	// top -->	lib
	//			protocal table
	//			type table
	//			family table
	//			......
	lua_pushvalue(L, -4);
	lua_setfield(L, -2, "Family");
	lua_pushvalue(L, -3);
	lua_setfield(L, -2, "Type");
	lua_pushvalue(L, -2);
	lua_setfield(L, -2, "Protocal");
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
		luaL_getmetatable(L, "Lua.Socket.Socket");
		lua_setmetatable(L, -2);
		return 1;
	}
}

int lua_closeSocket(lua_State * L)
{
	auto * pSocket = CHECK_USERDATA_SOCKET(L);
	int error = closesocket(*pSocket);
	if (error)
	{
		return doWhenFailed(L, "socket close error");
	}
	else
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_newAddress(lua_State * L)
{
	// by default us AF_INET as the famaliy
	const int		af			= AF_INET;
	const char *	ipAddress	= luaL_checkstring(L, 1);
	const int		port		= static_cast<int>(luaL_checkinteger(L, 2));

	auto * pAddr = 
		reinterpret_cast<SOCKADDR_IN*>(lua_newuserdata(L, sizeof(SOCKADDR_IN)));

	pAddr->sin_family = af;
	pAddr->sin_port = port;

	// if failed, error will be negative
	int error = inet_pton(af, ipAddress, 
		reinterpret_cast<void*>(&pAddr->sin_addr));

	if (error < 0)
	{
		return doWhenFailed(L, "Address creation error: Ip address illegal!");
	}
	else
	{
		luaL_getmetatable(L, "Lua.Socket.Address");
		lua_setmetatable(L, -2);
		return 1;
	}


	return 0;
}

int lua_bind(lua_State * L)
{
	auto * pSocket	= CHECK_USERDATA_SOCKET_INDEX(L, 1);
	auto * pAddress = CHECK_USERDATA_ADDRESS_INDEX(L, 2);

	int error = bind(*pSocket, reinterpret_cast<sockaddr*>(pAddress), sizeof(SOCKADDR_IN));
	
	if (error == SOCKET_ERROR)
	{
		return doWhenFailed(L, "Bind Failed");
	}
	else
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_listen(lua_State * L)
{
	auto * pSocket = CHECK_USERDATA_SOCKET(L);
	int maxNum = static_cast<int>(luaL_checkinteger(L, 2));

	if (listen(*pSocket, maxNum))
	{
		// if error happend
		return doWhenFailed(L, "transforming socket state to listening failed");
	}
	else
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_accept(lua_State * L)
{
	// be aware that this function will be block inside here,
	// if no client connect to it.

	auto * pSocket = CHECK_USERDATA_SOCKET(L);

	sockaddr remoteAddr;

	int addrLength = sizeof(remoteAddr);

	auto remoteSocket = accept(*pSocket, &remoteAddr, &addrLength);

	if (remoteSocket == INVALID_SOCKET)
	{
		return doWhenFailed(L, "Accepted a Invalid socket.");
	}
	else
	{
		// create a new userdata in the stack, and assigned it with the accepted socket.
		auto * pSocket =
			reinterpret_cast<SOCKET *>(lua_newuserdata(L, sizeof(SOCKET)));
		*pSocket = remoteSocket;
		// set metatable
		luaL_getmetatable(L, "Lua.Socket.Socket");
		lua_setmetatable(L, -2);

		// create a new Address in the stack, and assigned it with the accepted address.
		auto * pAddr =
			reinterpret_cast<sockaddr*>(lua_newuserdata(L, sizeof(SOCKADDR_IN)));
		*pAddr = remoteAddr;
		// setmetatable
		luaL_getmetatable(L, "Lua.Socket.Address");
		lua_setmetatable(L, -2);

		// return the remote socket and address.
		return 2;
	}
}

int lua_connect(lua_State * L)
{
	auto * pSocket			= CHECK_USERDATA_SOCKET_INDEX	(L, 1);
	auto * pTargetAddress	= CHECK_USERDATA_ADDRESS_INDEX	(L, 2);

	int error = connect(*pSocket, 
		reinterpret_cast<sockaddr*>(pTargetAddress), sizeof(SOCKADDR_IN));

	if (error)
	{
		return doWhenFailed(L, "connection failed");
	}
	else
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_send(lua_State * L)
{
	auto * pSocket	= CHECK_USERDATA_SOCKET_INDEX	(L, 1);
	auto * pNetData = CHECK_USERDATA_NETDATA_INDEX	(L, 2);

	int sLen = send(*pSocket, 
		reinterpret_cast<const char *>(pNetData->Data), pNetData->Length, 0);

	if (sLen == SOCKET_ERROR)
	{
		return doWhenFailed(L, "Send data failed");
	}
	else
	{
		lua_pushinteger(L, sLen);
		return 1;
	}
}

int lua_recv(lua_State * L)
{
	auto * pSocket = CHECK_USERDATA_SOCKET_INDEX(L, 1);
	auto * pNetData = CHECK_USERDATA_NETDATA_INDEX(L, 2);

	int rLen = recv(*pSocket, 
		reinterpret_cast<char *>(pNetData->Data), pNetData->Length - 1, 0);

	if (rLen == SOCKET_ERROR)
	{
		return doWhenFailed(L, "Recieve data failed");
	}
	else
	{
		lua_pushinteger(L, rLen);
		// put a zero at the end of the data
		pNetData->Data[rLen] = 0;
		return 1;
	}
}

int lua_sendto(lua_State * L)
{
	auto * pSocket	= CHECK_USERDATA_SOCKET_INDEX(L, 1);
	auto * pNetData = CHECK_USERDATA_NETDATA_INDEX(L, 2);
	auto * pAddress = CHECK_USERDATA_ADDRESS_INDEX(L, 3);

	int lSend = sendto(*pSocket,
		reinterpret_cast<const char *>(pNetData->Data), pNetData->Length, 0,
		reinterpret_cast<SOCKADDR*>(pAddress), sizeof(SOCKADDR));

	if (lSend == SOCKET_ERROR)
	{
		return doWhenFailed(L, "sendto failed");
	}
	else
	{
		lua_pushinteger(L, lSend);
		return 1;
	}
	return 0;
}

int lua_recvfrom(lua_State * L)
{
	auto * pSocket = CHECK_USERDATA_SOCKET_INDEX(L, 1);
	auto * pNetData = CHECK_USERDATA_NETDATA_INDEX(L, 2);
	auto * pAddress = CHECK_USERDATA_ADDRESS_INDEX(L, 3);

	int iLen = sizeof(SOCKADDR);
	int lRecv = recvfrom(*pSocket, 
		reinterpret_cast<char *>(pNetData->Data), pNetData->Length - 1, 0,
		reinterpret_cast<SOCKADDR*>(pAddress), &iLen);

	if (lRecv == SOCKET_ERROR)
	{
		return doWhenFailed(L, "recvfrom failed");
	}
	else
	{
		lua_pushinteger(L, lRecv);
		pNetData->Data[lRecv] = 0;
		return 1;
	}
	return 0;
}

int lua_recvAll(lua_State * L)
{
	auto * psocket = CHECK_USERDATA_SOCKET(L);
	DWORD dwValue = 1;
	int error;
	// leave the ip head to the user
	int flag = 1;
	error = setsockopt(*psocket, IPPROTO_IP, IP_HDRINCL,
		(char*)&flag, sizeof(flag));

	if (error == SOCKET_ERROR)
	{
		return doWhenFailed(L, "set socket to recieve all package failed");
	}

	error = ioctlsocket(*psocket, SIO_RCVALL, &dwValue);

	if (error == SOCKET_ERROR)
	{
		return doWhenFailed(L, "set socket to recieve all  failed");
	}
	else
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_newIPData(lua_State * L)
{
	auto * pnetdata = CHECK_USERDATA_NETDATA(L);
	size_t dataSize = NetData::Size(pnetdata->Length);

	auto * pnewData = newUserdataInStack<IPData>(L, "Lua.Socket.IPData", dataSize);
	// copy the memory
	memcpy_s((void*)pnewData, dataSize, (void*)pnetdata, dataSize);

	return 1;
}

int lua_IPDataSourceIP(lua_State * L)
{
	auto * ipdata = CHECK_USERDATA_IPDATA(L);
	Formater<128> buffer;
	if (nullptr == inet_ntop(AF_INET, (void*)&(ipdata->ipPackage.SourceIP), buffer.bufferPointer(), 128))
	{
		return doWhenFailed(L, "convert ip address failed, cannot return the ipaddress");
	}
	lua_pushstring(L, buffer.bufferPointer());
	return 1;
}

int lua_IPDataSetSourceIP(lua_State * L)
{
	auto * pipdata = CHECK_USERDATA_IPDATA(L);
	size_t len;
	const char * ipaddress = lua_tolstring(L, 2, &len);
	if (0 == inet_pton(AF_INET, ipaddress, (void*)&pipdata->ipPackage.SourceIP))
	{
		return doWhenFailed(L, "cannot convert the string to ipaddress");
	}
	else 
	{
		lua_pushboolean(L, true);
		return 1;
	}
}

int lua_newNetData(lua_State * L)
{
	size_t length = 0;
	const char * pbuffer = nullptr;


	// if pass in a string, then use the string to initialize the NetData
	if (LUA_TSTRING == lua_type(L, 1))
	{
		// use the string to create the netData
		pbuffer = lua_tolstring(L, 1, &length);
	}
	else
	{
		length = static_cast<size_t>(luaL_checkinteger(L, 1));
	}
	
	if (length <= 0)
	{
		return doWhenFailed(L, "the netData must be greater than 1 byte");
	}

	auto * pNetData =
		reinterpret_cast<NetData*>(lua_newuserdata(L, NetData::Size(length)));
	pNetData->Length = length;

	// initialization
	if (pbuffer)
	{
		memcpy_s((void*)pNetData->Data, length, (const void*)pbuffer, length);
	}

	luaL_getmetatable(L, "Lua.Socket.NetData");
	lua_setmetatable(L, -2);

	return 1;
}

int lua_sizeOfNetData(lua_State * L)
{
	auto * pNetData = CHECK_USERDATA_NETDATA(L);
	lua_pushinteger(L, pNetData->Length);
	return 1;
}

int lua_NetDataToString(lua_State * L)
{
	auto * pNetData = CHECK_USERDATA_NETDATA(L);
	char buffer[63555];
	strcpy_s(buffer, (char*)pNetData->Data);
	lua_pushstring(L, reinterpret_cast<const char *>(buffer));
	return 1;
}

int lua_fillNetData(lua_State * L)
{
	auto * pNetData = CHECK_USERDATA_NETDATA(L);
	const char * pstr = luaL_checkstring(L, 2);
	size_t len;
	lua_tolstring(L, 2, &len);

	if (len + 1 > pNetData->Length)
	{
		return doWhenFailed(L, "filling failed, the lenght of the string is greater than the NetData");
	}
	else
	{
		memcpy_s((void*)pNetData->Data, pNetData->Length, (void*)pstr, len + 1);
		lua_pushboolean(L, true);
		return 1;
	}
	
	return 0;
}

int lua_showInHexData(lua_State * L)
{
	//if (lua_typename)
	return 0;
}

int doWhenFailed(lua_State * L, const char * message)
{

	lua_pushboolean(L, false);
	lua_pushstring(L, message);
	lua_pushinteger(L, WSAGetLastError());
	return 3;
}
