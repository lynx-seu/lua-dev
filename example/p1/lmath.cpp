
#include <math.h>
extern "C" {
#   include "lua.h"
#   include "lualib.h"
#   include "lauxlib.h"
}

static int lsin(lua_State *L) {
    double n = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(n));
    return 1;
}

extern "C"
int luaopen_math2(lua_State *L) {
    static const luaL_Reg mathlib[2] = {
        {"sin", lsin},
        {NULL, NULL},
    };

    luaL_newlib(L, mathlib);
    return 1;
}


