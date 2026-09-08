#define EXTENSION_NAME playables
#define LIB_NAME "Playables"
#define MODULE_NAME "playables"

#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_HTML5)

extern "C" {
    void PlayablesJs_GameReady();
}

static int Playables_GameReady(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    PlayablesJs_GameReady();
    return 0;
}

static const luaL_reg Module_methods[] =
{
    {"game_ready", Playables_GameReady},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    luaL_register(L, MODULE_NAME, Module_methods);
    lua_pop(L, 1);

    assert(top == lua_gettop(L));
}

static dmExtension::Result InitializePlayables(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    return dmExtension::RESULT_OK;
}

#else // unsupported platforms

static dmExtension::Result InitializePlayables(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

#endif

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializePlayables, 0, 0, 0)
