#include <External_Deps/lua/lua.hpp>

#include <iostream>

static int pmain(lua_State *L)
{
    auto var = lua_tointeger(L, 1);
    luaL_openlibs(L);

    printf("%lld\n", var);
    return 0;
}

int main()
{
    auto&& L = luaL_newstate();
    if (nullptr == L)
    {
        return 1;
    }
    lua_pushcfunction(L, &pmain);
    lua_pushinteger(L, 100500);
    auto status = lua_pcall(L, 1, 0, 0);
    if (status != LUA_OK)
    {
        const auto&& msg = lua_tostring(L, -1);
        std::cout << msg << std::endl;
        lua_pop(L, 1);
    }

    lua_close(L);
    return 0;
}

