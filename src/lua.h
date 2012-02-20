#ifndef _AKUMA_LUA_H_
#define _AKUMA_LUA_H_

typedef int (*lua_CFunction) (lua_State *L);
extern "C"
{
	const char *(lua_tolstring) (lua_State *L, int idx, size_t *len);
	lua_CFunction (lua_atpanic) (lua_State *L, lua_CFunction panicf);
}

#endif