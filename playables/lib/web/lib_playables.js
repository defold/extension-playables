// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

var LibPlayables = {
    $Playables: {
        _loadDataCallback: null,
        _loadDataErrorCallback: null,
        _saveDataCallback: null,
        _saveDataErrorCallback: null,

        _errorMessage: function(error) {
            if (error && typeof error.message === "string") {
                return error.message;
            }
            if (error == null) {
                return "Unknown YouTube Playables SDK error";
            }
            return String(error);
        },

        _invokeErrorCallback: function(callback, error) {
            var message = Playables._errorMessage(error);
            var messagePointer = stringToNewUTF8(message);
            try {
                {{{ makeDynCall("vii", "callback") }}}(messagePointer, lengthBytesUTF8(message));
            } finally {
                Module._free(messagePointer);
            }
        },

        _loadDataSucceeded: function(data) {
            var callback = Playables._loadDataCallback;
            Playables._loadDataCallback = null;
            Playables._loadDataErrorCallback = null;
            if (!callback) {
                return;
            }

            var value = String(data);
            var valuePointer = stringToNewUTF8(value);
            try {
                {{{ makeDynCall("vii", "callback") }}}(valuePointer, lengthBytesUTF8(value));
            } finally {
                Module._free(valuePointer);
            }
        },

        _loadDataFailed: function(error) {
            var callback = Playables._loadDataErrorCallback;
            Playables._loadDataCallback = null;
            Playables._loadDataErrorCallback = null;
            if (callback) {
                Playables._invokeErrorCallback(callback, error);
            }
        },

        _saveDataSucceeded: function() {
            var callback = Playables._saveDataCallback;
            Playables._saveDataCallback = null;
            Playables._saveDataErrorCallback = null;
            if (callback) {
                {{{ makeDynCall("v", "callback") }}}();
            }
        },

        _saveDataFailed: function(error) {
            var callback = Playables._saveDataErrorCallback;
            Playables._saveDataCallback = null;
            Playables._saveDataErrorCallback = null;
            if (callback) {
                Playables._invokeErrorCallback(callback, error);
            }
        }
    },

    PlayablesJs_FirstFrameReady: function() {
        ytgame.game.firstFrameReady();
    },

    PlayablesJs_GameReady: function() {
        ytgame.game.gameReady();
    },

    PlayablesJs_LoadData: function(callback, errorCallback) {
        Playables._loadDataCallback = callback;
        Playables._loadDataErrorCallback = errorCallback;
        try {
            Promise.resolve(ytgame.game.loadData()).then(Playables._loadDataSucceeded, Playables._loadDataFailed);
        } catch (error) {
            Promise.resolve(error).then(Playables._loadDataFailed);
        }
    },

    PlayablesJs_SaveData: function(data, dataLength, callback, errorCallback) {
        Playables._saveDataCallback = callback;
        Playables._saveDataErrorCallback = errorCallback;
        try {
            var value = UTF8ToString(data, dataLength);
            Promise.resolve(ytgame.game.saveData(value)).then(Playables._saveDataSucceeded, Playables._saveDataFailed);
        } catch (error) {
            Promise.resolve(error).then(Playables._saveDataFailed);
        }
    }
};

autoAddDeps(LibPlayables, '$Playables');
addToLibrary(LibPlayables);
