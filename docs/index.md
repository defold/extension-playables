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

## Report that the game is ready

Call `playables.game_ready()` after the game has finished loading, the loading screen is no longer visible, and the player can interact with the game:

```lua
playables.game_ready()
```

This calls `ytgame.game.gameReady()` and does not return a value.

::: important
YouTube requires `ytgame.game.firstFrameReady()` to be called before `gameReady()`. The current initial scaffold exposes only `gameReady()`. Do not submit a game for certification until the first-frame integration is also implemented.
:::
