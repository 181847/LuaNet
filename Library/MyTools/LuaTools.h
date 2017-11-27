#pragma once
#include "DLLTools.h"
#include "MyAssert.h"
#include "MyTools.h"
#include "LuaHeadFiles.h"
#include <functional>

// this head file contain some tools for writing c module for lua

// next two marco to get the userdata and reinterpret the void pointer
#define CHECK_USERDATA_INDEX(L, INDEX, USERDATATYPE, METATABLE) \
	(reinterpret_cast<USERDATATYPE*>(luaL_checkudata(L, INDEX, METATABLE)))

#define CHECK_USERDATA(L, USERDATATYPE, METATABLE) \
	CHECK_USERDATA_INDEX(L, 1, USERDATATYPE, METATABLE)

// new userdata type function to be called in the 
// luaopen_****(lua_State * L)
// the method parameter can be nullptr.
inline void registeUserdataType(lua_State * L, const char * metatable,
	const luaL_Reg * methods = nullptr)
{
	luaL_newmetatable(L, metatable);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	if (methods)
	{
		luaL_setfuncs(L, methods, 0);
	}
}

// create a specific userdatatype in the lua_State
template<typename USERDATATYPE>
inline USERDATATYPE* newUserdataInStack(lua_State * L, const char * metatable = nullptr)
{
	auto * puserdata = reinterpret_cast<USERDATATYPE *>(
		lua_newuserdata(L, sizeof(USERDATATYPE)));

	if (metatable)
	{
		luaL_getmetatable(L, metatable);
		lua_setmetatable(L, -2);
	}

	return puserdata;
}


// this is the container which store the pointer of the user.
template<typename T>
struct LuaPointerContainer
{
public:
	T * pointer;

	void setPointer(T * p)
	{
		pointer = p;
	}

	T * operator -> () const
	{
		return pointer;
	}

	void freePointer()
	{
		if (pointer)
		{
			delete pointer;
		}
	}
};
