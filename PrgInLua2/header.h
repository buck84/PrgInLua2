/**lua程序设计2nd源代码
*/

// common
extern void stackDump(lua_State *L);
extern void call_va(lua_State *L, const char *func, const char *sig, ...);

// 25
extern void testTable(lua_State *L);
extern void testCCallLuaFunc(lua_State *L);
extern void call_va(lua_State *L, const char *func, const char *sig, ...);
extern void testcall_va(lua_State *L);

// 26
extern void testCFunc(lua_State *L);

// 27
extern void testMapFunc(lua_State *L);
extern void testSplitFunc(lua_State *L);