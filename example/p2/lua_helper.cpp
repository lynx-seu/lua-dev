
// 参考[为 Lua 绑定 C/C++ 对象](https://blog.codingnow.com/2013/01/binding_c_object_for_lua.html)

extern "C" {
#   include "lua.h"
#   include "lualib.h"
#   include "lauxlib.h"
}

#include <assert.h>
#include <stdio.h>
#include "lua_helper.h"

static const char *kMetaTable = "luax_script";

void luax_pushobject(lua_State *L, void *obj) {
    void **ud;
    if (luaL_newmetatable(L, kMetaTable)) {
        // 在注册表中创建一个表存放所有的 object 指针到 userdata 的关系。
        // 这个表应该是一个 weak table ，当 Lua 中不再存在对 C 对象的引用会删除对应的记录。
        lua_newtable(L);
        lua_pushliteral(L, "kv");
        lua_setfield(L, -2, "__mode");
        lua_setmetatable(L, -2);
    }

    lua_rawgetp(L, -1, obj);
    if (lua_type(L, -1) == LUA_TUSERDATA) {
        ud = (void **)lua_touserdata(L, -1);
        if (*ud == obj) {
            lua_replace(L, -2);
            return;
        }
        // C 对象指针被释放后，有可能地址被重用。
        // 这个时候，可能取到曾经保存起来的 userdata ，里面的指针必然为空。
        assert(*ud == NULL);
    }

    ud = (void **)lua_newuserdata(L, sizeof(void *));
    *ud = obj;
    lua_pushvalue(L, -1);
    lua_rawsetp(L, -4, obj);
    lua_replace(L, -3);
    lua_pop(L, 1);
}

void *luax_toobject(lua_State *L, int index) {
    void **ud = (void **)lua_touserdata(L, index);
    if (ud == NULL) {
        return NULL;
    }
    return *ud;
}

void *luax_checkobject(lua_State *L, int index, const char *tname) {
    void **ud = (void **)luaL_checkudata(L, index, tname);
    if (ud == NULL) {
        return NULL;
    }
    return *ud;
}

void luax_deleteobject(lua_State *L,  void *obj) {
    luaL_getmetatable(L, kMetaTable);
    if (lua_istable(L, -1)) {
        lua_rawgetp(L, -1, obj);
        if (lua_type(L,-1) == LUA_TUSERDATA) {
            void **ud = (void **)lua_touserdata(L,-1);
            // 这个 assert 防止 deleteobject 被重复调用。
            assert(*ud == obj);
            // 销毁一个被 Lua 引用住的对象，只需要把 *ud 置为 NULL 。
            *ud = NULL;
        }
        lua_pop(L,2);
    } else {
        // 有可能从未调用过 pushobject ，此时注册表中 script 项尚未建立。
        lua_pop(L, 1);
    }
}

void luax_dump(lua_State *L) {
    printf("----- dump lua stack -----\n");
    int i = 0;
    int top = lua_gettop(L);

    for (i = 1; i <= top; i++) {
        int t = lua_type(L, i);
        printf("\t%s", lua_typename(L, t));
        switch(t) {
        case LUA_TSTRING:
            printf("\t\t%s\n", lua_tostring(L, i));
            break;
        case LUA_TBOOLEAN:
            printf("\t\t%s\n", lua_toboolean(L, i) ? "true" : "false");
            break;
        case LUA_TNUMBER:
            printf("\t\t%g\n", lua_tonumber(L, i));
            break;
        default:
            printf("\n");
            break;
        }
    }
    printf("\n");
}

