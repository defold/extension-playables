[![Build with bob](https://github.com/defold/extension-playables/actions/workflows/bob.yml/badge.svg)](https://github.com/defold/extension-playables/actions/workflows/bob.yml)

# YouTube Playables SDK Extension for Defold

This [Defold native extension](https://defold.com/manuals/extensions/) exposes the [YouTube Playables SDK](https://developers.google.com/youtube/gaming/playables/reference/sdk) to Lua.

The Lua API covers the public functions in `ytgame.game`:

```lua
playables.first_frame_ready()
playables.game_ready()
playables.load_data(function(self, data, error) end)
playables.save_data(data, function(self, success, error) end)
```

Add the extension to a Defold project using a release archive URL:

```text
https://github.com/defold/extension-playables/archive/refs/tags/<version>.zip
```

The extension's HTML5 engine template loads the YouTube Playables SDK before the game code. YouTube requires `first_frame_ready()` before `game_ready()`, and `game_ready()` must only be called after the loading screen is removed and the game accepts player input.

See the [extension documentation](docs/index.md) for more information.
