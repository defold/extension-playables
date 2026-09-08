---@meta playables

---Functions for interacting with the YouTube Playables SDK.
playables = {}

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

return playables
