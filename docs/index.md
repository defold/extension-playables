---
title: Defold YouTube Playables SDK extension API documentation
brief: This manual covers how to integrate and use the YouTube Playables SDK in Defold.
---

# Defold YouTube Playables SDK extension

This [Defold native extension](https://defold.com/manuals/extensions/) provides Lua access to the [YouTube Playables SDK](https://developers.google.com/youtube/gaming/playables/reference/sdk).

## Installation

Add a released version of the extension to the dependencies in `game.project`:

```text
https://github.com/defold/extension-playables/archive/refs/tags/<version>.zip
```

Then select **Project → Fetch Libraries** in the Defold editor.

The extension includes the required synchronous SDK import in its HTML5 engine template:

```html
<script src="https://www.youtube.com/game_api/v1"></script>
```

## Game lifecycle

Call `playables.first_frame_ready()` when the game has begun showing frames. YouTube does not show the game to players until this is reported:

```lua
playables.first_frame_ready()
```

Call `playables.game_ready()` after the game has finished loading, the loading screen is no longer visible, and the player can interact with the game:

```lua
playables.game_ready()
```

This calls `ytgame.game.gameReady()` and does not return a value.

::: important
`playables.first_frame_ready()` must be called before `playables.game_ready()`.
:::

## Load data

`playables.load_data(callback)` loads the game's serialized data string from YouTube. The callback receives the data and `nil` on success, or `nil` and an error message on failure:

```lua
playables.load_data(function(self, data, error)
	if error then
		print("Unable to load data:", error)
		return
	end

	print("Loaded data:", data)
end)
```

Only one `load_data()` request may be in progress at a time.

## Save data

`playables.save_data(data, callback)` saves a serialized string to YouTube. The callback receives `true` and `nil` on success, or `false` and an error message on failure:

```lua
local data = json.encode({ level = 3, score = 1200 })

playables.save_data(data, function(self, success, error)
	if not success then
		print("Unable to save data:", error)
		return
	end

	print("Data saved")
end)
```

The SDK requires a valid, well-formed string no larger than 3 MiB. Only one `save_data()` request may be in progress at a time.

## System integration

### Audio

Use `playables.is_audio_enabled()` to initialize the game's audio state, then register a handler to keep it synchronized with YouTube:

```lua
local function apply_audio_setting(enabled)
	sound.set_group_gain(hash("master"), enabled and 1 or 0)
end

apply_audio_setting(playables.is_audio_enabled())

playables.on_audio_enabled_change(function(self, enabled)
	apply_audio_setting(enabled)
end)
```

Registering another callback replaces the previous callback. Pass `nil` to unregister it:

```lua
playables.on_audio_enabled_change(nil)
```

### Pause and resume

YouTube can pause the game when it is backgrounded or exited. A paused game is not guaranteed to resume, so save important state during the pause callback:

```lua
playables.on_pause(function(self)
	-- Pause gameplay and save important state.
end)

playables.on_resume(function(self)
	-- Resume gameplay.
end)
```

Registering another callback replaces the previous callback. Pass `nil` to the corresponding function to unregister it:

```lua
playables.on_pause(nil)
playables.on_resume(nil)
```

### Language

`playables.get_language(callback)` returns the language from the player's YouTube settings as a BCP-47 language tag:

```lua
playables.get_language(function(self, language, error)
	if error then
		print("Unable to get language:", error)
		return
	end

	print("YouTube language:", language)
end)
```

Only one `get_language()` request may be in progress at a time. YouTube recommends using this value instead of another locale source or a language stored in cloud save data.
