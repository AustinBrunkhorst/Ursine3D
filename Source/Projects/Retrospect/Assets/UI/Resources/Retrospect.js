(function () { "use strict";
var $hxClasses = {};
function $extend(from, fields) {
	function Inherit() {} Inherit.prototype = from; var proto = new Inherit();
	for (var name in fields) proto[name] = fields[name];
	if( fields.toString !== Object.prototype.toString ) proto.toString = fields.toString;
	return proto;
}
var Application = function() { };
$hxClasses["Application"] = Application;
Application.__name__ = true;
Application.main = function() {
	Application.broadcastManager = new ursine_utils_NativeBroadcastManager();
	Application.gamepadManager = new GamepadManager();
	Application.keyboardManager = new KeyboardManager();
	Application.screenManager = new ursine_screen_ScreenManager("retrospect.screens.");
	ursine_native_Extern.InitGame();
};
Math.__name__ = true;
var Type = function() { };
$hxClasses["Type"] = Type;
Type.__name__ = true;
Type.resolveClass = function(name) {
	var cl = $hxClasses[name];
	if(cl == null || !cl.__name__) return null;
	return cl;
};
Type.createInstance = function(cl,args) {
	var _g = args.length;
	switch(_g) {
	case 0:
		return new cl();
	case 1:
		return new cl(args[0]);
	case 2:
		return new cl(args[0],args[1]);
	case 3:
		return new cl(args[0],args[1],args[2]);
	case 4:
		return new cl(args[0],args[1],args[2],args[3]);
	case 5:
		return new cl(args[0],args[1],args[2],args[3],args[4]);
	case 6:
		return new cl(args[0],args[1],args[2],args[3],args[4],args[5]);
	case 7:
		return new cl(args[0],args[1],args[2],args[3],args[4],args[5],args[6]);
	case 8:
		return new cl(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7]);
	default:
		throw new js__$Boot_HaxeError("Too many arguments");
	}
	return null;
};
var haxe_IMap = function() { };
$hxClasses["haxe.IMap"] = haxe_IMap;
haxe_IMap.__name__ = true;
var haxe_Timer = function(time_ms) {
	var me = this;
	this.id = setInterval(function() {
		me.run();
	},time_ms);
};
$hxClasses["haxe.Timer"] = haxe_Timer;
haxe_Timer.__name__ = true;
haxe_Timer.delay = function(f,time_ms) {
	var t = new haxe_Timer(time_ms);
	t.run = function() {
		t.stop();
		f();
	};
	return t;
};
haxe_Timer.prototype = {
	stop: function() {
		if(this.id == null) return;
		clearInterval(this.id);
		this.id = null;
	}
	,run: function() {
	}
};
var haxe_ds_IntMap = function() {
	this.h = { };
};
$hxClasses["haxe.ds.IntMap"] = haxe_ds_IntMap;
haxe_ds_IntMap.__name__ = true;
haxe_ds_IntMap.__interfaces__ = [haxe_IMap];
haxe_ds_IntMap.prototype = {
	remove: function(key) {
		if(!this.h.hasOwnProperty(key)) return false;
		delete(this.h[key]);
		return true;
	}
};
var haxe_ds_StringMap = function() {
	this.h = { };
};
$hxClasses["haxe.ds.StringMap"] = haxe_ds_StringMap;
haxe_ds_StringMap.__name__ = true;
haxe_ds_StringMap.__interfaces__ = [haxe_IMap];
haxe_ds_StringMap.prototype = {
	set: function(key,value) {
		if(__map_reserved[key] != null) this.setReserved(key,value); else this.h[key] = value;
	}
	,get: function(key) {
		if(__map_reserved[key] != null) return this.getReserved(key);
		return this.h[key];
	}
	,setReserved: function(key,value) {
		if(this.rh == null) this.rh = { };
		this.rh["$" + key] = value;
	}
	,getReserved: function(key) {
		if(this.rh == null) return null; else return this.rh["$" + key];
	}
};
var js__$Boot_HaxeError = function(val) {
	Error.call(this);
	this.val = val;
	this.message = String(val);
	if(Error.captureStackTrace) Error.captureStackTrace(this,js__$Boot_HaxeError);
};
$hxClasses["js._Boot.HaxeError"] = js__$Boot_HaxeError;
js__$Boot_HaxeError.__name__ = true;
js__$Boot_HaxeError.__super__ = Error;
js__$Boot_HaxeError.prototype = $extend(Error.prototype,{
});
var ursine_utils_IEventContainer = function() { };
$hxClasses["ursine.utils.IEventContainer"] = ursine_utils_IEventContainer;
ursine_utils_IEventContainer.__name__ = true;
var retrospect_screens_BasicMenu = function(container) {
	this.events = new ursine_utils_EventManager();
	this.m_container = container;
	this.m_activeItem = container.querySelector(".active");
};
$hxClasses["retrospect.screens.BasicMenu"] = retrospect_screens_BasicMenu;
retrospect_screens_BasicMenu.__name__ = true;
retrospect_screens_BasicMenu.__interfaces__ = [ursine_utils_IEventContainer];
retrospect_screens_BasicMenu.prototype = {
	up: function() {
		var previous = this.findPrevious();
		if(previous != null) {
			this.setActive(previous);
			this.events.trigger("up",{ item : previous});
		}
	}
	,down: function() {
		var next = this.findNext();
		if(next != null) {
			this.setActive(next);
			this.events.trigger("down",{ item : next});
		}
	}
	,get_container: function() {
		return this.m_container;
	}
	,get_activeItem: function() {
		return this.m_activeItem;
	}
	,setActive: function(item) {
		if(this.m_activeItem == item) return;
		this.m_activeItem.classList.remove("active");
		item.classList.add("active");
		this.m_activeItem = item;
	}
	,findPrevious: function() {
		var previous = this.m_activeItem;
		while((previous = previous.previousElementSibling) != null) if(!previous.classList.contains("no-select")) return previous;
		return null;
	}
	,findNext: function() {
		var next = this.m_activeItem;
		while((next = next.nextElementSibling) != null) if(!next.classList.contains("no-select")) return next;
		return null;
	}
};
var ursine_screen_Screen = function(id,frame,data) {
	this.events = new ursine_utils_EventManager();
	this.m_id = id;
	this.m_frame = frame;
	this.m_document = frame.contentDocument;
	this.m_data = data;
	window.addEventListener("resize",$bind(this,this.handleAspectResize));
	this.handleAspectResize();
};
$hxClasses["ursine.screen.Screen"] = ursine_screen_Screen;
ursine_screen_Screen.__name__ = true;
ursine_screen_Screen.__interfaces__ = [ursine_utils_IEventContainer];
ursine_screen_Screen.prototype = {
	getID: function() {
		return this.m_id;
	}
	,getFrame: function() {
		return this.m_frame;
	}
	,exit: function() {
		Application.screenManager.removeScreen(this);
	}
	,handleAspectResize: function() {
		var aspectContainer = this.m_document.querySelector(".aspect-ratio-container");
		if(aspectContainer == null) return;
		aspectContainer.style.zoom = Math.min(window.innerWidth / ursine_screen_Screen.m_baseScreenWidth,window.innerHeight / ursine_screen_Screen.m_baseScreenHeight);
	}
};
var retrospect_screens_BasicMenuScreen = function(id,frame,data) {
	ursine_screen_Screen.call(this,id,frame,data);
	this.menu = new retrospect_screens_BasicMenu(this.m_document.querySelector(".basic-menu"));
	this.handlers = new haxe_ds_StringMap();
	this.m_exiting = false;
	this.events.on(ursine_input_KeyboardEventType.KeyDown,$bind(this,this.onKeyboardKeyDown)).on(ursine_input_GamepadEventType.ButtonDown,$bind(this,this.onGamepadButtonDown));
	this.menu.events.on("up",$bind(this,this.onMenuUp)).on("down",$bind(this,this.onMenuDown));
};
$hxClasses["retrospect.screens.BasicMenuScreen"] = retrospect_screens_BasicMenuScreen;
retrospect_screens_BasicMenuScreen.__name__ = true;
retrospect_screens_BasicMenuScreen.__super__ = ursine_screen_Screen;
retrospect_screens_BasicMenuScreen.prototype = $extend(ursine_screen_Screen.prototype,{
	transitionExit: function(callback,transition) {
		if(transition == null) transition = "bounceOutUp";
		var _g = this;
		this.m_exiting = true;
		var wrapper = this.menu.get_container().parentNode;
		wrapper.className = "inner-wrapper animated " + transition;
		ElementUtils.once(wrapper,"webkitAnimationEnd",function() {
			if(callback != null) callback();
			_g.exit();
		});
	}
	,apply: function() {
		if(this.menu.get_activeItem() == null) return;
		var handlerName = this.menu.get_activeItem().getAttribute("data-handler");
		var handler = this.handlers.get(handlerName);
		if(handler == null) return;
		ursine_native_Extern.AudioPlayGlobalEvent(retrospect_screens_BasicMenuScreen.m_menuSelectSound);
		handler();
	}
	,onKeyboardKeyDown: function(e) {
		if(this.m_exiting || !(e.triggered && e.pressed)) return;
		if(e.key == 13) this.apply(); else if(e.key == 38 || e.key == 87) this.menu.up(); else if(e.key == 40 || e.key == 83) this.menu.down();
	}
	,onGamepadButtonDown: function(e) {
		if(this.m_exiting || !(e.triggered && e.pressed)) return;
		if(e.button == 0) this.apply(); else if(e.button == 11 || e.button == 18) this.menu.up(); else if(e.button == 12 || e.button == 19) this.menu.down();
	}
	,onMenuUp: function() {
		ursine_native_Extern.AudioPlayGlobalEvent(retrospect_screens_BasicMenuScreen.m_menuUpSound);
	}
	,onMenuDown: function() {
		ursine_native_Extern.AudioPlayGlobalEvent(retrospect_screens_BasicMenuScreen.m_menuDownSound);
	}
});
var retrospect_screens_ConfirmNavigationScreen = function(id,frame,data) {
	var _g = this;
	retrospect_screens_BasicMenuScreen.call(this,id,frame,data);
	var title = this.m_document.querySelector("#navigation-title");
	title.innerHTML = data.title;
	{
		this.handlers.set("yes",function() {
			_g.transitionExit(function() {
				if(data.target.name == "quit") ursine_native_Extern.QuitGame();
				if(data.sender != null) {
					var sender = Application.screenManager.getScreen(data.sender);
					if(sender != null) sender.exit();
				}
				if(data.removeCurrent != null) Application.screenManager.removeCurrent();
				var outputData;
				if(data.target.data == null) outputData = { }; else outputData = data.target.data;
				if(data.target.type == "screen") Application.screenManager.setScreen(data.target.name,outputData); else Application.screenManager.addOverlay(data.target.name,outputData);
			},"bounceOut");
		});
		(function() {
			_g.transitionExit(function() {
				if(data.target.name == "quit") ursine_native_Extern.QuitGame();
				if(data.sender != null) {
					var sender = Application.screenManager.getScreen(data.sender);
					if(sender != null) sender.exit();
				}
				if(data.removeCurrent != null) Application.screenManager.removeCurrent();
				var outputData;
				if(data.target.data == null) outputData = { }; else outputData = data.target.data;
				if(data.target.type == "screen") Application.screenManager.setScreen(data.target.name,outputData); else Application.screenManager.addOverlay(data.target.name,outputData);
			},"bounceOut");
		});
	}
	{
		this.handlers.set("no",function() {
			_g.transitionExit(null,"bounceOut");
		});
		(function() {
			_g.transitionExit(null,"bounceOut");
		});
	}
};
$hxClasses["retrospect.screens.ConfirmNavigationScreen"] = retrospect_screens_ConfirmNavigationScreen;
retrospect_screens_ConfirmNavigationScreen.__name__ = true;
retrospect_screens_ConfirmNavigationScreen.__super__ = retrospect_screens_BasicMenuScreen;
retrospect_screens_ConfirmNavigationScreen.prototype = $extend(retrospect_screens_BasicMenuScreen.prototype,{
});
var retrospect_screens_HowToPlayScreen = function(id,frame,data) {
	ursine_screen_Screen.call(this,id,frame,data);
	this.m_leaving = false;
	this.m_controls = this.m_document.querySelector("#controls");
	this.m_document.addEventListener("mousedown",$bind(this,this.leave));
	this.events.on(ursine_input_KeyboardEventType.KeyDown,$bind(this,this.leave)).on(ursine_input_GamepadEventType.ButtonDown,$bind(this,this.leave));
};
$hxClasses["retrospect.screens.HowToPlayScreen"] = retrospect_screens_HowToPlayScreen;
retrospect_screens_HowToPlayScreen.__name__ = true;
retrospect_screens_HowToPlayScreen.__super__ = ursine_screen_Screen;
retrospect_screens_HowToPlayScreen.prototype = $extend(ursine_screen_Screen.prototype,{
	leave: function() {
		if(this.m_leaving) return;
		this.m_leaving = true;
		this.m_controls.className = "animated zoomOut";
		ElementUtils.once(this.m_controls,"webkitAnimationEnd",$bind(this,this.exit));
	}
});
var retrospect_screens_MainMenuScreen = function(id,frame,data) {
	var _g = this;
	retrospect_screens_BasicMenuScreen.call(this,id,frame,data);
	{
		this.handlers.set("play",function() {
			_g.transitionExit(function() {
				Application.screenManager.setScreen("MultiplayerPlayScreen",{ });
			});
		});
		(function() {
			_g.transitionExit(function() {
				Application.screenManager.setScreen("MultiplayerPlayScreen",{ });
			});
		});
	}
	{
		this.handlers.set("how-to-play",function() {
			Application.screenManager.addOverlay("HowToPlayScreen",{ });
		});
		(function() {
			Application.screenManager.addOverlay("HowToPlayScreen",{ });
		});
	}
	{
		this.handlers.set("credits",function() {
		});
		(function() {
		});
	}
	{
		this.handlers.set("quit",function() {
			Application.screenManager.addOverlay("ConfirmNavigationScreen",{ title : "Quit Game?", target : { name : "quit"}});
		});
		(function() {
			Application.screenManager.addOverlay("ConfirmNavigationScreen",{ title : "Quit Game?", target : { name : "quit"}});
		});
	}
	var musicPlaying = ursine_native_Extern.AudioIsGlobalEventPlaying(retrospect_screens_MainMenuScreen.m_mainMenuMusic);
	if(!musicPlaying) ursine_native_Extern.AudioPlayGlobalEvent(retrospect_screens_MainMenuScreen.m_mainMenuMusic);
};
$hxClasses["retrospect.screens.MainMenuScreen"] = retrospect_screens_MainMenuScreen;
retrospect_screens_MainMenuScreen.__name__ = true;
retrospect_screens_MainMenuScreen.__super__ = retrospect_screens_BasicMenuScreen;
retrospect_screens_MainMenuScreen.prototype = $extend(retrospect_screens_BasicMenuScreen.prototype,{
	exit: function() {
		retrospect_screens_BasicMenuScreen.prototype.exit.call(this);
		var musicPlaying = ursine_native_Extern.AudioIsGlobalEventPlaying(retrospect_screens_MainMenuScreen.m_mainMenuMusic);
		if(musicPlaying) ursine_native_Extern.AudioStopGlobalEvent(retrospect_screens_MainMenuScreen.m_mainMenuMusic);
	}
});
var retrospect_screens_MultiplayerPlayScreen = function(id,frame,data) {
	ursine_screen_Screen.call(this,id,frame,data);
	this.events.on(ursine_input_KeyboardEventType.KeyDown,$bind(this,this.onKeyboardKeyDown)).on(ursine_input_GamepadEventType.ButtonDown,$bind(this,this.onGamepadButtonDown));
};
$hxClasses["retrospect.screens.MultiplayerPlayScreen"] = retrospect_screens_MultiplayerPlayScreen;
retrospect_screens_MultiplayerPlayScreen.__name__ = true;
retrospect_screens_MultiplayerPlayScreen.__super__ = ursine_screen_Screen;
retrospect_screens_MultiplayerPlayScreen.prototype = $extend(ursine_screen_Screen.prototype,{
	triggerPause: function() {
		Application.screenManager.addOverlay("PauseScreen",{ });
	}
	,onKeyboardKeyDown: function(e) {
		if(!(e.triggered && e.pressed)) return;
		if(e.key == 27) this.triggerPause();
	}
	,onGamepadButtonDown: function(e) {
		if(!(e.triggered && e.pressed)) return;
		if(e.button == 4) this.triggerPause();
	}
});
var retrospect_screens_PauseScreen = function(id,frame,data) {
	var _g = this;
	retrospect_screens_BasicMenuScreen.call(this,id,frame,data);
	{
		this.handlers.set("resume",function() {
			_g.transitionExit(null,"bounceOut");
		});
		(function() {
			_g.transitionExit(null,"bounceOut");
		});
	}
	{
		this.handlers.set("restart",function() {
			Application.screenManager.addOverlay("ConfirmNavigationScreen",{ title : "Restart?", sender : _g.getID(), target : { name : "MultiplayerPlayScreen", type : "screen"}, removeCurrent : true});
		});
		(function() {
			Application.screenManager.addOverlay("ConfirmNavigationScreen",{ title : "Restart?", sender : _g.getID(), target : { name : "MultiplayerPlayScreen", type : "screen"}, removeCurrent : true});
		});
	}
	{
		this.handlers.set("main-menu",function() {
			Application.screenManager.addOverlay("ConfirmNavigationScreen",{ title : "Return to Main Menu?", sender : _g.getID(), target : { name : "MainMenuScreen", type : "overlay"}, removeCurrent : true});
		});
		(function() {
			Application.screenManager.addOverlay("ConfirmNavigationScreen",{ title : "Return to Main Menu?", sender : _g.getID(), target : { name : "MainMenuScreen", type : "overlay"}, removeCurrent : true});
		});
	}
	{
		this.handlers.set("how-to-play",function() {
			Application.screenManager.addOverlay("HowToPlayScreen",{ });
		});
		(function() {
			Application.screenManager.addOverlay("HowToPlayScreen",{ });
		});
	}
	{
		this.handlers.set("quit",function() {
			Application.screenManager.addOverlay("ConfirmNavigationScreen",{ title : "Quit Game?", target : { name : "quit"}});
		});
		(function() {
			Application.screenManager.addOverlay("ConfirmNavigationScreen",{ title : "Quit Game?", target : { name : "quit"}});
		});
	}
};
$hxClasses["retrospect.screens.PauseScreen"] = retrospect_screens_PauseScreen;
retrospect_screens_PauseScreen.__name__ = true;
retrospect_screens_PauseScreen.__super__ = retrospect_screens_BasicMenuScreen;
retrospect_screens_PauseScreen.prototype = $extend(retrospect_screens_BasicMenuScreen.prototype,{
});
var retrospect_screens_SplashScreen = function(id,frame,data) {
	var _g = this;
	ursine_screen_Screen.call(this,id,frame,data);
	var logo = this.m_document.querySelector("#logo");
	ElementUtils.once(logo,"webkitAnimationEnd",function() {
		haxe_Timer.delay(function() {
			logo.className = "animated zoomOutUp";
		},1000);
		ElementUtils.once(logo,"webkitAnimationEnd",$bind(_g,_g.exit));
	});
	this.m_document.addEventListener("mousedown",$bind(this,this.exit));
	this.events.on(ursine_input_KeyboardEventType.KeyDown,$bind(this,this.exit)).on(ursine_input_GamepadEventType.ButtonDown,$bind(this,this.exit));
};
$hxClasses["retrospect.screens.SplashScreen"] = retrospect_screens_SplashScreen;
retrospect_screens_SplashScreen.__name__ = true;
retrospect_screens_SplashScreen.__super__ = ursine_screen_Screen;
retrospect_screens_SplashScreen.prototype = $extend(ursine_screen_Screen.prototype,{
	exit: function() {
		ursine_screen_Screen.prototype.exit.call(this);
		Application.screenManager.addOverlay("MainMenuScreen",{ });
	}
});
var ursine_input_GamepadEventType = function() { };
$hxClasses["ursine.input.GamepadEventType"] = ursine_input_GamepadEventType;
ursine_input_GamepadEventType.__name__ = true;
var ursine_input_KeyboardEventType = function() { };
$hxClasses["ursine.input.KeyboardEventType"] = ursine_input_KeyboardEventType;
ursine_input_KeyboardEventType.__name__ = true;
var ursine_native_Extern = function() { };
$hxClasses["ursine.native.Extern"] = ursine_native_Extern;
ursine_native_Extern.__name__ = true;
ursine_native_Extern.InitGame = function() {
	return InitGame();
};
ursine_native_Extern.QuitGame = function() {
	return QuitGame();
};
ursine_native_Extern.AudioPlayGlobalEvent = function(event) {
	return AudioPlayGlobalEvent( event );
};
ursine_native_Extern.AudioStopGlobalEvent = function(event) {
	return AudioStopGlobalEvent( event );
};
ursine_native_Extern.AudioIsGlobalEventPlaying = function(event) {
	return AudioIsGlobalEventPlaying( event );
};
var ursine_screen_ScreenManager = function(screenPackagePrefix) {
	ursine_screen_ScreenManager.instance = this;
	this.m_screenPackagePrefix = screenPackagePrefix;
	this.m_nativeManager = new ScreenManager();
	this.m_screens = new haxe_ds_IntMap();
	this.m_screensContainer = window.document.querySelector("#screens");
	Application.broadcastManager.getChannel("ScreenManager").on("Event",$bind(this,this.onScreenEvent)).on("Entered",$bind(this,this.onScreenEntered)).on("Exited",$bind(this,this.onScreenExited));
	Application.broadcastManager.getChannel("GamepadManager").on(ursine_input_GamepadEventType.ButtonDown,$bind(this,this.onGamepadBtnDown)).on(ursine_input_GamepadEventType.ButtonUp,$bind(this,this.onGamepadBtnUp)).on(ursine_input_GamepadEventType.Connected,$bind(this,this.onGamepadConnected)).on(ursine_input_GamepadEventType.Disconnected,$bind(this,this.onGamepadDisconnected));
	Application.broadcastManager.getChannel("KeyboardManager").on(ursine_input_KeyboardEventType.KeyDown,$bind(this,this.onKeyDown)).on(ursine_input_KeyboardEventType.KeyUp,$bind(this,this.onKeyUp));
};
$hxClasses["ursine.screen.ScreenManager"] = ursine_screen_ScreenManager;
ursine_screen_ScreenManager.__name__ = true;
ursine_screen_ScreenManager.prototype = {
	getScreen: function(id) {
		return this.m_screens.h[id];
	}
	,removeScreen: function(screen) {
		var id = screen.getID();
		this.m_nativeManager.removeScreen(id);
		this.m_screensContainer.removeChild(screen.getFrame());
		this.m_screens.remove(id);
	}
	,removeCurrent: function() {
		this.m_nativeManager.removeCurrent();
	}
	,setScreen: function(name,data) {
		this.m_nativeManager.setScreen(name,data);
	}
	,addOverlay: function(name,data) {
		this.m_nativeManager.addOverlay(name,data);
	}
	,createScreen: function(name,id,data) {
		var _g = this;
		var frame;
		var _this = window.document;
		frame = _this.createElement("iframe");
		frame.src = "Screens/" + name + ".html";
		frame.style.display = "none";
		this.m_screensContainer.appendChild(frame);
		frame.contentWindow.addEventListener("load",function() {
			frame.style.display = "block";
			var screenType = Type.resolveClass(_g.m_screenPackagePrefix + name);
			if(screenType == null) throw new js__$Boot_HaxeError("Unknown screen type \"" + name + "\".");
			var value = Type.createInstance(screenType,[id,frame,data]);
			_g.m_screens.h[id] = value;
		});
	}
	,onScreenEvent: function(e) {
		var screen;
		var key = e.screenID;
		screen = this.m_screens.h[key];
		if(screen != null) screen.events.trigger(e.event,e.data);
	}
	,onScreenEntered: function(e) {
		this.createScreen(e.screenName,e.screenID,e.data);
	}
	,onScreenExited: function(e) {
		var screen;
		var key = e.screenID;
		screen = this.m_screens.h[key];
		if(screen != null) screen.exit();
	}
	,onGamepadBtnDown: function(e) {
		var screen;
		var key = this.m_nativeManager.getFocusedScreen();
		screen = this.m_screens.h[key];
		if(screen != null) screen.events.trigger(ursine_input_GamepadEventType.ButtonDown,e);
	}
	,onGamepadBtnUp: function(e) {
		var screen;
		var key = this.m_nativeManager.getFocusedScreen();
		screen = this.m_screens.h[key];
		if(screen != null) screen.events.trigger(ursine_input_GamepadEventType.ButtonUp,e);
	}
	,onGamepadConnected: function(e) {
		var screen;
		var key = this.m_nativeManager.getFocusedScreen();
		screen = this.m_screens.h[key];
		if(screen != null) screen.events.trigger(ursine_input_GamepadEventType.Connected,e);
	}
	,onGamepadDisconnected: function(e) {
		var screen;
		var key = this.m_nativeManager.getFocusedScreen();
		screen = this.m_screens.h[key];
		if(screen != null) screen.events.trigger(ursine_input_GamepadEventType.Disconnected,e);
	}
	,onKeyDown: function(e) {
		var screen;
		var key = this.m_nativeManager.getFocusedScreen();
		screen = this.m_screens.h[key];
		if(screen != null) screen.events.trigger(ursine_input_KeyboardEventType.KeyDown,e);
	}
	,onKeyUp: function(e) {
		var screen;
		var key = this.m_nativeManager.getFocusedScreen();
		screen = this.m_screens.h[key];
		if(screen != null) screen.events.trigger(ursine_input_KeyboardEventType.KeyUp,e);
	}
};
var ursine_utils_EventManager = function() {
	this.m_events = new haxe_ds_StringMap();
};
$hxClasses["ursine.utils.EventManager"] = ursine_utils_EventManager;
ursine_utils_EventManager.__name__ = true;
ursine_utils_EventManager.prototype = {
	on: function(event,handler) {
		var handlers = this.m_events.get(event);
		if(handlers == null) {
			handlers = [];
			this.m_events.set(event,handlers);
		}
		handlers.push(handler);
		return this;
	}
	,trigger: function(event,data) {
		var handlers = this.m_events.get(event);
		if(handlers == null) return true;
		var result = true;
		var _g = 0;
		while(_g < handlers.length) {
			var handler = handlers[_g];
			++_g;
			if(!handler(data)) result = false;
		}
		return result;
	}
};
var ursine_utils_NativeBroadcastManager = function() {
	this.m_channels = new haxe_ds_StringMap();
	ursine_utils_NativeBroadcastManager.m_instance = this;
	window.NativeBroadcast = ursine_utils_NativeBroadcastManager.onBroadcast;
};
$hxClasses["ursine.utils.NativeBroadcastManager"] = ursine_utils_NativeBroadcastManager;
ursine_utils_NativeBroadcastManager.__name__ = true;
ursine_utils_NativeBroadcastManager.onBroadcast = function(target,message,data) {
	ursine_utils_NativeBroadcastManager.m_instance.getChannel(target).trigger(message,data);
};
ursine_utils_NativeBroadcastManager.prototype = {
	getChannel: function(name) {
		var channel = this.m_channels.get(name);
		if(channel == null) {
			channel = new ursine_utils_EventManager();
			this.m_channels.set(name,channel);
		}
		return channel;
	}
};
var $_, $fid = 0;
function $bind(o,m) { if( m == null ) return null; if( m.__id__ == null ) m.__id__ = $fid++; var f; if( o.hx__closures__ == null ) o.hx__closures__ = {}; else f = o.hx__closures__[m.__id__]; if( f == null ) { f = function(){ return f.method.apply(f.scope, arguments); }; f.scope = o; f.method = m; o.hx__closures__[m.__id__] = f; } return f; }
$hxClasses.Math = Math;
String.__name__ = true;
$hxClasses.Array = Array;
Array.__name__ = true;
var __map_reserved = {}
ursine_screen_Screen.m_baseScreenWidth = 1280;
ursine_screen_Screen.m_baseScreenHeight = 720;
retrospect_screens_BasicMenuScreen.m_menuUpSound = "MENU_CYCLE_UP";
retrospect_screens_BasicMenuScreen.m_menuDownSound = "MENU_CYCLE_DOWN";
retrospect_screens_BasicMenuScreen.m_menuSelectSound = "MENU_SELECT";
retrospect_screens_MainMenuScreen.m_mainMenuMusic = "MUSIC_THEME";
ursine_input_GamepadEventType.ButtonDown = "GamepadButtonDown";
ursine_input_GamepadEventType.ButtonUp = "GamepadButtonUp";
ursine_input_GamepadEventType.Connected = "GamepadConnected";
ursine_input_GamepadEventType.Disconnected = "GamepadDisconnected";
ursine_input_KeyboardEventType.KeyDown = "KeyboardKeyDown";
ursine_input_KeyboardEventType.KeyUp = "KeyboardKeyUp";
Application.main();
})();
