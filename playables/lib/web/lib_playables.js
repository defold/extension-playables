// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

var LibPlayables = {
    $Playables: {
        _loadDataCallback: null,
        _loadDataErrorCallback: null,
        _saveDataCallback: null,
        _saveDataErrorCallback: null,
        _getLanguageCallback: null,
        _getLanguageErrorCallback: null,
        _audioEnabledChangeCallback: null,
        _audioEnabledChangeUnsubscribe: null,
        _pauseCallback: null,
        _pauseUnsubscribe: null,
        _resumeCallback: null,
        _resumeUnsubscribe: null,
        _sendScoreCallback: null,
        _sendScoreErrorCallback: null,
        _openYTContentCallback: null,
        _openYTContentErrorCallback: null,
        _interstitialAdCallback: null,
        _interstitialAdErrorCallback: null,
        _rewardedAdCallback: null,
        _rewardedAdErrorCallback: null,

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
        },

        _getLanguageSucceeded: function(language) {
            var callback = Playables._getLanguageCallback;
            Playables._getLanguageCallback = null;
            Playables._getLanguageErrorCallback = null;
            if (!callback) {
                return;
            }

            var value = String(language);
            var valuePointer = stringToNewUTF8(value);
            try {
                {{{ makeDynCall("vii", "callback") }}}(valuePointer, lengthBytesUTF8(value));
            } finally {
                Module._free(valuePointer);
            }
        },

        _getLanguageFailed: function(error) {
            var callback = Playables._getLanguageErrorCallback;
            Playables._getLanguageCallback = null;
            Playables._getLanguageErrorCallback = null;
            if (callback) {
                Playables._invokeErrorCallback(callback, error);
            }
        },

        _clearAudioEnabledChange: function() {
            var unsubscribe = Playables._audioEnabledChangeUnsubscribe;
            Playables._audioEnabledChangeCallback = null;
            Playables._audioEnabledChangeUnsubscribe = null;
            if (typeof unsubscribe === "function") {
                unsubscribe();
            }
        },

        _clearPause: function() {
            var unsubscribe = Playables._pauseUnsubscribe;
            Playables._pauseCallback = null;
            Playables._pauseUnsubscribe = null;
            if (typeof unsubscribe === "function") {
                unsubscribe();
            }
        },

        _clearResume: function() {
            var unsubscribe = Playables._resumeUnsubscribe;
            Playables._resumeCallback = null;
            Playables._resumeUnsubscribe = null;
            if (typeof unsubscribe === "function") {
                unsubscribe();
            }
        },

        _audioEnabledChanged: function(isAudioEnabled) {
            var callback = Playables._audioEnabledChangeCallback;
            if (callback && !{{{ makeDynCall("ii", "callback") }}}(isAudioEnabled ? 1 : 0)) {
                Playables._clearAudioEnabledChange();
            }
        },

        _paused: function() {
            var callback = Playables._pauseCallback;
            if (callback && !{{{ makeDynCall("i", "callback") }}}()) {
                Playables._clearPause();
            }
        },

        _resumed: function() {
            var callback = Playables._resumeCallback;
            if (callback && !{{{ makeDynCall("i", "callback") }}}()) {
                Playables._clearResume();
            }
        },

        _sendScoreSucceeded: function() {
            var callback = Playables._sendScoreCallback;
            Playables._sendScoreCallback = null;
            Playables._sendScoreErrorCallback = null;
            if (callback) {
                {{{ makeDynCall("v", "callback") }}}();
            }
        },

        _sendScoreFailed: function(error) {
            var callback = Playables._sendScoreErrorCallback;
            Playables._sendScoreCallback = null;
            Playables._sendScoreErrorCallback = null;
            if (callback) {
                Playables._invokeErrorCallback(callback, error);
            }
        },

        _openYTContentSucceeded: function() {
            var callback = Playables._openYTContentCallback;
            Playables._openYTContentCallback = null;
            Playables._openYTContentErrorCallback = null;
            if (callback) {
                {{{ makeDynCall("v", "callback") }}}();
            }
        },

        _openYTContentFailed: function(error) {
            var callback = Playables._openYTContentErrorCallback;
            Playables._openYTContentCallback = null;
            Playables._openYTContentErrorCallback = null;
            if (callback) {
                Playables._invokeErrorCallback(callback, error);
            }
        },

        _interstitialAdSucceeded: function() {
            var callback = Playables._interstitialAdCallback;
            Playables._interstitialAdCallback = null;
            Playables._interstitialAdErrorCallback = null;
            if (callback) {
                {{{ makeDynCall("v", "callback") }}}();
            }
        },

        _interstitialAdFailed: function(error) {
            var callback = Playables._interstitialAdErrorCallback;
            Playables._interstitialAdCallback = null;
            Playables._interstitialAdErrorCallback = null;
            if (callback) {
                Playables._invokeErrorCallback(callback, error);
            }
        },

        _rewardedAdSucceeded: function(rewardEarned) {
            var callback = Playables._rewardedAdCallback;
            Playables._rewardedAdCallback = null;
            Playables._rewardedAdErrorCallback = null;
            if (callback) {
                {{{ makeDynCall("vi", "callback") }}}(rewardEarned ? 1 : 0);
            }
        },

        _rewardedAdFailed: function(error) {
            var callback = Playables._rewardedAdErrorCallback;
            Playables._rewardedAdCallback = null;
            Playables._rewardedAdErrorCallback = null;
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
            // Decode the full byte range, including embedded NULs and a leading BOM.
            // Copy the bytes so decoding also works with shared WebAssembly memory.
            var bytes = HEAPU8.slice(data, data + dataLength);
            var value = new TextDecoder("utf-8", { ignoreBOM: true }).decode(bytes);
            Promise.resolve(ytgame.game.saveData(value)).then(Playables._saveDataSucceeded, Playables._saveDataFailed);
        } catch (error) {
            Promise.resolve(error).then(Playables._saveDataFailed);
        }
    },

    PlayablesJs_IsAudioEnabled: function() {
        return ytgame.system.isAudioEnabled();
    },

    PlayablesJs_OnAudioEnabledChange: function(callback) {
        Playables._clearAudioEnabledChange();
        if (callback) {
            Playables._audioEnabledChangeCallback = callback;
            Playables._audioEnabledChangeUnsubscribe = ytgame.system.onAudioEnabledChange(Playables._audioEnabledChanged);
        }
    },

    PlayablesJs_OnPause: function(callback) {
        Playables._clearPause();
        if (callback) {
            Playables._pauseCallback = callback;
            Playables._pauseUnsubscribe = ytgame.system.onPause(Playables._paused);
        }
    },

    PlayablesJs_OnResume: function(callback) {
        Playables._clearResume();
        if (callback) {
            Playables._resumeCallback = callback;
            Playables._resumeUnsubscribe = ytgame.system.onResume(Playables._resumed);
        }
    },

    PlayablesJs_GetLanguage: function(callback, errorCallback) {
        Playables._getLanguageCallback = callback;
        Playables._getLanguageErrorCallback = errorCallback;
        try {
            Promise.resolve(ytgame.system.getLanguage()).then(Playables._getLanguageSucceeded, Playables._getLanguageFailed);
        } catch (error) {
            Promise.resolve(error).then(Playables._getLanguageFailed);
        }
    },

    PlayablesJs_ClearCallbacks: function() {
        Playables._loadDataCallback = null;
        Playables._loadDataErrorCallback = null;
        Playables._saveDataCallback = null;
        Playables._saveDataErrorCallback = null;
        Playables._getLanguageCallback = null;
        Playables._getLanguageErrorCallback = null;
        Playables._sendScoreCallback = null;
        Playables._sendScoreErrorCallback = null;
        Playables._openYTContentCallback = null;
        Playables._openYTContentErrorCallback = null;
        Playables._interstitialAdCallback = null;
        Playables._interstitialAdErrorCallback = null;
        Playables._rewardedAdCallback = null;
        Playables._rewardedAdErrorCallback = null;
        Playables._clearAudioEnabledChange();
        Playables._clearPause();
        Playables._clearResume();
    },

    PlayablesJs_SendScore: function(score, callback, errorCallback) {
        Playables._sendScoreCallback = callback;
        Playables._sendScoreErrorCallback = errorCallback;
        try {
            Promise.resolve(ytgame.engagement.sendScore({ value: score })).then(Playables._sendScoreSucceeded, Playables._sendScoreFailed);
        } catch (error) {
            Promise.resolve(error).then(Playables._sendScoreFailed);
        }
    },

    PlayablesJs_OpenYTContent: function(contentId, contentIdLength, contentType, callback, errorCallback) {
        Playables._openYTContentCallback = callback;
        Playables._openYTContentErrorCallback = errorCallback;
        try {
            var id = UTF8ToString(contentId, contentIdLength);
            var content = { id: id, contentType: contentType };
            Promise.resolve(ytgame.engagement.openYTContent(content)).then(Playables._openYTContentSucceeded, Playables._openYTContentFailed);
        } catch (error) {
            Promise.resolve(error).then(Playables._openYTContentFailed);
        }
    },

    PlayablesJs_RequestInterstitialAd: function(callback, errorCallback) {
        Playables._interstitialAdCallback = callback;
        Playables._interstitialAdErrorCallback = errorCallback;
        try {
            Promise.resolve(ytgame.ads.requestInterstitialAd()).then(Playables._interstitialAdSucceeded, Playables._interstitialAdFailed);
        } catch (error) {
            Promise.resolve(error).then(Playables._interstitialAdFailed);
        }
    },

    PlayablesJs_RequestRewardedAd: function(rewardId, rewardIdLength, callback, errorCallback) {
        Playables._rewardedAdCallback = callback;
        Playables._rewardedAdErrorCallback = errorCallback;
        try {
            var bytes = HEAPU8.slice(rewardId, rewardId + rewardIdLength);
            var id = new TextDecoder("utf-8", { ignoreBOM: true }).decode(bytes);
            Promise.resolve(ytgame.ads.requestRewardedAd(id)).then(Playables._rewardedAdSucceeded, Playables._rewardedAdFailed);
        } catch (error) {
            Promise.resolve(error).then(Playables._rewardedAdFailed);
        }
    }
};

autoAddDeps(LibPlayables, '$Playables');
addToLibrary(LibPlayables);
