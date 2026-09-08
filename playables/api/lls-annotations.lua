---@meta playables

---Functions for interacting with the YouTube Playables SDK.
playables = {}

---Notifies YouTube that the game is ready for players to interact with.
---Call this only after the loading screen is gone and the game accepts player input.
function playables.game_ready() end

return playables
