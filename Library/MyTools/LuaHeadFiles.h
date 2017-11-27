// this head file is to include some useful lua head file
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef THREAD_SAFTY
#include "../Lua/Lua-5.3.4/src/lua.h"
#include "../Lua/Lua-5.3.4/src/lauxlib.h"
#include "../Lua/Lua-5.3.4/src/lualib.h"
#elif THREAD_SAFTY
#include "../Lua/Lua-5.3.4/srcThreadSafty/lua.h"
#include "../Lua/Lua-5.3.4/srcThreadSafty/lauxlib.h"
#include "../Lua/Lua-5.3.4/srcThreadSafty/lualib.h"
#endif
#ifdef __cplusplus
}
#endif