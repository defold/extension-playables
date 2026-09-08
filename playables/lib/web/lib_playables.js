// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

var LibPlayables = {
    PlayablesJs_GameReady: function() {
        ytgame.game.gameReady();
    }
};

addToLibrary(LibPlayables);
