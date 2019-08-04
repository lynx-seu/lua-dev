
// 参考
// 1. [PIL]
// 2. [绑定一个简单的C++类](http://4gamers.cn/blog/2014-08-11-bind-a-simple-cpp-class-in-lua.html)

extern "C" {
#   include "lua.h"
#   include "lualib.h"
#   include "lauxlib.h"
}

#include <vector>
#include "lua_helper.h"

typedef std::vector<double> Array;
static const char *kArrayName = "lynx94.array";

static int lnewarray(lua_State *L) {
    int n = 0;
    if (lua_gettop(L) > 0) {
        n = luaL_checknumber(L, 1);
    }

    auto vd = new Array(n);
    luax_pushobject(L, vd);
    luaL_getmetatable(L, kArrayName);
    lua_setmetatable(L, -2);
    return 1;
}

static int ldeletearray(lua_State *L) {
    auto vd = (Array *)luax_checkobject(L, 1, kArrayName);
    luaL_argcheck(L, vd != NULL, 1, "`array' excepted");
    delete vd;
    return 0;
}

static int lsetarray(lua_State *L) {
    auto vd = (Array *)luax_checkobject(L, 1, kArrayName);
    int index = luaL_checkinteger(L, 2);
    double value = luaL_checknumber(L, 3);

    luaL_argcheck(L, vd != NULL, 1, "`array' excepted");
    luaL_argcheck(L, index >= 1 && index <= vd->size(), 2, "index out of range");

    (*vd)[index-1] = value;
    return 0;
}

static int lgetarray(lua_State *L) {
    auto vd = (Array *)luax_checkobject(L, 1, kArrayName);
    int index = luaL_checkinteger(L, 2);

    luaL_argcheck(L, vd != NULL, 1, "`array' excepted");
    luaL_argcheck(L, index >= 1 && index <= vd->size(), 2, "index out of range");

    lua_pushnumber(L, (*vd)[index-1]);
    return 1;
}

static int lgetsize(lua_State *L) {
    auto vd = (Array *)luax_checkobject(L, 1, kArrayName);
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

    static const luaL_Reg metalib[] = {
        {"set", lsetarray},
        {"get", lgetarray},
        {"size", lgetsize},
        {NULL, NULL},
    };

    luaL_newmetatable(L, kArrayName);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, metalib, 0);

    luaL_newlib(L, arraylib);
    return 1;
}

