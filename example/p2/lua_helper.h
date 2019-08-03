#ifndef LUA_HELPER
#define LUA_HELPER

struct lua_State;

void luax_pushobject(lua_State *L, void *obj);
void *luax_toobject(lua_State *L,  int index);
void luax_deleteobject(lua_State *L,  void *obj);
void luax_dump(lua_State *L);

#endif
