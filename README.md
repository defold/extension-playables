[![Build with bob](https://github.com/defold/extension-playables/actions/workflows/bob.yml/badge.svg)](https://github.com/defold/extension-playables/actions/workflows/bob.yml)

# YouTube Playables SDK Extension for Defold

This [Defold native extension](https://defold.com/manuals/extensions/) exposes the [YouTube Playables SDK](https://developers.google.com/youtube/gaming/playables/reference/sdk) to Lua.

The Lua API covers the public functions in `ytgame.game`, `ytgame.system`, `ytgame.engagement`, `ytgame.ads`, and `ytgame.health`:

```lua
local in_playables_env = playables.is_in_playables_env()
local sdk_version = playables.get_sdk_version()

playables.game_ready()
playables.load_data(function(self, data, error) end)
playables.save_data(data, function(self, success, error) end)

local audio_enabled = playables.is_audio_enabled()
playables.on_audio_enabled_change(function(self, enabled) end)
playables.on_pause(function(self) end)
playables.on_resume(function(self) end)
playables.get_language(function(self, language, error) end)

playables.send_score(score, function(self, success, error) end)
playables.open_yt_content(video_id, playables.CONTENT_TYPE_VIDEO, function(self, success, error) end)

playables.request_interstitial_ad(function(self, success, error) end)
playables.request_rewarded_ad("extra_life", function(self, reward_earned, error) end)

playables.log_error()
playables.log_warning()
```

Add the extension to a Defold project using a release archive URL:

```text
https://github.com/defold/extension-playables/archive/refs/tags/<version>.zip
```

The extension's HTML5 engine template loads the YouTube Playables SDK before the game code. The extension automatically calls `ytgame.game.firstFrameReady()` during initialization. Call `playables.game_ready()` only after the loading screen is removed and the game accepts player input.

See the [extension documentation](docs/index.md) for more information.
