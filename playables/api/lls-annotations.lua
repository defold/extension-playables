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

---Returns whether the SDK reports that the game is running in the Playables environment.
---Returns false if the SDK is not loaded.
---@return boolean in_playables_env
function playables.is_in_playables_env() end

---Returns the loaded YouTube Playables SDK version, or nil if it is unavailable.
---@return string|nil sdk_version
function playables.get_sdk_version() end

---Notifies YouTube that the game is ready for players to interact with.
---Call this only after the loading screen is gone and the game accepts player input.
---The extension automatically calls `firstFrameReady()` during initialization.
function playables.game_ready() end

---Loads serialized game data from YouTube.
---Only one load request may be in progress at a time. The request remains in progress until its callback returns,
---so the callback cannot start another `playables.load_data()` request.
---@param callback fun(self: userdata, data: string|nil, error: string|nil)
function playables.load_data(callback) end

---Saves serialized game data to YouTube.
---The data must be valid UTF-8 text no larger than 3 MiB. Invalid UTF-8 returns false and an error through the callback.
---Base64-encode binary data such as sys.serialize() output before saving. Only one save request may be in progress at a time.
---The request remains in progress until its callback returns, so the callback cannot start another `playables.save_data()` request.
---@param data string
---@param callback fun(self: userdata, success: boolean, error: string|nil)
function playables.save_data(data, callback) end

---Returns whether game audio is enabled in the player's YouTube settings.
---@return boolean is_audio_enabled
function playables.is_audio_enabled() end

---Registers a callback for YouTube audio-setting changes.
---A new callback replaces the previous one. Pass `nil` to unregister it.
---This function cannot be called from its own callback.
---@param callback (fun(self: userdata, is_audio_enabled: boolean))|nil
function playables.on_audio_enabled_change(callback) end

---Registers a callback for YouTube pause events.
---A new callback replaces the previous one. Pass `nil` to unregister it.
---This function cannot be called from its own callback.
---@param callback (fun(self: userdata))|nil
function playables.on_pause(callback) end

---Registers a callback for YouTube resume events.
---A new callback replaces the previous one. Pass `nil` to unregister it.
---This function cannot be called from its own callback.
---@param callback (fun(self: userdata))|nil
function playables.on_resume(callback) end

---Gets the language in the player's YouTube settings as a BCP-47 tag.
---Only one language request may be in progress at a time. The request remains in progress until its callback returns,
---so the callback cannot start another `playables.get_language()` request.
---@param callback fun(self: userdata, language: string|nil, error: string|nil)
function playables.get_language(callback) end

---Sends an integer score to YouTube.
---Only one score request may be in progress at a time. The request remains in progress until its callback returns,
---so the callback cannot start another `playables.send_score()` request.
---@param score number
---@param callback fun(self: userdata, success: boolean, error: string|nil)
function playables.send_score(score, callback) end

---Requests that YouTube open a video or another Playable.
---The content type defaults to `playables.CONTENT_TYPE_VIDEO`. Only one request may be in progress at a time.
---The request remains in progress until its callback returns, so the callback cannot start another `playables.open_yt_content()` request.
---@param content_id string
---@param content_type PlayablesContentType
---@param callback fun(self: userdata, success: boolean, error: string|nil)
---@overload fun(content_id: string, callback: fun(self: userdata, success: boolean, error: string|nil))
function playables.open_yt_content(content_id, content_type, callback) end

---Requests an interstitial ad. Success does not guarantee that an ad was shown.
---Do not use this API to grant rewards. Only one interstitial request may be in progress at a time.
---The request remains in progress until its callback returns, so the callback cannot start another request.
---@param callback fun(self: userdata, success: boolean, error: string|nil)
function playables.request_interstitial_ad(callback) end

---Requests a rewarded ad. Grant a reward only when reward_earned is true.
---A successful request returns true or false; a failed request returns nil and an error message.
---Only one rewarded request may be in progress at a time. The request remains in progress until its callback returns,
---so the callback cannot start another request. Keep handling YouTube audio, pause, and resume events during ads.
---@param reward_id string A stable ID unique to the reward type. Reuse it for each offer of that reward; do not include user data.
---@param callback fun(self: userdata, reward_earned: boolean|nil, error: string|nil)
function playables.request_rewarded_ad(reward_id, callback) end

---Reports an error occurrence to YouTube. Takes no arguments and returns no value.
---Reporting is best-effort and rate-limited, so delivery is not guaranteed.
function playables.log_error() end

---Reports a warning occurrence to YouTube. Takes no arguments and returns no value.
---Reporting is best-effort and rate-limited, so delivery is not guaranteed.
function playables.log_warning() end

return playables
