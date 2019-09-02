
//[lua table to c](https://stackoverflow.com/questions/2219669/how-to-i-pass-a-table-from-lua-into-c)

extern "C" {
#   include "lua.h"
#   include "lualib.h"
#   include "lauxlib.h"
}

#include <assert.h>
#include "dlx.h"
#include "lua_helper.h"

static const char *kDlxName = "lynx94.dlx";
static const int kMaxColumn = 1024 * 10;

static int lnewdlx(lua_State *L) {
    int rows = luaL_checknumber(L, 1);
    int cols = luaL_checknumber(L, 2);
    assert(cols <=  kMaxColumn);

    lynx94::dlx_matrix *vd = lynx94::dlx_matrix_create(rows, cols);
    luax_pushobject(L, vd);
    luaL_getmetatable(L, kDlxName);
    lua_setmetatable(L, -2);
    return 1;
}

static int ldeletedlx(lua_State *L) {
    lynx94::dlx_matrix *vd = (lynx94::dlx_matrix *)luax_checkobject(L, 1, kDlxName);
    luaL_argcheck(L, vd != NULL, 1, "`dlx_matrix' excepted");
    lynx94::dlx_matrix_release(vd);
    return 0;
}

static int lset(lua_State *L) {
    lynx94::dlx_matrix *vd = (lynx94::dlx_matrix *)luax_checkobject(L, 1, kDlxName);
    luaL_argcheck(L, vd != NULL, 1, "`dlx_matrix' excepted");
    int row = luaL_checkinteger(L, 2)-1;
    int col = luaL_checkinteger(L, 3)-1;
    int val = luaL_checkinteger(L, 4);
    lynx94::dlx_matrix_set(vd, row, col, val != 0);
    return 0;
}

static int lget(lua_State *L) {
    lynx94::dlx_matrix *vd = (lynx94::dlx_matrix *)luax_checkobject(L, 1, kDlxName);
    luaL_argcheck(L, vd != NULL, 1, "`dlx_matrix' excepted");
    int row = luaL_checkinteger(L, 2)-1;
    int col = luaL_checkinteger(L, 3)-1;
    bool val = lynx94::dlx_matrix_get(vd, row, col);
    lua_pushinteger(L, val ? 1 : 0);
    return 1;
}

static int ldance(lua_State *L) {
    lynx94::dlx_matrix *vd = (lynx94::dlx_matrix *)luax_checkobject(L, 1, kDlxName);
    luaL_argcheck(L, vd != NULL, 1, "`dlx_matrix' excepted");
    int limits = 1;
    if (lua_gettop(L) > 1) {
        limits = luaL_checkinteger(L, 2);
    }

    lua_newtable(L);
    int index = 0;
    auto f = [&](int *result, int len) {
        index ++;
        lua_newtable(L);
        for (int i = 0; i<len; i++) {
            lua_pushinteger(L, result[i]+1);
            lua_rawseti(L, -2, i+1);
        }
        lua_rawseti(L, -2, index);
    };
    lynx94::dlx_matrix_dance(vd, limits, f);

    return 1;
}

extern "C"
int luaopen_dlx(lua_State *L) {
    static const luaL_Reg dlxlib[] = {
        {"new", lnewdlx},
        {NULL, NULL},
    };

    static const luaL_Reg metalib[] = {
        {"set",     lset},
        {"get",     lget},
        {"dance",   ldance},
        {"__gc",    ldeletedlx},
        {NULL, NULL},
    };

    luaL_newmetatable(L, kDlxName);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, metalib, 0);

    luaL_newlib(L, dlxlib);
    return 1;
}

