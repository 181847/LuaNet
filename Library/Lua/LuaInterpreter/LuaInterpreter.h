#pragma once

#include "../../MyTools/LuaTools.h"

#define Not(x) (!x)

#pragma comment (lib, "Lua.lib")

#ifdef LUAINTERPRETER_EXPORTS
#define LUAINTERPRETER_API __declspec(dllexport)
#else
#ifdef LUAINTERPRETER_IMPORTS
#define LUAINTERPRETER_API __declspec(dllimport)
#endif
#endif

class LUAINTERPRETER_API LuaInterpreter
{
public:
	LuaInterpreter();
	LuaInterpreter(lua_State * L, bool isMainThread = false);
	~LuaInterpreter();

	DELETE_COPY_CONSTRUCTOR(LuaInterpreter)

	// start running 
	void Run();

	int GetStackSize();

	// get the top element as a number,
	// throw SimpleException if it is not a number,
	// only if there is no error,
	// the number will be pop from the stack.
	template<typename NUMBER_TYPE = lua_Number>
	NUMBER_TYPE ToNumberAndPop();

	// get the top element as integer,
	// throw SimpleException if it is not a number,
	// only if there is no error,
	// the integer will be pop from the stack.
	template<typename INTEGER_TYPE = lua_Integer>
	INTEGER_TYPE ToIntegerAndPop();

	// get the top element as string,
	// throw SimpleException if it is not a String or 
	// the BufferSize is less than the string length - 1,
	// copy the string into the buffer,
	// then pop the string from the stack.
	template<int BufferSize>
	void ToStringAndClear(char * buffer);

	// getField on the top element.
	void GetFieldOnTop(const char * key);
	// get indexed field of the top 
	void GetIndexOnTop(const lua_Integer index);

	// pop one element on the top of the stack
	void Pop();

	// load and call the file,
	// throws SimpleException whenever there is an error.
	LuaInterpreter* DoFile(const char * file);

	// can use lambda to directly control the lua_State;
	void Do(std::function<void(lua_State * L)> func);

	bool IsNil();


	// use a outer function to convert the userData(void*)
	// to the specific pointer type.
	template<typename USERDATA_TYPE>
	USERDATA_TYPE *
	ToUserDataAndClear(
		const char * metaTable, 
		std::function<USERDATA_TYPE*(void*)> converter);

public:
	bool stop = false;
	bool isMainThread = false;
	lua_State * m_L;

private:
	// use to store the command line.
	char buffer[255];
	int error;
};

template<typename NUMBER_TYPE>
inline NUMBER_TYPE LuaInterpreter::ToNumberAndPop()
{
	ASSERT(m_L);
	int isNum = 0;
	auto ret = lua_tonumberx(m_L, -1, &isNum);
	ThrowIfFalse(isNum);
	lua_pop(m_L, 1);
	return static_cast<NUMBER_TYPE>(ret);
}

template<typename INTEGER_TYPE>
inline INTEGER_TYPE LuaInterpreter::ToIntegerAndPop()
{
	ASSERT(m_L);
	int isNum = 0;
	auto ret = lua_tointegerx(m_L, -1, &isNum);
	ThrowIfFalse(isNum);
	lua_pop(m_L, 1);
	return static_cast<INTEGER_TYPE>(ret);
}

template<int BufferSize>
inline void LuaInterpreter::ToStringAndClear(char * buffer)
{
	ASSERT(m_L);
	size_t stringLen;
	const char * tempString = lua_tolstring(m_L, -1, &stringLen);

	// if tempString is nullptr, then throw exception
	ThrowIfFalse(tempString);
	ThrowIfFalse(BufferSize > stringLen + 1);

	strcpy_s(buffer, BufferSize, tempString);
	lua_pop(m_L, 1);
}

template<typename USERDATA_TYPE>
inline USERDATA_TYPE *
LuaInterpreter::ToUserDataAndClear(
	const char * metaTableName, 
	std::function<USERDATA_TYPE*(void*)> converter)
{
	void * pointer = lua_touserdata(m_L, -1);
	ThrowIfFalse(pointer);
	Pop();
	// pop the userData
	return converter(pointer);
}
