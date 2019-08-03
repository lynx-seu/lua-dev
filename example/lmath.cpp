
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

int luaopen_math2(lua_State *L) {
    static const struct luaL_reg mathlib[] = {
        {"sin", lsin},
        {NULL, NULL},
    };

    lua_newtable(L);
    luaL_setfuncs(L, mathlib, 0);
    return 1;
}


