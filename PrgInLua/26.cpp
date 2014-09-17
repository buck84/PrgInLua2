/**lua程序设计2nd源代码
*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <math.h>

static int l_sin(lua_State *L)
{
	double d = lua_tonumber(L, 1);
	double dsin = sin(d);
	lua_pushnumber(L, dsin);
	return 1;
}

static int l_dir(lua_State*L)
{
	return 1;
}

void initCFunc(lua_State *L)
{
	lua_pushcfunction(L, l_sin);
	lua_setglobal(L, "mysin");
	lua_pushcfunction(L, l_dir);
	lua_setglobal(L, "mydir");
}

/*
local a=0.707
b = mysin(a)
*/
void testCFunc(lua_State *L)
{
	initCFunc(L);
	
	if(luaL_loadfile(L, "26.lua") || lua_pcall(L, 0, 0, 0))
		printf("cannot run config. file:%s\n", lua_tostring(L, -1));

	lua_getglobal(L, "b");
	if(!lua_isnumber(L, -1))
		printf("height should be a number\n");
	double b = lua_tonumber(L, -1);
	printf("sin func return:%f\n", b);
}