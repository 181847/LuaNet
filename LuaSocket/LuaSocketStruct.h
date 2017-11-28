#pragma once

#include "../Library/MyTools/LuaTools.h"
#include "../Library/MyTools/UsefulDataType.h"
#include "../Library/winsock/mstcpip.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "WS2_32.lib")

// NetData is a Data that is used to contain the data from the net,
// and additional information about the data,
// such as byteLength.

static const UINT MaxNetDataLength = 1500;


struct NetData
{
	UINT Length;
	
	BYTE Data[1];

	inline static size_t Size(size_t bytesize) { return sizeof(UINT) + bytesize; }
};

struct IPData
{
	union {
		NetData netData;
		struct
		{
			union
			{
				BYTE Version;
				BYTE HeadLength;
			};
			BYTE TypeOfService;
			WORD TotalLenght;
			// 4 BYTE

			WORD Identify;
			union
			{
				BYTE flag;
				WORD FragmentOffset;
			};
			// 4 BYTE

			BYTE TTL;
			BYTE Protocol;
			WORD CheckSum;
			// 4 BYTE

			DWORD SourceIP;
			DWORD DestIP;
			// 4 BYTE

			// Rest data
			BYTE Data[1];
		} ipPackage;
	};
};

#define LENGTH_OF_NETDATA(byteSize) (sizeof(NetData::Length) + byteSize)

typedef SOCKET LuaSocket;


