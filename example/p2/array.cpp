
extern "C" {
#   include "lua.h"
#   include "lualib.h"
#   include "lauxlib.h"
}

#include <vector>
#include "lua_helper.h"

typedef Array std::vector<double>

static int lnewarray(lua_State *L) {
    int n = 0;
    if (lua_gettop(L) > 0) {
        n = luaL_checknumber(L, 1);
    }

    auto vd = new Array(n);
    luax_pushobject(L, vd);
    return 1;
}

static int lsetarray(lua_State *L) {
    auto vd = (Array *)luax_toobject(L, 1);
    int index = luaL_checkinteger(L, 2);
    dobule value = luaL_checknumber(L, 3);

    luaL_argcheck(L, vd != NULL, 1, "`array' excepted");
    luaL_argcheck(L, index >= 1 && index <= vd->size(), 2, "index out of range");

    (*vd)[index-1] = value;
    return 0;
}

static int lgetarray(lua_State *L) {
    auto vd = (Array *)luax_toobject(L, 1);
    int index = luaL_checkinteger(L, 2);

    luaL_argcheck(L, vd != NULL, 1, "`array' excepted");
    luaL_argcheck(L, index >= 1 && index <= vd->size(), 2, "index out of range");

    lua_pushnumber(L, (*vd)[index-1]);
    return 1;

}

static int lgetsize(lua_State *L) {
    auto vd = (Array *)luax_toobject(L, 1);
    luaL_argcheck(L, vd != NULL, 1, "`array' excepted");

    lua_pushnumber(L, vd->size());
    return 1;

}

extern "C"
int luaopen_array(lua_State *L) {
    static const luaL_Reg arraylib[] = {
        {"new", lnewarray},
        {NULL, NULL},
    };

    luaL_newmetatable(L, "lynx94.array");
    luaL_newlib(L, arraylib);
    return 1;
}

