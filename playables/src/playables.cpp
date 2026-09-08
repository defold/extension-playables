#define EXTENSION_NAME playables
#define LIB_NAME "Playables"
#define MODULE_NAME "playables"

#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_HTML5)

enum PlayablesCallbackSlot
{
    CALLBACK_SLOT_LOAD_DATA = 0,
    CALLBACK_SLOT_SAVE_DATA,
    CALLBACK_SLOT_COUNT
};

typedef void (*LoadDataCallback)(const char* data, int data_length);
typedef void (*SaveDataCallback)();
typedef void (*AsyncErrorCallback)(const char* error, int error_length);

extern "C" {
    void PlayablesJs_FirstFrameReady();
    void PlayablesJs_GameReady();
    void PlayablesJs_LoadData(LoadDataCallback callback, AsyncErrorCallback error_callback);
    void PlayablesJs_SaveData(const char* data, int data_length, SaveDataCallback callback, AsyncErrorCallback error_callback);
}

static dmScript::LuaCallbackInfo* playables_Callbacks[CALLBACK_SLOT_COUNT] = {0x0};

static const char* Playables_GetCallbackName(PlayablesCallbackSlot slot)
{
    switch (slot)
    {
        case CALLBACK_SLOT_LOAD_DATA:
            return "load_data";
        case CALLBACK_SLOT_SAVE_DATA:
            return "save_data";
        case CALLBACK_SLOT_COUNT:
        default:
            return "unknown";
    }
}

static void Playables_DestroyCallback(PlayablesCallbackSlot slot)
{
    if (playables_Callbacks[slot] != 0x0)
    {
        dmScript::DestroyCallback(playables_Callbacks[slot]);
        playables_Callbacks[slot] = 0x0;
    }
}

static void Playables_SetCallback(lua_State* L, int index, PlayablesCallbackSlot slot)
{
    if (lua_type(L, index) != LUA_TFUNCTION)
    {
        luaL_error(L, "playables.%s() requires a callback function", Playables_GetCallbackName(slot));
        return;
    }

    if (playables_Callbacks[slot] != 0x0)
    {
        luaL_error(L, "playables.%s() already has a request in progress", Playables_GetCallbackName(slot));
        return;
    }

    playables_Callbacks[slot] = dmScript::CreateCallback(L, index);
}

static dmScript::LuaCallbackInfo* Playables_TakeCallback(PlayablesCallbackSlot slot)
{
    dmScript::LuaCallbackInfo* callback = playables_Callbacks[slot];
    playables_Callbacks[slot] = 0x0;
    if (!dmScript::IsCallbackValid(callback))
    {
        dmLogError("playables.%s() callback is invalid", Playables_GetCallbackName(slot));
        if (callback != 0x0)
        {
            dmScript::DestroyCallback(callback);
        }
        return 0x0;
    }
    return callback;
}

static void Playables_LoadDataCallback(const char* data, int data_length)
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_LOAD_DATA);
    if (callback == 0x0)
    {
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);
    DM_LUA_STACK_CHECK(L, 0);

    if (dmScript::SetupCallback(callback))
    {
        lua_pushlstring(L, data, data_length);
        lua_pushnil(L);
        dmScript::PCall(L, 3, 0);
        dmScript::TeardownCallback(callback);
    }

    dmScript::DestroyCallback(callback);
}

static void Playables_LoadDataErrorCallback(const char* error, int error_length)
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_LOAD_DATA);
    if (callback == 0x0)
    {
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);
    DM_LUA_STACK_CHECK(L, 0);

    if (dmScript::SetupCallback(callback))
    {
        lua_pushnil(L);
        lua_pushlstring(L, error, error_length);
        dmScript::PCall(L, 3, 0);
        dmScript::TeardownCallback(callback);
    }

    dmScript::DestroyCallback(callback);
}

static void Playables_SaveDataCallback()
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_SAVE_DATA);
    if (callback == 0x0)
    {
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);
    DM_LUA_STACK_CHECK(L, 0);

    if (dmScript::SetupCallback(callback))
    {
        lua_pushboolean(L, true);
        lua_pushnil(L);
        dmScript::PCall(L, 3, 0);
        dmScript::TeardownCallback(callback);
    }

    dmScript::DestroyCallback(callback);
}

static void Playables_SaveDataErrorCallback(const char* error, int error_length)
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_SAVE_DATA);
    if (callback == 0x0)
    {
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);
    DM_LUA_STACK_CHECK(L, 0);

    if (dmScript::SetupCallback(callback))
    {
        lua_pushboolean(L, false);
        lua_pushlstring(L, error, error_length);
        dmScript::PCall(L, 3, 0);
        dmScript::TeardownCallback(callback);
    }

    dmScript::DestroyCallback(callback);
}

static int Playables_FirstFrameReady(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    PlayablesJs_FirstFrameReady();
    return 0;
}

static int Playables_GameReady(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    PlayablesJs_GameReady();
    return 0;
}

static int Playables_LoadData(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    Playables_SetCallback(L, 1, CALLBACK_SLOT_LOAD_DATA);
    PlayablesJs_LoadData((LoadDataCallback)Playables_LoadDataCallback, (AsyncErrorCallback)Playables_LoadDataErrorCallback);
    return 0;
}

static int Playables_SaveData(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    size_t data_length = 0;
    const char* data = luaL_checklstring(L, 1, &data_length);
    Playables_SetCallback(L, 2, CALLBACK_SLOT_SAVE_DATA);
    PlayablesJs_SaveData(data, (int)data_length, (SaveDataCallback)Playables_SaveDataCallback, (AsyncErrorCallback)Playables_SaveDataErrorCallback);
    return 0;
}

static const luaL_reg Module_methods[] =
{
    {"first_frame_ready", Playables_FirstFrameReady},
    {"game_ready", Playables_GameReady},
    {"load_data", Playables_LoadData},
    {"save_data", Playables_SaveData},
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

static dmExtension::Result FinalizePlayables(dmExtension::Params* params)
{
    for (int slot = 0; slot < CALLBACK_SLOT_COUNT; ++slot)
    {
        Playables_DestroyCallback((PlayablesCallbackSlot)slot);
    }
    return dmExtension::RESULT_OK;
}

#else // unsupported platforms

static dmExtension::Result InitializePlayables(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizePlayables(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

#endif

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializePlayables, 0, 0, FinalizePlayables)
