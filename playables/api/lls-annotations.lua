---@meta playables

---Functions for interacting with the YouTube Playables SDK.
playables = {}

---@type number
playables.CONTENT_TYPE_VIDEO = nil
---@type number
playables.CONTENT_TYPE_PLAYABLE = nil

---@alias PlayablesContentType
---| `playables.CONTENT_TYPE_VIDEO`
---| `playables.CONTENT_TYPE_PLAYABLE`

---Notifies YouTube that the game has begun showing frames.
---This must be called before `playables.game_ready()`.
function playables.first_frame_ready() end

---Notifies YouTube that the game is ready for players to interact with.
---Call this only after the loading screen is gone and the game accepts player input.
function playables.game_ready() end

---Loads serialized game data from YouTube.
---Only one load request may be in progress at a time.
---@param callback fun(self: userdata, data: string|nil, error: string|nil)
function playables.load_data(callback) end

---Saves serialized game data to YouTube.
---The data must be a valid string no larger than 3 MiB. Only one save request may be in progress at a time.
---@param data string
---@param callback fun(self: userdata, success: boolean, error: string|nil)
function playables.save_data(data, callback) end

---Returns whether game audio is enabled in the player's YouTube settings.
---@return boolean is_audio_enabled
function playables.is_audio_enabled() end

---Registers a callback for YouTube audio-setting changes.
---A new callback replaces the previous one. Pass `nil` to unregister it.
---@param callback (fun(self: userdata, is_audio_enabled: boolean))|nil
function playables.on_audio_enabled_change(callback) end

---Registers a callback for YouTube pause events.
---A new callback replaces the previous one. Pass `nil` to unregister it.
---@param callback (fun(self: userdata))|nil
function playables.on_pause(callback) end

---Registers a callback for YouTube resume events.
---A new callback replaces the previous one. Pass `nil` to unregister it.
---@param callback (fun(self: userdata))|nil
function playables.on_resume(callback) end

---Gets the language in the player's YouTube settings as a BCP-47 tag.
---Only one language request may be in progress at a time.
---@param callback fun(self: userdata, language: string|nil, error: string|nil)
function playables.get_language(callback) end

---Sends an integer score to YouTube.
---Only one score request may be in progress at a time.
---@param score number
---@param callback fun(self: userdata, success: boolean, error: string|nil)
function playables.send_score(score, callback) end

---Requests that YouTube open a video or another Playable.
---The content type defaults to `playables.CONTENT_TYPE_VIDEO`. Only one request may be in progress at a time.
---@param content_id string
---@param content_type PlayablesContentType
---@param callback fun(self: userdata, success: boolean, error: string|nil)
---@overload fun(content_id: string, callback: fun(self: userdata, success: boolean, error: string|nil))
function playables.open_yt_content(content_id, content_type, callback) end

return playables
