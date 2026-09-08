#define EXTENSION_NAME playables
#define LIB_NAME "Playables"
#define MODULE_NAME "playables"

#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_HTML5)

enum PlayablesCallbackSlot
{
    CALLBACK_SLOT_LOAD_DATA = 0,
    CALLBACK_SLOT_SAVE_DATA,
    CALLBACK_SLOT_GET_LANGUAGE,
    CALLBACK_SLOT_AUDIO_ENABLED_CHANGE,
    CALLBACK_SLOT_PAUSE,
    CALLBACK_SLOT_RESUME,
    CALLBACK_SLOT_SEND_SCORE,
    CALLBACK_SLOT_OPEN_YT_CONTENT,
    CALLBACK_SLOT_COUNT
};

enum PlayablesContentType
{
    CONTENT_TYPE_VIDEO = 0,
    CONTENT_TYPE_PLAYABLE = 1
};

typedef void (*LoadDataCallback)(const char* data, int data_length);
typedef void (*SaveDataCallback)();
typedef void (*AsyncErrorCallback)(const char* error, int error_length);
typedef void (*GetLanguageCallback)(const char* language, int language_length);
typedef int (*AudioEnabledChangeCallback)(int is_audio_enabled);
typedef int (*SystemEventCallback)();
typedef void (*EngagementSuccessCallback)();

extern "C" {
    void PlayablesJs_FirstFrameReady();
    void PlayablesJs_GameReady();
    void PlayablesJs_LoadData(LoadDataCallback callback, AsyncErrorCallback error_callback);
    void PlayablesJs_SaveData(const char* data, int data_length, SaveDataCallback callback, AsyncErrorCallback error_callback);
    int PlayablesJs_IsAudioEnabled();
    void PlayablesJs_OnAudioEnabledChange(AudioEnabledChangeCallback callback);
    void PlayablesJs_OnPause(SystemEventCallback callback);
    void PlayablesJs_OnResume(SystemEventCallback callback);
    void PlayablesJs_GetLanguage(GetLanguageCallback callback, AsyncErrorCallback error_callback);
    void PlayablesJs_ClearCallbacks();
    void PlayablesJs_SendScore(double score, EngagementSuccessCallback callback, AsyncErrorCallback error_callback);
    void PlayablesJs_OpenYTContent(const char* content_id, int content_id_length, int content_type, EngagementSuccessCallback callback, AsyncErrorCallback error_callback);
}

static dmScript::LuaCallbackInfo* playables_Callbacks[CALLBACK_SLOT_COUNT] = {0x0};
static uint32_t playables_CallbackVersions[CALLBACK_SLOT_COUNT] = {0};

static const char* Playables_GetCallbackName(PlayablesCallbackSlot slot)
{
    switch (slot)
    {
        case CALLBACK_SLOT_LOAD_DATA:
            return "load_data";
        case CALLBACK_SLOT_SAVE_DATA:
            return "save_data";
        case CALLBACK_SLOT_GET_LANGUAGE:
            return "get_language";
        case CALLBACK_SLOT_AUDIO_ENABLED_CHANGE:
            return "on_audio_enabled_change";
        case CALLBACK_SLOT_PAUSE:
            return "on_pause";
        case CALLBACK_SLOT_RESUME:
            return "on_resume";
        case CALLBACK_SLOT_SEND_SCORE:
            return "send_score";
        case CALLBACK_SLOT_OPEN_YT_CONTENT:
            return "open_yt_content";
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

static void Playables_SetEventCallback(lua_State* L, int index, PlayablesCallbackSlot slot)
{
    if (lua_isnil(L, index))
    {
        ++playables_CallbackVersions[slot];
        Playables_DestroyCallback(slot);
        return;
    }

    if (lua_type(L, index) != LUA_TFUNCTION)
    {
        luaL_error(L, "playables.%s() requires a callback function or nil", Playables_GetCallbackName(slot));
        return;
    }

    ++playables_CallbackVersions[slot];
    Playables_DestroyCallback(slot);
    playables_Callbacks[slot] = dmScript::CreateCallback(L, index);
}

static dmScript::LuaCallbackInfo* Playables_GetValidCallback(PlayablesCallbackSlot slot)
{
    dmScript::LuaCallbackInfo* callback = playables_Callbacks[slot];
    if (!dmScript::IsCallbackValid(callback))
    {
        Playables_DestroyCallback(slot);
        return 0x0;
    }
    return callback;
}

static dmScript::LuaCallbackInfo* Playables_BeginEventCallback(PlayablesCallbackSlot slot, uint32_t* version)
{
    dmScript::LuaCallbackInfo* callback = Playables_GetValidCallback(slot);
    if (callback != 0x0)
    {
        *version = playables_CallbackVersions[slot];
        playables_Callbacks[slot] = 0x0;
    }
    return callback;
}

static int Playables_EndEventCallback(PlayablesCallbackSlot slot, dmScript::LuaCallbackInfo* callback, uint32_t version)
{
    if (playables_CallbackVersions[slot] == version)
    {
        playables_Callbacks[slot] = callback;
        return 1;
    }

    dmScript::DestroyCallback(callback);
    return playables_Callbacks[slot] != 0x0;
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

static void Playables_GetLanguageCallback(const char* language, int language_length)
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_GET_LANGUAGE);
    if (callback == 0x0)
    {
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);
    DM_LUA_STACK_CHECK(L, 0);

    if (dmScript::SetupCallback(callback))
    {
        lua_pushlstring(L, language, language_length);
        lua_pushnil(L);
        dmScript::PCall(L, 3, 0);
        dmScript::TeardownCallback(callback);
    }

    dmScript::DestroyCallback(callback);
}

static void Playables_GetLanguageErrorCallback(const char* error, int error_length)
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_GET_LANGUAGE);
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

static void Playables_SendScoreCallback()
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_SEND_SCORE);
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

static void Playables_SendScoreErrorCallback(const char* error, int error_length)
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_SEND_SCORE);
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

static void Playables_OpenYTContentCallback()
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_OPEN_YT_CONTENT);
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

static void Playables_OpenYTContentErrorCallback(const char* error, int error_length)
{
    dmScript::LuaCallbackInfo* callback = Playables_TakeCallback(CALLBACK_SLOT_OPEN_YT_CONTENT);
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

static int Playables_AudioEnabledChangeCallback(int is_audio_enabled)
{
    uint32_t version = 0;
    dmScript::LuaCallbackInfo* callback = Playables_BeginEventCallback(CALLBACK_SLOT_AUDIO_ENABLED_CHANGE, &version);
    if (callback == 0x0)
    {
        return 0;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);
    DM_LUA_STACK_CHECK(L, 0);

    if (dmScript::SetupCallback(callback))
    {
        lua_pushboolean(L, is_audio_enabled);
        dmScript::PCall(L, 2, 0);
        dmScript::TeardownCallback(callback);
    }

    return Playables_EndEventCallback(CALLBACK_SLOT_AUDIO_ENABLED_CHANGE, callback, version);
}

static int Playables_PauseCallback()
{
    uint32_t version = 0;
    dmScript::LuaCallbackInfo* callback = Playables_BeginEventCallback(CALLBACK_SLOT_PAUSE, &version);
    if (callback == 0x0)
    {
        return 0;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);
    DM_LUA_STACK_CHECK(L, 0);

    if (dmScript::SetupCallback(callback))
    {
        dmScript::PCall(L, 1, 0);
        dmScript::TeardownCallback(callback);
    }

    return Playables_EndEventCallback(CALLBACK_SLOT_PAUSE, callback, version);
}

static int Playables_ResumeCallback()
{
    uint32_t version = 0;
    dmScript::LuaCallbackInfo* callback = Playables_BeginEventCallback(CALLBACK_SLOT_RESUME, &version);
    if (callback == 0x0)
    {
        return 0;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);
    DM_LUA_STACK_CHECK(L, 0);

    if (dmScript::SetupCallback(callback))
    {
        dmScript::PCall(L, 1, 0);
        dmScript::TeardownCallback(callback);
    }

    return Playables_EndEventCallback(CALLBACK_SLOT_RESUME, callback, version);
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

static int Playables_IsAudioEnabled(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    lua_pushboolean(L, PlayablesJs_IsAudioEnabled());
    return 1;
}

static int Playables_OnAudioEnabledChange(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    Playables_SetEventCallback(L, 1, CALLBACK_SLOT_AUDIO_ENABLED_CHANGE);
    PlayablesJs_OnAudioEnabledChange(lua_isnil(L, 1) ? 0x0 : (AudioEnabledChangeCallback)Playables_AudioEnabledChangeCallback);
    return 0;
}

static int Playables_OnPause(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    Playables_SetEventCallback(L, 1, CALLBACK_SLOT_PAUSE);
    PlayablesJs_OnPause(lua_isnil(L, 1) ? 0x0 : (SystemEventCallback)Playables_PauseCallback);
    return 0;
}

static int Playables_OnResume(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    Playables_SetEventCallback(L, 1, CALLBACK_SLOT_RESUME);
    PlayablesJs_OnResume(lua_isnil(L, 1) ? 0x0 : (SystemEventCallback)Playables_ResumeCallback);
    return 0;
}

static int Playables_GetLanguage(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    Playables_SetCallback(L, 1, CALLBACK_SLOT_GET_LANGUAGE);
    PlayablesJs_GetLanguage((GetLanguageCallback)Playables_GetLanguageCallback, (AsyncErrorCallback)Playables_GetLanguageErrorCallback);
    return 0;
}

static int Playables_SendScore(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    lua_Number score = luaL_checknumber(L, 1);
    Playables_SetCallback(L, 2, CALLBACK_SLOT_SEND_SCORE);
    PlayablesJs_SendScore(score, (EngagementSuccessCallback)Playables_SendScoreCallback, (AsyncErrorCallback)Playables_SendScoreErrorCallback);
    return 0;
}

static int Playables_OpenYTContent(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    size_t content_id_length = 0;
    const char* content_id = luaL_checklstring(L, 1, &content_id_length);

    int content_type = CONTENT_TYPE_VIDEO;
    int callback_index = 2;
    if (lua_isnil(L, 2))
    {
        callback_index = 3;
    }
    else if (!lua_isfunction(L, 2))
    {
        content_type = luaL_checkint(L, 2);
        callback_index = 3;
    }

    if (content_type != CONTENT_TYPE_VIDEO && content_type != CONTENT_TYPE_PLAYABLE)
    {
        return luaL_error(L, "playables.open_yt_content() content type must be CONTENT_TYPE_VIDEO or CONTENT_TYPE_PLAYABLE");
    }

    Playables_SetCallback(L, callback_index, CALLBACK_SLOT_OPEN_YT_CONTENT);
    PlayablesJs_OpenYTContent(content_id, (int)content_id_length, content_type, (EngagementSuccessCallback)Playables_OpenYTContentCallback, (AsyncErrorCallback)Playables_OpenYTContentErrorCallback);
    return 0;
}

static const luaL_reg Module_methods[] =
{
    {"first_frame_ready", Playables_FirstFrameReady},
    {"game_ready", Playables_GameReady},
    {"load_data", Playables_LoadData},
    {"save_data", Playables_SaveData},
    {"is_audio_enabled", Playables_IsAudioEnabled},
    {"on_audio_enabled_change", Playables_OnAudioEnabledChange},
    {"on_pause", Playables_OnPause},
    {"on_resume", Playables_OnResume},
    {"get_language", Playables_GetLanguage},
    {"send_score", Playables_SendScore},
    {"open_yt_content", Playables_OpenYTContent},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pushnumber(L, CONTENT_TYPE_VIDEO);
    lua_setfield(L, -2, "CONTENT_TYPE_VIDEO");
    lua_pushnumber(L, CONTENT_TYPE_PLAYABLE);
    lua_setfield(L, -2, "CONTENT_TYPE_PLAYABLE");

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
    PlayablesJs_ClearCallbacks();
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
