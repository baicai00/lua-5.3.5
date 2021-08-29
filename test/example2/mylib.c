#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int mylib_add(lua_State *L)
{
    int n = lua_gettop(L); // 这样可以获取到函数的参数个数
    //栈中索引为1的元素为函数的第一个参数,top处的元素为函数最后一个参数
    for (int i = 1; i <= n; i++) {
        if (lua_isnumber(L, i) == 1) {
            printf("第%d个参数:%d\n", i, (int)lua_tointeger(L, i));
        }
    }
    // 将返回值按顺序压入栈中
    lua_pushinteger(L, 10);//第一个返回值
    lua_pushinteger(L, 11);//第二个返回值
    lua_pushinteger(L, 12);//第三个返回值
    return 3; // 返回3表示该函数的返回值个数为3个
}

static int lib2_sub(lua_State *L)
{
    // 获取第二个upvalue,即key为"test_key"的upvalue
    int * p = lua_touserdata(L, lua_upvalueindex(1));
    int v = *p;
    printf("[lib2_sub] upvalue 1 is:%d\n", v);
    *p = v+1;
    return 0;
}

static int lib2_func1(lua_State *L)
{
    // 获取第二个upvalue,即key为"test_key_2"的upvalue
    int * p = lua_touserdata(L, lua_upvalueindex(2));
    int v = *p;
    printf("[lib2_func1] upvalue 2 is:%d\n", v);
    *p = v+1;
    return 0;
}

int luaopen_mylib(lua_State *L)
{
    luaL_Reg mylib[] = {
        {"l_add", mylib_add},//在lua脚本中调用l_add函数时,实际是在调用mylib_add
        {NULL, NULL} //数组结尾
    };

    luaL_Reg lib2[] = {
        {"l_sub", lib2_sub},
        {"l_func1", lib2_func1},
        {NULL, NULL}
    };

    int *p = (int*)malloc(sizeof(int));
    int *q = (int*)malloc(sizeof(int));
    *p = 10;
    *q = 100;
    lua_pushlightuserdata(L, p);
    lua_setfield(L, LUA_REGISTRYINDEX, "test_key");
    lua_pushlightuserdata(L, q);
    lua_setfield(L, LUA_REGISTRYINDEX, "test_key_2");
    //free(p);

    // 创建表并压入栈顶,该表用于存放C模块的导出函数
    lua_createtable(L, 0, sizeof(mylib)/sizeof(mylib[0]) + sizeof(lib2)/sizeof(lib2[0]) - 2);
    lua_getfield(L, LUA_REGISTRYINDEX, "test_key");
    lua_getfield(L, LUA_REGISTRYINDEX, "test_key_2");

    // 有upvalue的函数需要先注册,因为luaL_setfuncs调用结束后会将所有的upvalue从栈顶移除
    luaL_setfuncs(L, lib2, 2);
    // 将数组mylib中的函数注册到栈顶的表中
    luaL_setfuncs(L, mylib, 0);
    // 返回值为1,表示该函数有一个返回值,即使用lua_createtable创建的表
    return 1;
}