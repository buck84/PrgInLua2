// DynamicLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static int Add(lua_State *L)
{
	double v1 = lua_tonumber(L, 1);
	double v2 = lua_tonumber(L, 2);
	lua_pushnumber(L, v1+v2);
	return 1;
}

static const struct luaL_Reg mylib[] = {
	{"MyAdd", Add},
	{NULL, NULL}
};

extern "C" __declspec(dllexport) int luaopen_DynamicLib(lua_State *L)
{
	luaL_register(L, "mylib", mylib);
	return 1;
}