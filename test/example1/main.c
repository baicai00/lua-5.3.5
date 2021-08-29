#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int main()
{
    lua_State *L = luaL_newstate();

    luaL_openlibs(L);

    luaL_dofile(L, "test.lua");

    lua_getglobal(L, "Add");
    lua_pushinteger(L, 5);
    lua_pushinteger(L, 6);
    lua_pcall(L, 2, 1, 0);
    if (lua_isnumber(L, -1) == 1) {
        printf("Add ret=%d\n", (int)lua_tointeger(L, -1));
    }

    lua_close(L);

    return 0;
}