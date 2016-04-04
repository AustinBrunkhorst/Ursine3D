(function ($global) { "use strict";
function $extend(from, fields) {
	function Inherit() {} Inherit.prototype = from; var proto = new Inherit();
	for (var name in fields) proto[name] = fields[name];
	if( fields.toString !== Object.prototype.toString ) proto.toString = fields.toString;
	return proto;
}
var EReg = function(r,opt) {
	opt = opt.split("u").join("");
	this.r = new RegExp(r,opt);
};
EReg.__name__ = true;
EReg.prototype = {
	replace: function(s,by) {
		return s.replace(this.r,by);
	}
	,__class__: EReg
};
var Game = function() { };
Game.__name__ = true;
Game.main = function() {
	var broadcastManager = new ursine_api_native_NativeBroadcastManager();
	var screenManager = new ui_ScreenManager(window.document.querySelector("#screen-manager"));
};
var HxOverrides = function() { };
HxOverrides.__name__ = true;
HxOverrides.cca = function(s,index) {
	var x = s.charCodeAt(index);
	if(x != x) return undefined;
	return x;
};
HxOverrides.iter = function(a) {
	return { cur : 0, arr : a, hasNext : function() {
		return this.cur < this.arr.length;
	}, next : function() {
		return this.arr[this.cur++];
	}};
};
Math.__name__ = true;
var Reflect = function() { };
Reflect.__name__ = true;
Reflect.field = function(o,field) {
	try {
		return o[field];
	} catch( e ) {
		if (e instanceof js__$Boot_HaxeError) e = e.val;
		return null;
	}
};
Reflect.fields = function(o) {
	var a = [];
	if(o != null) {
		var hasOwnProperty = Object.prototype.hasOwnProperty;
		for( var f in o ) {
		if(f != "__id__" && f != "hx__closures__" && hasOwnProperty.call(o,f)) a.push(f);
		}
	}
	return a;
};
var Std = function() { };
Std.__name__ = true;
Std.string = function(s) {
	return js_Boot.__string_rec(s,"");
};
var StringBuf = function() {
	this.b = "";
};
StringBuf.__name__ = true;
StringBuf.prototype = {
	add: function(x) {
		this.b += Std.string(x);
	}
	,__class__: StringBuf
};
var Type = function() { };
Type.__name__ = true;
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
haxe_IMap.__name__ = true;
var haxe_ds_IntMap = function() {
	this.h = { };
};
haxe_ds_IntMap.__name__ = true;
haxe_ds_IntMap.__interfaces__ = [haxe_IMap];
haxe_ds_IntMap.prototype = {
	remove: function(key) {
		if(!this.h.hasOwnProperty(key)) return false;
		delete(this.h[key]);
		return true;
	}
	,keys: function() {
		var a = [];
		for( var key in this.h ) {
		if(this.h.hasOwnProperty(key)) a.push(key | 0);
		}
		return HxOverrides.iter(a);
	}
	,iterator: function() {
		return { ref : this.h, it : this.keys(), hasNext : function() {
			return this.it.hasNext();
		}, next : function() {
			var i = this.it.next();
			return this.ref[i];
		}};
	}
	,__class__: haxe_ds_IntMap
};
var haxe_ds_StringMap = function() {
	this.h = { };
};
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
	,exists: function(key) {
		if(__map_reserved[key] != null) return this.existsReserved(key);
		return this.h.hasOwnProperty(key);
	}
	,setReserved: function(key,value) {
		if(this.rh == null) this.rh = { };
		this.rh["$" + key] = value;
	}
	,getReserved: function(key) {
		if(this.rh == null) return null; else return this.rh["$" + key];
	}
	,existsReserved: function(key) {
		if(this.rh == null) return false;
		return this.rh.hasOwnProperty("$" + key);
	}
	,remove: function(key) {
		if(__map_reserved[key] != null) {
			key = "$" + key;
			if(this.rh == null || !this.rh.hasOwnProperty(key)) return false;
			delete(this.rh[key]);
			return true;
		} else {
			if(!this.h.hasOwnProperty(key)) return false;
			delete(this.h[key]);
			return true;
		}
	}
	,__class__: haxe_ds_StringMap
};
var haxe_io_Path = function() { };
haxe_io_Path.__name__ = true;
haxe_io_Path.normalize = function(path) {
	var slash = "/";
	path = path.split("\\").join("/");
	if(path == null || path == slash) return slash;
	var target = [];
	var _g = 0;
	var _g1 = path.split(slash);
	while(_g < _g1.length) {
		var token = _g1[_g];
		++_g;
		if(token == ".." && target.length > 0 && target[target.length - 1] != "..") target.pop(); else if(token != ".") target.push(token);
	}
	var tmp = target.join(slash);
	var regex = new EReg("([^:])/+","g");
	var result = regex.replace(tmp,"$1" + slash);
	var acc = new StringBuf();
	var colon = false;
	var slashes = false;
	var _g11 = 0;
	var _g2 = tmp.length;
	while(_g11 < _g2) {
		var i = _g11++;
		var _g21 = HxOverrides.cca(tmp,i);
		var i1 = _g21;
		if(_g21 != null) switch(_g21) {
		case 58:
			acc.b += ":";
			colon = true;
			break;
		case 47:
			if(colon == false) slashes = true; else {
				colon = false;
				if(slashes) {
					acc.b += "/";
					slashes = false;
				}
				acc.add(String.fromCharCode(i1));
			}
			break;
		default:
			colon = false;
			if(slashes) {
				acc.b += "/";
				slashes = false;
			}
			acc.add(String.fromCharCode(i1));
		} else {
			colon = false;
			if(slashes) {
				acc.b += "/";
				slashes = false;
			}
			acc.add(String.fromCharCode(i1));
		}
	}
	var result1 = acc.b;
	return result1;
};
var js__$Boot_HaxeError = function(val) {
	Error.call(this);
	this.val = val;
	this.message = String(val);
	if(Error.captureStackTrace) Error.captureStackTrace(this,js__$Boot_HaxeError);
};
js__$Boot_HaxeError.__name__ = true;
js__$Boot_HaxeError.__super__ = Error;
js__$Boot_HaxeError.prototype = $extend(Error.prototype,{
	__class__: js__$Boot_HaxeError
});
var js_Boot = function() { };
js_Boot.__name__ = true;
js_Boot.getClass = function(o) {
	if((o instanceof Array) && o.__enum__ == null) return Array; else {
		var cl = o.__class__;
		if(cl != null) return cl;
		var name = js_Boot.__nativeClassName(o);
		if(name != null) return js_Boot.__resolveNativeClass(name);
		return null;
	}
};
js_Boot.__string_rec = function(o,s) {
	if(o == null) return "null";
	if(s.length >= 5) return "<...>";
	var t = typeof(o);
	if(t == "function" && (o.__name__ || o.__ename__)) t = "object";
	switch(t) {
	case "object":
		if(o instanceof Array) {
			if(o.__enum__) {
				if(o.length == 2) return o[0];
				var str2 = o[0] + "(";
				s += "\t";
				var _g1 = 2;
				var _g = o.length;
				while(_g1 < _g) {
					var i1 = _g1++;
					if(i1 != 2) str2 += "," + js_Boot.__string_rec(o[i1],s); else str2 += js_Boot.__string_rec(o[i1],s);
				}
				return str2 + ")";
			}
			var l = o.length;
			var i;
			var str1 = "[";
			s += "\t";
			var _g2 = 0;
			while(_g2 < l) {
				var i2 = _g2++;
				str1 += (i2 > 0?",":"") + js_Boot.__string_rec(o[i2],s);
			}
			str1 += "]";
			return str1;
		}
		var tostr;
		try {
			tostr = o.toString;
		} catch( e ) {
			if (e instanceof js__$Boot_HaxeError) e = e.val;
			return "???";
		}
		if(tostr != null && tostr != Object.toString && typeof(tostr) == "function") {
			var s2 = o.toString();
			if(s2 != "[object Object]") return s2;
		}
		var k = null;
		var str = "{\n";
		s += "\t";
		var hasp = o.hasOwnProperty != null;
		for( var k in o ) {
		if(hasp && !o.hasOwnProperty(k)) {
			continue;
		}
		if(k == "prototype" || k == "__class__" || k == "__super__" || k == "__interfaces__" || k == "__properties__") {
			continue;
		}
		if(str.length != 2) str += ", \n";
		str += s + k + " : " + js_Boot.__string_rec(o[k],s);
		}
		s = s.substring(1);
		str += "\n" + s + "}";
		return str;
	case "function":
		return "<function>";
	case "string":
		return o;
	default:
		return String(o);
	}
};
js_Boot.__interfLoop = function(cc,cl) {
	if(cc == null) return false;
	if(cc == cl) return true;
	var intf = cc.__interfaces__;
	if(intf != null) {
		var _g1 = 0;
		var _g = intf.length;
		while(_g1 < _g) {
			var i = _g1++;
			var i1 = intf[i];
			if(i1 == cl || js_Boot.__interfLoop(i1,cl)) return true;
		}
	}
	return js_Boot.__interfLoop(cc.__super__,cl);
};
js_Boot.__instanceof = function(o,cl) {
	if(cl == null) return false;
	switch(cl) {
	case Int:
		return (o|0) === o;
	case Float:
		return typeof(o) == "number";
	case Bool:
		return typeof(o) == "boolean";
	case String:
		return typeof(o) == "string";
	case Array:
		return (o instanceof Array) && o.__enum__ == null;
	case Dynamic:
		return true;
	default:
		if(o != null) {
			if(typeof(cl) == "function") {
				if(o instanceof cl) return true;
				if(js_Boot.__interfLoop(js_Boot.getClass(o),cl)) return true;
			} else if(typeof(cl) == "object" && js_Boot.__isNativeObj(cl)) {
				if(o instanceof cl) return true;
			}
		} else return false;
		if(cl == Class && o.__name__ != null) return true;
		if(cl == Enum && o.__ename__ != null) return true;
		return o.__enum__ == cl;
	}
};
js_Boot.__nativeClassName = function(o) {
	var name = js_Boot.__toStr.call(o).slice(8,-1);
	if(name == "Object" || name == "Function" || name == "Math" || name == "JSON") return null;
	return name;
};
js_Boot.__isNativeObj = function(o) {
	return js_Boot.__nativeClassName(o) != null;
};
js_Boot.__resolveNativeClass = function(name) {
	return $global[name];
};
var ui_ScreenLayoutCache = function() {
};
ui_ScreenLayoutCache.__name__ = true;
ui_ScreenLayoutCache.prototype = {
	__class__: ui_ScreenLayoutCache
};
var ursine_api_ui_ScreenManager = function() { };
ursine_api_ui_ScreenManager.__name__ = true;
ursine_api_ui_ScreenManager.prototype = {
	__class__: ursine_api_ui_ScreenManager
};
var ui_ScreenManager = function(container) {
	this.m_nativeManager = new NativeScreenManager();
	this.m_screens = new haxe_ds_IntMap();
	var _this = window.document;
	this.m_container = _this.createElement("div");
	this.m_container.classList.add("screen-manager");
	container.appendChild(this.m_container);
	this.m_projectCacheState = new haxe_ds_StringMap();
	this.m_projectScriptCache = new haxe_ds_StringMap();
	this.m_screenLoadQueue = new haxe_ds_StringMap();
	this.m_screenLayoutCache = new haxe_ds_StringMap();
	this.m_screenTypeCache = new haxe_ds_StringMap();
	var kbManager = new NativeKeyboardManager();
	var gpManager = new NativeGamepadManager();
	var bm = ursine_api_native_NativeBroadcastManager.instance;
	bm.getChannel("ScreenManager").on("ScreenAdded",$bind(this,this.onScreenAdded)).on("ScreenMessaged",$bind(this,this.onScreenMessaged)).on("ScreenExited",$bind(this,this.onScreenExited)).on("ScreensCleared",$bind(this,this.onScreensCleared));
	bm.getChannel("GamepadManager").on("GamepadButtonDown",$bind(this,this.onGamepadBtnDown)).on("GamepadButtonUp",$bind(this,this.onGamepadBtnUp)).on("GamepadConnected",$bind(this,this.onGamepadConnected)).on("GamepadDisconnected",$bind(this,this.onGamepadDisconnected));
	bm.getChannel("KeyboardManager").on("KeyboardKeyDown",$bind(this,this.onKeyDown)).on("KeyboardKeyUp",$bind(this,this.onKeyUp));
};
ui_ScreenManager.__name__ = true;
ui_ScreenManager.__interfaces__ = [ursine_api_ui_ScreenManager];
ui_ScreenManager.prototype = {
	getScreen: function(id) {
		return this.m_screens.h[id];
	}
	,removeScreen: function(screen) {
		var id = screen.getID();
		this.m_nativeManager.removeScreen(id);
		this.m_container.removeChild(screen.getHost());
		this.m_screens.remove(id);
	}
	,addScreen: function(path,initData,inputBlocking,priority) {
		if(priority == null) priority = 0;
		if(inputBlocking == null) inputBlocking = true;
		var id = this.m_nativeManager.createScreen(path,inputBlocking,priority);
		this.createScreen(path,id,priority,inputBlocking);
		return id;
	}
	,hasInputFocus: function(screen) {
		return this.m_nativeManager.screenHasFocus(screen.getID());
	}
	,clearScreens: function() {
		this.m_container.innerHTML = "";
		this.m_screens = new haxe_ds_IntMap();
	}
	,invalidateScreenTypeCache: function() {
		var screens = window.screens;
		var typeNames = Reflect.fields(screens);
		var _g = 0;
		while(_g < typeNames.length) {
			var typeName = typeNames[_g];
			++_g;
			var value = Reflect.field(screens,typeName);
			this.m_screenTypeCache.set(typeName,value);
		}
	}
	,initProjectConfig: function(guid) {
		var _g = this;
		var v = 0;
		this.m_projectCacheState.set(guid,v);
		v;
		var head = window.document.head;
		var script;
		var _this = window.document;
		script = _this.createElement("script");
		script.src = this.createQualfiedResourcePath("" + guid + "/" + "UIProject.js");
		script.addEventListener("load",function() {
			var v1 = 1;
			_g.m_projectCacheState.set(guid,v1);
			v1;
			_g.invalidateScreenTypeCache();
			_g.initQueuedScreens(guid);
		});
		script.addEventListener("error",function() {
			_g.m_projectCacheState.remove(guid);
			_g.m_projectScriptCache.remove(guid);
			_g.m_screenLoadQueue.remove(guid);
			head.removeChild(script);
			throw new js__$Boot_HaxeError("UI Project \"" + guid + "\" is missing \"" + "UIProject.js" + "\" in the root.");
		});
		{
			this.m_projectScriptCache.set(guid,script);
			script;
		}
		head.appendChild(script);
	}
	,initQueuedScreens: function(guid) {
		var queued = this.m_screenLoadQueue.get(guid);
		if(queued != null) {
			var _g = 0;
			while(_g < queued.length) {
				var callback = queued[_g];
				++_g;
				callback();
			}
			this.m_screenLoadQueue.remove(guid);
		}
	}
	,queueScreen: function(guid,callback) {
		if(this.m_screenLoadQueue.exists(guid)) this.m_screenLoadQueue.get(guid).push(callback); else this.m_screenLoadQueue.set(guid,[callback]);
	}
	,createScreen: function(path,id,priority,data) {
		var guid = this.getProjectGUID(path);
		var readyCallback = (function(f,a1,a2,id1,a3,a4) {
			return function() {
				f(a1,a2,id1,a3,a4);
			};
		})($bind(this,this.onScreenElementReady),guid,this.createQualfiedResourcePath(path),id,priority,data);
		var _g = this.m_projectCacheState.get(guid);
		if(_g == null) {
			this.queueScreen(guid,readyCallback);
			this.initProjectConfig(guid);
		} else switch(_g) {
		case 0:
			this.queueScreen(guid,readyCallback);
			break;
		case 1:
			readyCallback();
			break;
		}
	}
	,cacheScreenLayout: function(qualifiedPath,link) {
		var layout = new ui_ScreenLayoutCache();
		layout.link = link;
		layout.template = link["import"].querySelector("template");
		if(layout.template == null) throw new js__$Boot_HaxeError("Screen layout \"" + qualifiedPath + "\" missing <template>");
		var styleSheets = link["import"].querySelectorAll("link[rel=\"stylesheet\"]");
		if(styleSheets != null) {
			var styleContainer;
			var _this = window.document;
			styleContainer = _this.createElement("style");
			var _g = 0;
			while(_g < styleSheets.length) {
				var node = styleSheets[_g];
				++_g;
				var sheet = node;
				styleContainer.innerHTML += "@import url(" + sheet.href + ");";
			}
			layout.template.content.appendChild(styleContainer);
		}
		var meta = link["import"].querySelector("meta");
		if(meta == null || !meta.hasAttribute("data-screen-logic")) throw new js__$Boot_HaxeError("Screen layout \"" + qualifiedPath + "\" missing meta." + "data-screen-logic");
		var handler = meta.getAttribute("data-screen-logic");
		layout.logicHandlerType = this.m_screenTypeCache.get(handler);
		if(layout.logicHandlerType == null) throw new js__$Boot_HaxeError("Unknown screen logic handler type \"" + handler + "\" in \"" + qualifiedPath + "\".");
		{
			this.m_screenLayoutCache.set(qualifiedPath,layout);
			layout;
		}
	}
	,onScreenElementReady: function(project,qualifiedPath,id,priority,data) {
		var _g = this;
		var cached = this.m_screenLayoutCache.get(qualifiedPath);
		var readyCallback = (function(f,a1,id1,a3,a4) {
			return function(a2) {
				f(a1,a2,id1,a3,a4);
			};
		})($bind(this,this.onScreenLayoutReady),project,id,priority,data);
		if(cached == null) {
			var head = window.document.head;
			var link;
			var _this = window.document;
			link = _this.createElement("link");
			link.rel = "import";
			link.href = "" + qualifiedPath + "?t=" + new Date().getTime();
			link.addEventListener("load",function() {
				try {
					_g.cacheScreenLayout(qualifiedPath,link);
				} catch( e ) {
					if (e instanceof js__$Boot_HaxeError) e = e.val;
					if( js_Boot.__instanceof(e,String) ) {
						head.removeChild(link);
						throw new js__$Boot_HaxeError("Unable to construct screen. error: " + e);
					} else throw(e);
				}
				readyCallback(_g.m_screenLayoutCache.get(qualifiedPath));
			});
			link.addEventListener("error",function() {
				head.removeChild(link);
				throw new js__$Boot_HaxeError("Failed to load screen layout " + qualifiedPath);
			});
			head.appendChild(link);
		} else readyCallback(cached);
	}
	,onScreenLayoutReady: function(project,layout,id,priority,data) {
		var container;
		var _this = window.document;
		container = _this.createElement("div");
		container.classList.add("screen");
		if(priority == null) container.style.zIndex = "null"; else container.style.zIndex = "" + priority;
		var element = container.createShadowRoot();
		element.appendChild(window.document.importNode(layout.template.content,true));
		this.m_container.appendChild(container);
		var config = { project : project, owner : this, id : id, container : element, data : data};
		var screen = Type.createInstance(layout.logicHandlerType,[config]);
		this.m_screens.h[id] = screen;
	}
	,getProjectGUID: function(path) {
		return haxe_io_Path.normalize(path).split("/")[0];
	}
	,createQualfiedResourcePath: function(path) {
		return haxe_io_Path.normalize("" + "http://game" + "/" + path);
	}
	,forEachFocusedScreen: function(callback) {
		var $it0 = this.m_screens.iterator();
		while( $it0.hasNext() ) {
			var screen = $it0.next();
			if(this.m_nativeManager.screenHasFocus(screen.getID())) callback(screen);
		}
	}
	,onScreenAdded: function(e) {
		this.createScreen(e.path,e.id,e.priority,e.initData);
	}
	,onScreenMessaged: function(e) {
		var screen;
		var key = e.id;
		screen = this.m_screens.h[key];
		if(screen != null) screen.events.trigger(e.message,e.data);
	}
	,onScreensCleared: function() {
		this.clearScreens();
	}
	,onScreenExited: function(e) {
		var screen;
		var key = e.id;
		screen = this.m_screens.h[key];
		if(screen != null) screen.exit();
	}
	,onGamepadBtnDown: function(e) {
		this.forEachFocusedScreen(function(screen) {
			screen.events.trigger("GamepadButtonDown",e);
		});
	}
	,onGamepadBtnUp: function(e) {
		this.forEachFocusedScreen(function(screen) {
			screen.events.trigger("GamepadButtonUp",e);
		});
	}
	,onGamepadConnected: function(e) {
		this.forEachFocusedScreen(function(screen) {
			screen.events.trigger("GamepadConnected",e);
		});
	}
	,onGamepadDisconnected: function(e) {
		this.forEachFocusedScreen(function(screen) {
			screen.events.trigger("GamepadDisconnected",e);
		});
	}
	,onKeyDown: function(e) {
		this.forEachFocusedScreen(function(screen) {
			screen.events.trigger("KeyboardKeyDown",e);
		});
	}
	,onKeyUp: function(e) {
		this.forEachFocusedScreen(function(screen) {
			screen.events.trigger("KeyboardKeyUp",e);
		});
	}
	,__class__: ui_ScreenManager
};
var ursine_api_events_EventManager = function() {
	this.m_events = new haxe_ds_StringMap();
};
ursine_api_events_EventManager.__name__ = true;
ursine_api_events_EventManager.prototype = {
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
	,__class__: ursine_api_events_EventManager
};
var ursine_api_events_IEventContainer = function() { };
ursine_api_events_IEventContainer.__name__ = true;
var ursine_api_native_NativeBroadcastManager = function() {
	this.m_channels = new haxe_ds_StringMap();
	ursine_api_native_NativeBroadcastManager.instance = this;
	window.NativeBroadcast = ursine_api_native_NativeBroadcastManager.onBroadcast;
};
ursine_api_native_NativeBroadcastManager.__name__ = true;
ursine_api_native_NativeBroadcastManager.onBroadcast = function(target,message,data) {
	ursine_api_native_NativeBroadcastManager.instance.getChannel(target).trigger(message,data);
};
ursine_api_native_NativeBroadcastManager.prototype = {
	getChannel: function(name) {
		var channel = this.m_channels.get(name);
		if(channel == null) {
			channel = new ursine_api_events_EventManager();
			this.m_channels.set(name,channel);
		}
		return channel;
	}
	,__class__: ursine_api_native_NativeBroadcastManager
};
var ursine_api_ui_Screen = function() { };
ursine_api_ui_Screen.__name__ = true;
ursine_api_ui_Screen.__interfaces__ = [ursine_api_events_IEventContainer];
ursine_api_ui_Screen.prototype = {
	getID: function() {
		return this.m_id;
	}
	,getHost: function() {
		return this.m_container.host;
	}
	,exit: function() {
		this.m_owner.removeScreen(this);
	}
	,__class__: ursine_api_ui_Screen
};
var $_, $fid = 0;
function $bind(o,m) { if( m == null ) return null; if( m.__id__ == null ) m.__id__ = $fid++; var f; if( o.hx__closures__ == null ) o.hx__closures__ = {}; else f = o.hx__closures__[m.__id__]; if( f == null ) { f = function(){ return f.method.apply(f.scope, arguments); }; f.scope = o; f.method = m; o.hx__closures__[m.__id__] = f; } return f; }
String.prototype.__class__ = String;
String.__name__ = true;
Array.__name__ = true;
Date.prototype.__class__ = Date;
Date.__name__ = ["Date"];
var Int = { __name__ : ["Int"]};
var Dynamic = { __name__ : ["Dynamic"]};
var Float = Number;
Float.__name__ = ["Float"];
var Bool = Boolean;
Bool.__ename__ = ["Bool"];
var Class = { __name__ : ["Class"]};
var Enum = { };
var __map_reserved = {}
js_Boot.__toStr = {}.toString;
Game.main();
})(typeof window != "undefined" ? window : typeof global != "undefined" ? global : typeof self != "undefined" ? self : this);
