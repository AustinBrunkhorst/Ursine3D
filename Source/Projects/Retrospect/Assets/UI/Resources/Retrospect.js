(function () { "use strict";
var Application = function() { };
Application.main = function() {
	Application.gamepadManager = new GamepadManager();
	Application.keyboardManager = new KeyboardManager();
	var screenManager = new ursine_screen_ScreenManager();
	ursine_native_Extern.InitGame();
};
var haxe_IMap = function() { };
var haxe_ds_IntMap = function() {
	this.h = { };
};
haxe_ds_IntMap.__interfaces__ = [haxe_IMap];
var ursine_native_Extern = function() { };
ursine_native_Extern.InitGame = function() {
	return InitGame();
};
var ursine_utils_IEventContainer = function() { };
var ursine_screen_Screen = function() { };
ursine_screen_Screen.__interfaces__ = [ursine_utils_IEventContainer];
var ursine_screen_ScreenManager = function() {
	ursine_screen_ScreenManager.instance = this;
	this.m_screens = new haxe_ds_IntMap();
	this.m_screensContainer = window.document.querySelector("#screens");
};
Application.main();
})();
