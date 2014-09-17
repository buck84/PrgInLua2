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


// 24.2.2打印堆栈
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
