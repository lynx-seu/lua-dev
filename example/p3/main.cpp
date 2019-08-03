
extern "C" {
#   include <lua.h>
#   include <lauxlib.h>
#   include <lualib.h>
}
#include "lua_helper.h"

struct Point {
    int x;
    int y;
};

int main(int argc, char const* argv[])
{
    lua_State *L = luaL_newstate();
    luax_dump(L);

    Point *pa = new(Point);
    pa->x = 1;
    pa->y = 2;

    luax_pushobject(L, pa);
    luax_deleteobject(L, pa);
    printf("Point a = (%d, %d)\n", pa->x, pa->y);
    return 0;
}
