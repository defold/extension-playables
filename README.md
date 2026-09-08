[![Build with bob](https://github.com/defold/extension-playables/actions/workflows/bob.yml/badge.svg)](https://github.com/defold/extension-playables/actions/workflows/bob.yml)

# YouTube Playables SDK Extension for Defold

This [Defold native extension](https://defold.com/manuals/extensions/) exposes the [YouTube Playables SDK](https://developers.google.com/youtube/gaming/playables/reference/sdk) to Lua.

The initial API exposes `ytgame.game.gameReady()` as:

```lua
playables.game_ready()
```

Add the extension to a Defold project using a release archive URL:

```text
https://github.com/defold/extension-playables/archive/refs/tags/<version>.zip
```

The extension's HTML5 engine template loads the YouTube Playables SDK before the game code. Call `playables.game_ready()` only when the loading screen has been removed and the game can accept player input.

> **Important:** YouTube requires `ytgame.game.firstFrameReady()` to be called before `gameReady()`. The current initial scaffold only exposes `gameReady()`; a complete, certifiable integration must also report the first frame.

See the [extension documentation](docs/index.md) for more information.
