#include <iostream>
#include "window.h"

extern "C" {
#include "lauxlib.h"
#include "lualib.h"
#include "lua.h"
}


void getLuaInt(lua_State *L, const char *name, int &ret) {
    lua_getglobal(L, name);
    if (lua_isinteger(L, -1)) {
        ret = lua_tointeger(L, -1);
    }
    lua_pop(L, -1);
}


void getLuaBool(lua_State *L, const char *name, bool &ret) {
    lua_getglobal(L, name);
    if (lua_isboolean(L, -1)) {
        ret = lua_toboolean(L, -1);
    }
    lua_pop(L, -1);
}

int main() {
    lua_State *L = luaL_newstate(); /* opens Lua */
    luaL_openlibs(L); /* opens the standard libraries */

    int width = -1, height = -1;
    bool p1AI = false, p2AI = false;
    bool simpleAI = true;

    int ret = luaL_dofile(L, "lua/config.lua");
    if (ret) {
        std::cout << "load config file fail" << std::endl;
    } else {
        getLuaInt(L, "width", width);
        getLuaInt(L, "height", height);

        getLuaBool(L, "p1AI", p1AI);
        getLuaBool(L, "p2AI", p2AI);
        getLuaBool(L, "simpleAI", simpleAI);
    }

    lua_close(L);

    Window window(width, height, p1AI, p2AI, simpleAI);

    window.loop();
    return 0;
}