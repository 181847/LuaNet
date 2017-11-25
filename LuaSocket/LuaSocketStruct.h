#pragma once

#include "../Library/MyTools/LuaTools.h"
#include "../Library/MyTools/UsefulDataType.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "WS2_32.lib")

// NetData is a Data that is used to contain the data from the net,
// and additional information about the data,
// such as byteLength.

static const UINT MaxNetDataLength = 1500;

struct NetData
{
	BYTE Length;
	
	BYTE Data[1];
};

typedef SOCKET LuaSocket;


