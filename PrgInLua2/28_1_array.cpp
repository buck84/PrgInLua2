/**lua程序设计2nd源代码
lua test code:
a = array.new(1000)
print(a)
print(array.size(a))
for i=1,1000 do
	array.set(a, i, i%5==0)
end
print(array.get(a,10))
*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <limits.h>
#define BITS_PER_WORD (CHAR_BIT*sizeof(unsigned int))
#define I_WORD(i) ((unsigned int)(i)/BITS_PER_WORD)
#define I_BIT(i) (1<<((unsigned int)(i)%BITS_PER_WORD))

typedef struct NumArray
{
	int size;
	unsigned int values[1];
} NumArray;

static int newarray(lua_State *L)
{
	int i, n;
	size_t nbytes;
	NumArray *a;

	n = luaL_checkint(L, 1);
	luaL_argcheck(L, n>=1, 1, "invalid size");
	nbytes = sizeof(NumArray) + I_WORD(n-1)*sizeof(unsigned int);
	a = (NumArray*)lua_newuserdata(L, nbytes);

	a->size = n;
	for(i=0; i<=I_WORD(n-1); i++)
		a->values[i] = 0;

	luaL_getmetatable(L, "LuaBook.array");
	lua_setmetatable(L, -2);

	return 1;
}

static int setarray(lua_State *L)
{
	NumArray *a = (NumArray*)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2)-1;
	luaL_checkany(L, 3);

	luaL_argcheck(L, a!=NULL, 1, "'array' expected");
	luaL_argcheck(L, 0<=index && index<a->size, 2, "index out of range");

	if(lua_toboolean(L, 3))
		a->values[I_WORD(index)] |= I_BIT(index);
	else
		a->values[I_WORD(index)] &= ~I_BIT(index);
	return 0;
}

static int getarray(lua_State *L)
{
	NumArray *a = (NumArray*)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2)-1;
	luaL_argcheck(L, a!=NULL, 1, "'array' expected");
	luaL_argcheck(L, 0<=index &&index<a->size, 2, "index out of range");
	lua_pushboolean(L, a->values[I_WORD(index)] & I_BIT(index));
	return 1;
}

static int getsize(lua_State *L)
{
	NumArray *a = (NumArray*)lua_touserdata(L, 1);
	luaL_argcheck(L, a!=NULL, 1, "'array' expected");
	lua_pushinteger(L, a->size);
	return 1;
}

static const struct luaL_Reg arraylib[] = 
{
	{"new", newarray},
	{"set", setarray},
	{"get", getarray},
	{"size", getsize},
	{NULL, NULL},
};

int luaopen_array(lua_State *L)
{
	luaL_newmetatable(L, "LuaBook.array");
	luaL_register(L, "array", arraylib);
	return 1;
}

// 打印堆栈
void stackDump(lua_State *L)
{
	int i;
	int top = lua_gettop(L);
	for (i=1; i<=top; i++)
	{
		int t = lua_type(L, i);
		switch(t)
		{
		case LUA_TSTRING:
			{
				printf("'%s'", lua_tostring(L, i));
				break;
			}
		case LUA_TBOOLEAN:
			{
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;
			}
		case LUA_TNUMBER:
			{
				printf("%g", lua_tonumber(L, i));
				break;
			}
		default:
			{
				printf("%s", lua_typename(L, t));
				break;
			}
			printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

// main
int main()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	
	luaopen_array(L);
	if(luaL_loadfile(L, "func.lua") || lua_pcall(L, 0, 0, 0))
		printf("run func.lua error");

	lua_close(L);
	return 0;
}