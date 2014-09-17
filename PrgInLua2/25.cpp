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


// 25.1
void loadWHOnly(lua_State *L, const char *fname, int *w, int *h)
{
	if(luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		printf("cannot run config. file:%s\n", lua_tostring(L, -1));

	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	if(!lua_isnumber(L, -2))
		printf("width should be a number\n");
	if(!lua_isnumber(L, -1))
		printf("height should be a number\n");
	*w = lua_tointeger(L, -2);
	*h = lua_tointeger(L, -1);
}

// 25.1
/*

a.lua
width = 3
height = 4

*/
void testWHOnly(lua_State *L)
{
	int w, h;
	loadWHOnly(L, "a.lua", &w, &h);
	printf("w = %d\n", w);
	printf("h = %d\n", h);
}

// 25.2
#define MAX_COLOR	255
struct ColorTable {
	char *name;
	unsigned char r, g, b;
} colortable[] = {
	{"WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR},
	{"RED", MAX_COLOR, 0, 0},
	{NULL, 0, 0, 0},
};

void setfield(lua_State *L, const char *index, int value)
{
	lua_pushnumber(L, (double)value/MAX_COLOR);
	lua_setfield(L, -2, index);
}

void setcolor(lua_State *L, struct ColorTable *ct)
{
	lua_newtable(L);
	setfield(L, "r", ct->r);
	setfield(L, "g", ct->g);
	setfield(L, "b", ct->b);
	lua_setglobal(L, ct->name);
}

int getfield(lua_State *L, const char *key)
{
	int result;
	//lua_pushstring(L, key);
	//lua_gettable(L, -2);
	lua_getfield(L, -1, key);
	if(!lua_isnumber(L, -1))
		printf("invalid component in color component:%s", key);
	result = lua_tonumber(L, -1)*MAX_COLOR;
	lua_pop(L, 1);
	return result;
}

/*
a.lua
width=3
height=4
BLUE = {r=0, g=0, b=1}
background = {r=0.3, g=0.1, b=0.2}
foreground = WHITE
testground = RED
*/
void testTable(lua_State *L)
{
	// 初始化
	int i=0;
	while(colortable[i].name!=NULL)
		setcolor(L, &colortable[i++]);

	// 调用脚本	
	if(luaL_loadfile(L, "a.lua") || lua_pcall(L, 0, 0, 0))
		printf("cannot run config. error:%s\n", lua_tostring(L, -1));

	lua_getglobal(L, "foreground");
	int r, g, b;
	if(lua_isstring(L, -1))
	{
		const char *colorname = lua_tostring(L, -1);
		int i;
		for(i=0; colortable[i].name!=NULL; i++)
		{
			if(strcmp(colorname, colortable[i].name)==0)
				break;
		}
		if(colortable[i].name==NULL)
			printf("invalid colorname:%s\n", colorname);
		else
		{
			r = colortable[i].r;
			g = colortable[i].g;
			b = colortable[i].b;
		}
	}
	else
	{
		r = getfield(L, "r");
		g = getfield(L, "g");
		b = getfield(L, "b");
	}
}

// 25.3
// c调用lua函数封装
double f(lua_State *L, double x, double y)
{
	double z;

	lua_getglobal(L, "f");
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);

	if(lua_pcall(L, 2, 1, 0)!=0)
		printf("error running function 'f':%s", lua_tostring(L, -1));

	if(!lua_isnumber(L, -1))
		printf("function 'f' must return a number");
	z = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return z;
}

// 25.3
// c调用lua函数测试
/*
25_3.lua

*/
void testCCallLuaFunc(lua_State *L)
{
	if(luaL_loadfile(L, "25_3.lua") || lua_pcall(L, 0, 0, 0))
		printf("run func.lua error");
	int z = f(L, 3, 2);
	printf("%d\n", z);
}

// 通用的C调用lua函数
// 例如: call_va("f", "dd>d", x, y, &z)
void call_va(lua_State *L, const char *func, const char *sig, ...)
{
	va_list vl;
	int narg, nres;
	va_start(vl, sig);
	lua_getglobal(L, func);

	// 压入参数
	bool bEnd = false;
	for(narg=0; *sig; narg++)
	{
		luaL_checkstack(L, 1, "too many arguments");
		switch (*sig++)
		{
		case 'd':
			lua_pushnumber(L, va_arg(vl, double));
			break;
		case 'i':
			lua_pushinteger(L, va_arg(vl, int));
			break;
		case 's':
			lua_pushstring(L, va_arg(vl, char*));
			break;
		case '>':
			bEnd = true;
			break;
		default:
			printf("invalid option (%c)", *(sig-1));
		}
		if(bEnd)
			break;
	}

	nres = strlen(sig);
	if(lua_pcall(L, narg, nres, 0)!=0)
	{
		printf("error calling %s:%s", func, lua_tostring(L, -1));
	}

	// 检查结果
	nres = -nres;
	while(*sig)
	{
		switch(*sig++)
		{
		case 'd':
			if(!lua_isnumber(L, nres))
				printf("wrong result type");
			*va_arg(vl, double*) = lua_tonumber(L, nres);
			break;
		case 'i':
			if(!lua_isnumber(L, nres))
				printf("wrong result type");
			*va_arg(vl, int*) = lua_tointeger(L, nres);
			break;
		case 's':
			if(!lua_isstring(L, nres))
				printf("wrong result type");
			*va_arg(vl, const char**) = lua_tostring(L, nres);
			break;
		default:
			printf("invalid option (%c)", *(sig-1));
		}
		nres++;
	}

	va_end(vl);
}

void testcall_va(lua_State *L)
{
	if(luaL_loadfile(L, "25_3.lua") || lua_pcall(L, 0, 0, 0))
		printf("run func.lua error");
	double x = 3;
	double y = 2;
	double z;
	call_va(L, "f", "dd>d", x, y, &z);
	printf("%f\n", z);
}
