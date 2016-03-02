(function ($global) { "use strict";
var $hxClasses = {};
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
$hxClasses["EReg"] = EReg;
EReg.__name__ = ["EReg"];
EReg.prototype = {
	match: function(s) {
		if(this.r.global) this.r.lastIndex = 0;
		this.r.m = this.r.exec(s);
		this.r.s = s;
		return this.r.m != null;
	}
	,matched: function(n) {
		if(this.r.m != null && n >= 0 && n < this.r.m.length) return this.r.m[n]; else throw new js__$Boot_HaxeError("EReg::matched");
	}
	,matchedLeft: function() {
		if(this.r.m == null) throw new js__$Boot_HaxeError("No string matched");
		return HxOverrides.substr(this.r.s,0,this.r.m.index);
	}
	,replace: function(s,by) {
		return s.replace(this.r,by);
	}
	,__class__: EReg
};
var EditorMain = function() { };
$hxClasses["EditorMain"] = EditorMain;
EditorMain.__name__ = ["EditorMain"];
EditorMain.main = function() {
	window.addEventListener("WebComponentsReady",EditorMain.initWindows);
};
EditorMain.initWindows = function() {
	var editor = new ursine_editor_Editor();
	var mainDockContainer = window.document.body.querySelector("#main-dock-container");
	var mainDock = new DockContainerControl();
	mainDock.style.width = "100%";
	mainDock.style.height = "100%";
	mainDockContainer.appendChild(mainDock);
	var sceneView = new ursine_editor_windows_SceneView();
	var entityInspector = new ursine_editor_windows_EntityInspector();
	var sceneOutline = new ursine_editor_windows_SceneOutline();
	var projectBrowser = new ursine_editor_windows_ProjectBrowser();
	var leftColumn = mainDock.addColumn();
	leftColumn.style.width = "20%";
	var row = leftColumn.addRow();
	row.style.height = "100%";
	var column = row.addColumn();
	column.style.width = "100%";
	column.appendChild(entityInspector.window);
	var rightColumn = mainDock.addColumn();
	rightColumn.style.width = "80%";
	var topRow = rightColumn.addRow();
	topRow.style.height = "65%";
	var innerLeftColumn = topRow.addColumn();
	innerLeftColumn.style.width = "80%";
	var row1 = innerLeftColumn.addRow();
	row1.style.height = "100%";
	var column1 = row1.addColumn();
	column1.style.width = "100%";
	column1.appendChild(sceneView.window);
	var innerRightColumn = topRow.addColumn();
	innerRightColumn.style.width = "20%";
	var row2 = innerRightColumn.addRow();
	row2.style.height = "100%";
	var column2 = row2.addColumn();
	column2.style.width = "100%";
	column2.appendChild(sceneOutline.window);
	var bottomRow = rightColumn.addRow();
	bottomRow.style.height = "35%";
	var column3 = bottomRow.addColumn();
	column3.style.width = "100%";
	var row3 = column3.addRow();
	row3.style.height = "100%";
	var column4 = row3.addColumn();
	column4.style.width = "100%";
	column4.appendChild(projectBrowser.window);
	haxe_Timer.delay($bind(sceneView,sceneView.onViewportInvalidated),100);
};
var HxOverrides = function() { };
$hxClasses["HxOverrides"] = HxOverrides;
HxOverrides.__name__ = ["HxOverrides"];
HxOverrides.cca = function(s,index) {
	var x = s.charCodeAt(index);
	if(x != x) return undefined;
	return x;
};
HxOverrides.substr = function(s,pos,len) {
	if(pos != null && pos != 0 && len != null && len < 0) return "";
	if(len == null) len = s.length;
	if(pos < 0) {
		pos = s.length + pos;
		if(pos < 0) pos = 0;
	} else if(len < 0) len = s.length + len - pos;
	return s.substr(pos,len);
};
HxOverrides.remove = function(a,obj) {
	var i = a.indexOf(obj);
	if(i == -1) return false;
	a.splice(i,1);
	return true;
};
HxOverrides.iter = function(a) {
	return { cur : 0, arr : a, hasNext : function() {
		return this.cur < this.arr.length;
	}, next : function() {
		return this.arr[this.cur++];
	}};
};
var Lambda = function() { };
$hxClasses["Lambda"] = Lambda;
Lambda.__name__ = ["Lambda"];
Lambda.find = function(it,f) {
	var $it0 = $iterator(it)();
	while( $it0.hasNext() ) {
		var v = $it0.next();
		if(f(v)) return v;
	}
	return null;
};
Math.__name__ = ["Math"];
var Reflect = function() { };
$hxClasses["Reflect"] = Reflect;
Reflect.__name__ = ["Reflect"];
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
$hxClasses["Std"] = Std;
Std.__name__ = ["Std"];
Std.string = function(s) {
	return js_Boot.__string_rec(s,"");
};
Std["int"] = function(x) {
	return x | 0;
};
Std.parseInt = function(x) {
	var v = parseInt(x,10);
	if(v == 0 && (HxOverrides.cca(x,1) == 120 || HxOverrides.cca(x,1) == 88)) v = parseInt(x);
	if(isNaN(v)) return null;
	return v;
};
var StringTools = function() { };
$hxClasses["StringTools"] = StringTools;
StringTools.__name__ = ["StringTools"];
StringTools.replace = function(s,sub,by) {
	return s.split(sub).join(by);
};
var Type = function() { };
$hxClasses["Type"] = Type;
Type.__name__ = ["Type"];
Type.getSuperClass = function(c) {
	return c.__super__;
};
Type.getClassName = function(c) {
	var a = c.__name__;
	if(a == null) return null;
	return a.join(".");
};
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
var _$UInt_UInt_$Impl_$ = {};
$hxClasses["_UInt.UInt_Impl_"] = _$UInt_UInt_$Impl_$;
_$UInt_UInt_$Impl_$.__name__ = ["_UInt","UInt_Impl_"];
_$UInt_UInt_$Impl_$.toFloat = function(this1) {
	var $int = this1;
	if($int < 0) return 4294967296.0 + $int; else return $int + 0.0;
};
var haxe_IMap = function() { };
$hxClasses["haxe.IMap"] = haxe_IMap;
haxe_IMap.__name__ = ["haxe","IMap"];
var haxe_Timer = function(time_ms) {
	var me = this;
	this.id = setInterval(function() {
		me.run();
	},time_ms);
};
$hxClasses["haxe.Timer"] = haxe_Timer;
haxe_Timer.__name__ = ["haxe","Timer"];
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
	,__class__: haxe_Timer
};
var haxe_ds_IntMap = function() {
	this.h = { };
};
$hxClasses["haxe.ds.IntMap"] = haxe_ds_IntMap;
haxe_ds_IntMap.__name__ = ["haxe","ds","IntMap"];
haxe_ds_IntMap.__interfaces__ = [haxe_IMap];
haxe_ds_IntMap.prototype = {
	remove: function(key) {
		if(!this.h.hasOwnProperty(key)) return false;
		delete(this.h[key]);
		return true;
	}
	,__class__: haxe_ds_IntMap
};
var haxe_ds__$StringMap_StringMapIterator = function(map,keys) {
	this.map = map;
	this.keys = keys;
	this.index = 0;
	this.count = keys.length;
};
$hxClasses["haxe.ds._StringMap.StringMapIterator"] = haxe_ds__$StringMap_StringMapIterator;
haxe_ds__$StringMap_StringMapIterator.__name__ = ["haxe","ds","_StringMap","StringMapIterator"];
haxe_ds__$StringMap_StringMapIterator.prototype = {
	hasNext: function() {
		return this.index < this.count;
	}
	,next: function() {
		return this.map.get(this.keys[this.index++]);
	}
	,__class__: haxe_ds__$StringMap_StringMapIterator
};
var haxe_ds_StringMap = function() {
	this.h = { };
};
$hxClasses["haxe.ds.StringMap"] = haxe_ds_StringMap;
haxe_ds_StringMap.__name__ = ["haxe","ds","StringMap"];
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
	,keys: function() {
		var _this = this.arrayKeys();
		return HxOverrides.iter(_this);
	}
	,arrayKeys: function() {
		var out = [];
		for( var key in this.h ) {
		if(this.h.hasOwnProperty(key)) out.push(key);
		}
		if(this.rh != null) {
			for( var key in this.rh ) {
			if(key.charCodeAt(0) == 36) out.push(key.substr(1));
			}
		}
		return out;
	}
	,iterator: function() {
		return new haxe_ds__$StringMap_StringMapIterator(this,this.arrayKeys());
	}
	,__class__: haxe_ds_StringMap
};
var haxe_rtti_Meta = function() { };
$hxClasses["haxe.rtti.Meta"] = haxe_rtti_Meta;
haxe_rtti_Meta.__name__ = ["haxe","rtti","Meta"];
haxe_rtti_Meta.getType = function(t) {
	var meta = haxe_rtti_Meta.getMeta(t);
	if(meta == null || meta.obj == null) return { }; else return meta.obj;
};
haxe_rtti_Meta.getMeta = function(t) {
	return t.__meta__;
};
haxe_rtti_Meta.getStatics = function(t) {
	var meta = haxe_rtti_Meta.getMeta(t);
	if(meta == null || meta.statics == null) return { }; else return meta.statics;
};
var js__$Boot_HaxeError = function(val) {
	Error.call(this);
	this.val = val;
	this.message = String(val);
	if(Error.captureStackTrace) Error.captureStackTrace(this,js__$Boot_HaxeError);
};
$hxClasses["js._Boot.HaxeError"] = js__$Boot_HaxeError;
js__$Boot_HaxeError.__name__ = ["js","_Boot","HaxeError"];
js__$Boot_HaxeError.__super__ = Error;
js__$Boot_HaxeError.prototype = $extend(Error.prototype,{
	__class__: js__$Boot_HaxeError
});
var js_Boot = function() { };
$hxClasses["js.Boot"] = js_Boot;
js_Boot.__name__ = ["js","Boot"];
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
var ursine_editor_Editor = function() {
	ursine_editor_Editor.instance = this;
	this.mainMenu = new MainMenuControl();
	this.broadcastManager = new ursine_editor_NativeBroadcastManager();
	this.componentDatabase = new ursine_editor_scene_component_ComponentDatabase(ursine_native_Extern.GetNativeComponentDatabase());
	this.m_notificationManager = new ursine_editor_NativeNotificationManager(this.broadcastManager);
	this.buildMenus();
	window.document.querySelector("#header-toolbar").appendChild(this.mainMenu);
	this.initSimulationPlayback();
};
$hxClasses["ursine.editor.Editor"] = ursine_editor_Editor;
ursine_editor_Editor.__name__ = ["ursine","editor","Editor"];
ursine_editor_Editor.prototype = {
	buildMenus: function() {
		var classTypeNames = Object.keys( $hxClasses );
		var menuHandlerType = Type.resolveClass(Type.getClassName(ursine_editor_MenuItemHandler));
		var handlers = [];
		var _g = 0;
		while(_g < classTypeNames.length) {
			var className = classTypeNames[_g];
			++_g;
			var classType = Type.resolveClass(className);
			var base = Type.getSuperClass(classType);
			if(base == menuHandlerType) {
				var info = { };
				info.type = classType;
				var meta = haxe_rtti_Meta.getType(classType);
				if(Object.prototype.hasOwnProperty.call(meta,"menuIndex")) info.index = meta.menuIndex[0]; else info.index = Infinity;
				handlers.push(info);
			}
		}
		handlers.sort(function(a,b) {
			return a.index - b.index;
		});
		var _g1 = 0;
		while(_g1 < handlers.length) {
			var handler = handlers[_g1];
			++_g1;
			var statics = haxe_rtti_Meta.getStatics(handler.type);
			var fields = Reflect.fields(statics);
			var _g11 = 0;
			while(_g11 < fields.length) {
				var name = fields[_g11];
				++_g11;
				var field = Reflect.field(statics,name);
				if(!Object.prototype.hasOwnProperty.call(field,"mainMenuItem")) continue;
				var details = field.mainMenuItem;
				var levels = StringTools.replace(details[0],"\"","").split("/");
				var itemName = levels.pop();
				var parentMenu = this.populateMenuLevels(levels);
				var item = new MenuItemControl();
				item.text = itemName;
				item.icon = details[3];
				var handler1 = [Reflect.field(handler.type,name)];
				item.addEventListener("open",(function(handler1) {
					return function(e) {
						handler1[0].apply(handler1[0].type,[e]);
					};
				})(handler1));
				if(details[1] == true) parentMenu.appendChild(new MenuSeparatorControl());
				parentMenu.appendChild(item);
				if(details[2] == true) parentMenu.appendChild(new MenuSeparatorControl());
			}
		}
	}
	,populateMenuLevels: function(levels) {
		var parent = this.mainMenu;
		while(levels.length > 0) {
			var level = levels.shift();
			var item = parent.findItem(level);
			if(item == null) {
				item = new MenuItemControl();
				item.text = level;
				parent.appendChild(item);
			}
			parent = item.menu;
		}
		return parent;
	}
	,initSimulationPlayback: function() {
		var toolsContainer = window.document.querySelector("#simulation-tools");
		var btnPlay = window.document.querySelector("#simulation-play");
		var btnToggle = window.document.querySelector("#simulation-toggle");
		var btnStep = window.document.querySelector("#simulation-step");
		var btnStop = window.document.querySelector("#simulation-stop");
		btnPlay.addEventListener("click",function() {
			toolsContainer.classList.add("running");
			ursine_native_Extern.ScenePlayStart();
		});
		btnToggle.addEventListener("click",function() {
			var playing = !toolsContainer.classList.contains("paused");
			toolsContainer.classList.toggle("paused",playing);
			ursine_native_Extern.SceneSetPlayState(!playing);
		});
		btnStep.addEventListener("click",function() {
			if(btnStep.classList.contains("disabled")) return;
			ursine_native_Extern.SceneStep();
		});
		btnStop.addEventListener("click",function() {
			toolsContainer.classList.remove("running","paused");
			ursine_native_Extern.ScenePlayStop();
		});
	}
	,__class__: ursine_editor_Editor
};
var ursine_editor_MenuItemHandler = function() { };
$hxClasses["ursine.editor.MenuItemHandler"] = ursine_editor_MenuItemHandler;
ursine_editor_MenuItemHandler.__name__ = ["ursine","editor","MenuItemHandler"];
var ursine_editor_NativeBroadcastManager = function() {
	this.m_channels = new haxe_ds_StringMap();
	ursine_editor_NativeBroadcastManager.m_instance = this;
	window.NativeBroadcast = ursine_editor_NativeBroadcastManager.onBroadcast;
};
$hxClasses["ursine.editor.NativeBroadcastManager"] = ursine_editor_NativeBroadcastManager;
ursine_editor_NativeBroadcastManager.__name__ = ["ursine","editor","NativeBroadcastManager"];
ursine_editor_NativeBroadcastManager.onBroadcast = function(target,message,data) {
	ursine_editor_NativeBroadcastManager.m_instance.getChannel(target).trigger(message,data);
};
ursine_editor_NativeBroadcastManager.prototype = {
	getChannel: function(name) {
		var channel = this.m_channels.get(name);
		if(channel == null) {
			channel = new ursine_utils_EventManager();
			this.m_channels.set(name,channel);
		}
		return channel;
	}
	,__class__: ursine_editor_NativeBroadcastManager
};
var ursine_editor_WindowHandler = function() {
	this.window = new EditorWindowControl();
};
$hxClasses["ursine.editor.WindowHandler"] = ursine_editor_WindowHandler;
ursine_editor_WindowHandler.__name__ = ["ursine","editor","WindowHandler"];
ursine_editor_WindowHandler.prototype = {
	__class__: ursine_editor_WindowHandler
};
var ursine_editor_NativeCanvasWindowHandler = function(toolName) {
	ursine_editor_WindowHandler.call(this);
	this.m_nativeHandler = new NativeToolHandler(toolName);
	var _g = 0;
	var _g1 = ursine_editor_NativeCanvasWindowHandler.m_forwardedEvents;
	while(_g < _g1.length) {
		var event = _g1[_g];
		++_g;
		this.window.addEventListener(event,$bind(this,this.forwardEvent));
	}
	this.window.addEventListener("resize",$bind(this,this.onViewportInvalidated));
	window.addEventListener("resize",$bind(this,this.onViewportInvalidated));
};
$hxClasses["ursine.editor.NativeCanvasWindowHandler"] = ursine_editor_NativeCanvasWindowHandler;
ursine_editor_NativeCanvasWindowHandler.__name__ = ["ursine","editor","NativeCanvasWindowHandler"];
ursine_editor_NativeCanvasWindowHandler.__super__ = ursine_editor_WindowHandler;
ursine_editor_NativeCanvasWindowHandler.prototype = $extend(ursine_editor_WindowHandler.prototype,{
	forwardEvent: function(e) {
		this.m_nativeHandler.Event(e.type,e);
	}
	,onViewportInvalidated: function() {
		var bounds = this.window.container.getBoundingClientRect();
		this.m_nativeHandler.Event("viewportInvalidated",{ x : bounds.left, y : bounds.top, width : bounds.width, height : bounds.height});
	}
	,__class__: ursine_editor_NativeCanvasWindowHandler
});
var ursine_editor_NativeNotificationManager = function(broadcastManager) {
	this.m_notifications = new haxe_ds_IntMap();
	broadcastManager.getChannel("EditorNotification").on("created",$bind(this,this.onCreated)).on("close",$bind(this,this.onCloseRequested));
};
$hxClasses["ursine.editor.NativeNotificationManager"] = ursine_editor_NativeNotificationManager;
ursine_editor_NativeNotificationManager.__name__ = ["ursine","editor","NativeNotificationManager"];
ursine_editor_NativeNotificationManager.prototype = {
	onCreated: function(e) {
		var _g = this;
		var notification = new NotificationControl(e.type,e.message,e.header);
		notification.dismissible = e.dismissible;
		this.initNotificationButtons(e.id,notification,e.buttons);
		notification.addEventListener("closed",function() {
			ursine_native_Extern.NotificationCloseCallback(e.id);
			var key = e.id;
			_g.m_notifications.remove(key);
		});
		notification.show(e.duration);
		var k = e.id;
		this.m_notifications.h[k] = notification;
		notification;
	}
	,onCloseRequested: function(e) {
		var notification;
		var key = e.id;
		notification = this.m_notifications.h[key];
		if(notification == null) throw new js__$Boot_HaxeError("Invalid notification close requested: " + Std.string(e.id));
		notification.close();
	}
	,initNotificationButtons: function(id,notification,buttons) {
		var i = 0;
		var _g = 0;
		while(_g < buttons.length) {
			var button = buttons[_g];
			++_g;
			var element = new ButtonControl();
			element.text = button.text;
			element.addEventListener("click",(function(f,a1) {
				return function() {
					f(a1);
				};
			})(function(index) {
				ursine_native_Extern.NotificationButtonCallback(id,index);
			},i));
			notification.buttons.appendChild(element);
			++i;
		}
	}
	,__class__: ursine_editor_NativeNotificationManager
};
var ursine_editor_menus_DebugMenu = function() { };
$hxClasses["ursine.editor.menus.DebugMenu"] = ursine_editor_menus_DebugMenu;
ursine_editor_menus_DebugMenu.__name__ = ["ursine","editor","menus","DebugMenu"];
ursine_editor_menus_DebugMenu.doEditorReload = function() {
	window.location.reload(true);
};
ursine_editor_menus_DebugMenu.doEditorDebugTools = function() {
	editor_commands_InspectEditorUI();
};
ursine_editor_menus_DebugMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_DebugMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
	__class__: ursine_editor_menus_DebugMenu
});
var ursine_editor_menus_EditMenu = function() { };
$hxClasses["ursine.editor.menus.EditMenu"] = ursine_editor_menus_EditMenu;
ursine_editor_menus_EditMenu.__name__ = ["ursine","editor","menus","EditMenu"];
ursine_editor_menus_EditMenu.doUndo = function() {
	console.log("undo");
};
ursine_editor_menus_EditMenu.doRedo = function() {
	console.log("redo");
};
ursine_editor_menus_EditMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_EditMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
	__class__: ursine_editor_menus_EditMenu
});
var ursine_editor_menus_EntityMenu = function() { };
$hxClasses["ursine.editor.menus.EntityMenu"] = ursine_editor_menus_EntityMenu;
ursine_editor_menus_EntityMenu.__name__ = ["ursine","editor","menus","EntityMenu"];
ursine_editor_menus_EntityMenu.doCreateEmpty = function() {
	editor_commands_CreateEmptyEntity();
};
ursine_editor_menus_EntityMenu.doCreateFromArchetype = function() {
	editor_commands_CreateEntityFromArchetype();
};
ursine_editor_menus_EntityMenu.doCreatePlane = function() {
	editor_commands_CreatePlane();
};
ursine_editor_menus_EntityMenu.doCreateBox = function() {
	editor_commands_CreateBox();
};
ursine_editor_menus_EntityMenu.doCreateCylinder = function() {
	editor_commands_CreateCylinder();
};
ursine_editor_menus_EntityMenu.doCreateSphere = function() {
	editor_commands_CreateSphere();
};
ursine_editor_menus_EntityMenu.doCreatePointLight = function() {
	editor_commands_CreatePointLight();
};
ursine_editor_menus_EntityMenu.doCreateSpotLight = function() {
	editor_commands_CreateSpotLight();
};
ursine_editor_menus_EntityMenu.doCreateDirectionalLight = function() {
	editor_commands_CreateDirectionalLight();
};
ursine_editor_menus_EntityMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_EntityMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
	__class__: ursine_editor_menus_EntityMenu
});
var ursine_editor_menus_FileMenu = function() { };
$hxClasses["ursine.editor.menus.FileMenu"] = ursine_editor_menus_FileMenu;
ursine_editor_menus_FileMenu.__name__ = ["ursine","editor","menus","FileMenu"];
ursine_editor_menus_FileMenu.doNewWorld = function() {
	ursine_native_Extern.ProjectSetEmptyScene();
};
ursine_editor_menus_FileMenu.doSaveWorld = function() {
	ursine_native_Extern.SceneSaveWorld();
};
ursine_editor_menus_FileMenu.doSaveWorldAs = function() {
	ursine_native_Extern.SceneSaveWorldAs();
};
ursine_editor_menus_FileMenu.doSaveProject = function() {
	var notification = new NotificationControl(3,"Thanks for saving fella, but this doesn't do anything right now.","Save Project");
	notification.show();
};
ursine_editor_menus_FileMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_FileMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
	__class__: ursine_editor_menus_FileMenu
});
var ursine_editor_menus_HelpMenu = function() { };
$hxClasses["ursine.editor.menus.HelpMenu"] = ursine_editor_menus_HelpMenu;
ursine_editor_menus_HelpMenu.__name__ = ["ursine","editor","menus","HelpMenu"];
ursine_editor_menus_HelpMenu.doOpenGettingStarted = function() {
	ursine_native_Extern.ProcessOpen("Assets/UI/Resources/Documentation/html/index.html",true);
};
ursine_editor_menus_HelpMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_HelpMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
	__class__: ursine_editor_menus_HelpMenu
});
var ursine_editor_menus_ToolsMenu = function() { };
$hxClasses["ursine.editor.menus.ToolsMenu"] = ursine_editor_menus_ToolsMenu;
ursine_editor_menus_ToolsMenu.__name__ = ["ursine","editor","menus","ToolsMenu"];
ursine_editor_menus_ToolsMenu.uniConnector = function() {
	ursine_native_Extern.WaypointUnidirectionalConnector();
};
ursine_editor_menus_ToolsMenu.biConnector = function() {
	ursine_native_Extern.WaypointBidirectionalConnector();
};
ursine_editor_menus_ToolsMenu.enableLines = function() {
	ursine_native_Extern.WaypointEnableDrawLines();
};
ursine_editor_menus_ToolsMenu.disableLines = function() {
	ursine_native_Extern.WaypointDisableDrawLines();
};
ursine_editor_menus_ToolsMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_ToolsMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
	__class__: ursine_editor_menus_ToolsMenu
});
var ursine_editor_scene_component_ComponentDatabase = function(database) {
	this.m_componentInspectionHandlers = new haxe_ds_StringMap();
	this.m_fieldInspectionHandlers = new haxe_ds_StringMap();
	this.m_typeDB = new haxe_ds_StringMap();
	this.m_db = new haxe_ds_StringMap();
	var types = Reflect.fields(database.types);
	var _g = 0;
	while(_g < types.length) {
		var name = types[_g];
		++_g;
		var type = Reflect.field(database.types,name);
		this.m_typeDB.set(name,type);
	}
	var components = Reflect.fields(database.components);
	var _g1 = 0;
	while(_g1 < components.length) {
		var name1 = components[_g1];
		++_g1;
		var component = Reflect.field(database.components,name1);
		this.m_db.set(name1,component);
	}
	this.initComponentInspectors();
	this.initFieldInspectors();
};
$hxClasses["ursine.editor.scene.component.ComponentDatabase"] = ursine_editor_scene_component_ComponentDatabase;
ursine_editor_scene_component_ComponentDatabase.__name__ = ["ursine","editor","scene","component","ComponentDatabase"];
ursine_editor_scene_component_ComponentDatabase.prototype = {
	getNativeType: function(name) {
		return this.m_typeDB.get(name);
	}
	,getEnumValue: function(enewm,key) {
		var result = Lambda.find(enewm,function(entry) {
			return entry.key == key;
		});
		if(result == null) return null; else return result.value;
	}
	,getEnumNumberValue: function(enewm,value) {
		if(!(typeof(value) == "string")) return value;
		var result = Lambda.find(enewm,function(entry) {
			return entry.key == value;
		});
		if(result == null) return null; else return result.value;
	}
	,getComponentTypes: function() {
		var keys = [];
		var $it0 = this.m_db.keys();
		while( $it0.hasNext() ) {
			var key = $it0.next();
			keys.push(key);
		}
		return keys;
	}
	,getComponentType: function(name) {
		return this.m_db.get(name);
	}
	,getComponentTypeField: function(type,name) {
		return Lambda.find(type.fields,function(f) {
			return f.name == name;
		});
	}
	,createComponentInspector: function(entity,inspection) {
		var handler = this.m_componentInspectionHandlers.get(inspection.type);
		if(handler == null) handler = ursine_editor_scene_component_inspectors_components_DefaultComponentInspector;
		return Type.createInstance(handler,[entity,inspection]);
	}
	,createFieldInspector: function(owner,instance,field,type) {
		var typeOverride = Reflect.field(field.meta,ursine_native_Property.ForceEditorType);
		var typeName;
		if(typeOverride == null) typeName = type.name; else {
			typeName = Reflect.field(typeOverride,"typeName");
			type = this.m_typeDB.get(typeName);
		}
		var handler = this.m_fieldInspectionHandlers.get(typeName);
		if(handler == null) handler = ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector;
		return Type.createInstance(handler,[owner,instance,field,type]);
	}
	,initComponentInspectors: function() {
		var inspectors = ursine_utils_MetaUtils.getDerivedClasses(ursine_editor_scene_component_inspectors_ComponentInspectionHandler);
		var _g = 0;
		while(_g < inspectors.length) {
			var inspector = inspectors[_g];
			++_g;
			var meta = haxe_rtti_Meta.getType(inspector);
			if(!Object.prototype.hasOwnProperty.call(meta,ursine_editor_scene_component_ComponentDatabase.m_componentInspectorMeta)) continue;
			var componentNames = Reflect.field(meta,ursine_editor_scene_component_ComponentDatabase.m_componentInspectorMeta);
			var _g1 = 0;
			while(_g1 < componentNames.length) {
				var name = componentNames[_g1];
				++_g1;
				this.m_componentInspectionHandlers.set(name,inspector);
			}
		}
	}
	,initFieldInspectors: function() {
		var inspectors = ursine_utils_MetaUtils.getDerivedClasses(ursine_editor_scene_component_inspectors_FieldInspectionHandler);
		var _g = 0;
		while(_g < inspectors.length) {
			var inspector = inspectors[_g];
			++_g;
			var meta = haxe_rtti_Meta.getType(inspector);
			if(!Object.prototype.hasOwnProperty.call(meta,ursine_editor_scene_component_ComponentDatabase.m_fieldInspectorMeta)) continue;
			var componentNames = Reflect.field(meta,ursine_editor_scene_component_ComponentDatabase.m_fieldInspectorMeta);
			var _g1 = 0;
			while(_g1 < componentNames.length) {
				var name = componentNames[_g1];
				++_g1;
				this.m_fieldInspectionHandlers.set(name,inspector);
			}
		}
	}
	,__class__: ursine_editor_scene_component_ComponentDatabase
};
var ursine_editor_scene_component_inspectors_IFieldInspectionOwner = function() { };
$hxClasses["ursine.editor.scene.component.inspectors.IFieldInspectionOwner"] = ursine_editor_scene_component_inspectors_IFieldInspectionOwner;
ursine_editor_scene_component_inspectors_IFieldInspectionOwner.__name__ = ["ursine","editor","scene","component","inspectors","IFieldInspectionOwner"];
ursine_editor_scene_component_inspectors_IFieldInspectionOwner.prototype = {
	__class__: ursine_editor_scene_component_inspectors_IFieldInspectionOwner
};
var ursine_editor_scene_component_inspectors_ComponentInspectionHandler = function(entity,component) {
	this.entity = entity;
	this.component = component;
	this.componentType = ursine_editor_Editor.instance.componentDatabase.getComponentType(component.type);
	this.m_fieldHandlers = new haxe_ds_StringMap();
	this.fieldChangeEvents = new ursine_utils_EventManager();
	this.fieldArrayInsertEvents = new ursine_utils_EventManager();
	this.fieldArrayItemSetEvents = new ursine_utils_EventManager();
	this.fieldArrayItemRemoveEvents = new ursine_utils_EventManager();
	this.inspector = new ComponentInspectorControl();
	this.inspector.heading = component.type;
};
$hxClasses["ursine.editor.scene.component.inspectors.ComponentInspectionHandler"] = ursine_editor_scene_component_inspectors_ComponentInspectionHandler;
ursine_editor_scene_component_inspectors_ComponentInspectionHandler.__name__ = ["ursine","editor","scene","component","inspectors","ComponentInspectionHandler"];
ursine_editor_scene_component_inspectors_ComponentInspectionHandler.__interfaces__ = [ursine_editor_scene_component_inspectors_IFieldInspectionOwner];
ursine_editor_scene_component_inspectors_ComponentInspectionHandler.prototype = {
	copyInstance: function() {
		return this.entity.inspectComponent(this.component.type);
	}
	,updateField: function(name,value) {
		this.component.value[name] = value;
		var result = this.fieldChangeEvents.trigger(name,{ name : name, value : value});
		if(result != false) {
			var handler = this.m_fieldHandlers.get(name);
			if(handler != null) handler.updateValue(value);
		}
	}
	,arrayInsert: function(name,index,value) {
		var result = this.fieldArrayInsertEvents.trigger(name,{ name : name, index : index, value : value});
		if(result != false) {
			var handler = this.m_fieldHandlers.get(name);
			if(handler != null) handler.arrayInsert(index,value);
		}
	}
	,arraySet: function(name,index,value) {
		var result = this.fieldArrayInsertEvents.trigger(name,{ name : name, index : index, value : value});
		if(result != false) {
			var handler = this.m_fieldHandlers.get(name);
			if(handler != null) handler.arraySet(index,value);
		}
	}
	,arrayRemove: function(name,index) {
		var result = this.fieldArrayInsertEvents.trigger(name,{ name : name, index : index});
		if(result != false) {
			var handler = this.m_fieldHandlers.get(name);
			if(handler != null) handler.arrayRemove(index);
		}
	}
	,addButton: function(button) {
		var _g = this;
		var element = new ButtonControl();
		element.classList.add("x-component-inspector");
		element.text = button.text;
		element.addEventListener("click",function() {
			_g.entity.componentButtonInvoke(_g.component.type,button.name);
		});
		this.inspector.buttons.appendChild(element);
	}
	,addField: function(field) {
		var k = field.get_name();
		this.m_fieldHandlers.set(k,field);
		field;
		this.inspector.fieldInspectors.appendChild(field.inspector);
	}
	,removeField: function(field) {
		var key = field.get_name();
		this.m_fieldHandlers.remove(key);
		this.inspector.fieldInspectors.removeChild(field.inspector);
	}
	,remove: function() {
		var $it0 = this.m_fieldHandlers.iterator();
		while( $it0.hasNext() ) {
			var handler = $it0.next();
			handler.remove();
		}
		if(this.inspector.parentNode != null) this.inspector.parentNode.removeChild(this.inspector);
	}
	,getFieldHandlers: function() {
		var handlers = [];
		var $it0 = this.m_fieldHandlers.iterator();
		while( $it0.hasNext() ) {
			var handler = $it0.next();
			handlers.push(handler);
		}
		return handlers;
	}
	,notifyChanged: function(handler,field,value) {
		this.entity.componentFieldUpdate(this.component.type,field.name,value);
	}
	,ownerNotifyChanged: function(handler,field,value) {
		this.notifyChanged(handler,field,value);
	}
	,__class__: ursine_editor_scene_component_inspectors_ComponentInspectionHandler
};
var ursine_editor_scene_component_inspectors_FieldInspectionHandler = function(owner,instance,field,type) {
	this.m_owner = owner;
	this.m_instance = instance;
	this.m_field = field;
	this.m_type = type;
	this.inspector = new FieldInspectorControl();
	var prettyName = ursine_editor_scene_component_inspectors_FieldInspectionHandler.m_fieldNameRegex.replace(field.name,"$1 ");
	this.inspector.heading = prettyName.charAt(0).toUpperCase() + HxOverrides.substr(prettyName,1,null);
	if(Object.prototype.hasOwnProperty.call(field.meta,ursine_native_Property.Annotation)) this.inspector.annotation = Reflect.field(field.meta,ursine_native_Property.Annotation).text;
	this.arrayIndex = 0;
};
$hxClasses["ursine.editor.scene.component.inspectors.FieldInspectionHandler"] = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_FieldInspectionHandler.__name__ = ["ursine","editor","scene","component","inspectors","FieldInspectionHandler"];
ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype = {
	updateValue: function(value) {
		this.m_instance = value;
	}
	,arrayInsert: function(index,value) {
	}
	,arraySet: function(index,value) {
	}
	,arrayRemove: function(index) {
	}
	,remove: function() {
		if(this.inspector.parentNode != null) this.inspector.parentNode.removeChild(this.inspector);
	}
	,notifyChanged: function(field,value) {
		this.m_owner.ownerNotifyChanged(this,field,value);
	}
	,get_name: function() {
		return this.m_field.name;
	}
	,__class__: ursine_editor_scene_component_inspectors_FieldInspectionHandler
};
var ursine_editor_scene_component_inspectors_components_DefaultComponentInspector = function(entity,component) {
	ursine_editor_scene_component_inspectors_ComponentInspectionHandler.call(this,entity,component);
	var database = ursine_editor_Editor.instance.componentDatabase;
	var componentType = database.getComponentType(component.type);
	var _g = 0;
	var _g1 = component.buttons;
	while(_g < _g1.length) {
		var button = _g1[_g];
		++_g;
		this.addButton(button);
	}
	var _g2 = 0;
	var _g11 = componentType.fields;
	while(_g2 < _g11.length) {
		var field = _g11[_g2];
		++_g2;
		var instance = Reflect.field(component.value,field.name);
		var type = database.getNativeType(field.type);
		this.addField(database.createFieldInspector(this,instance,field,type));
	}
};
$hxClasses["ursine.editor.scene.component.inspectors.components.DefaultComponentInspector"] = ursine_editor_scene_component_inspectors_components_DefaultComponentInspector;
ursine_editor_scene_component_inspectors_components_DefaultComponentInspector.__name__ = ["ursine","editor","scene","component","inspectors","components","DefaultComponentInspector"];
ursine_editor_scene_component_inspectors_components_DefaultComponentInspector.__super__ = ursine_editor_scene_component_inspectors_ComponentInspectionHandler;
ursine_editor_scene_component_inspectors_components_DefaultComponentInspector.prototype = $extend(ursine_editor_scene_component_inspectors_ComponentInspectionHandler.prototype,{
	__class__: ursine_editor_scene_component_inspectors_components_DefaultComponentInspector
});
var ursine_editor_scene_component_inspectors_components_LightInspector = function(entity,component) {
	ursine_editor_scene_component_inspectors_ComponentInspectionHandler.call(this,entity,component);
	this.fieldChangeEvents.on(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName,$bind(this,this.onLightTypeChange));
	this.m_typeFields = [];
	if(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum == null) this.initTypeToFields();
	var database = ursine_editor_Editor.instance.componentDatabase;
	var typeInstance = Reflect.field(component.value,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName);
	this.addField(database.createFieldInspector(this,typeInstance,database.getComponentTypeField(this.componentType,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName),database.getNativeType(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeName)));
	this.setType(typeInstance);
};
$hxClasses["ursine.editor.scene.component.inspectors.components.LightInspector"] = ursine_editor_scene_component_inspectors_components_LightInspector;
ursine_editor_scene_component_inspectors_components_LightInspector.__name__ = ["ursine","editor","scene","component","inspectors","components","LightInspector"];
ursine_editor_scene_component_inspectors_components_LightInspector.__super__ = ursine_editor_scene_component_inspectors_ComponentInspectionHandler;
ursine_editor_scene_component_inspectors_components_LightInspector.prototype = $extend(ursine_editor_scene_component_inspectors_ComponentInspectionHandler.prototype,{
	setType: function(type) {
		var database = ursine_editor_Editor.instance.componentDatabase;
		var componentType = database.getComponentType(this.component.type);
		while(this.m_typeFields.length > 0) this.m_typeFields.pop().remove();
		var fields;
		var key = database.getEnumNumberValue(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum,type);
		fields = ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields.h[key];
		var _g = 0;
		while(_g < fields.length) {
			var fieldName = fields[_g];
			++_g;
			var field = database.getComponentTypeField(componentType,fieldName);
			if(field == null) throw new js__$Boot_HaxeError("Unknown field \"" + fieldName + "\" on type \"" + ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeName + "\"");
			if(field.name == ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName) continue;
			var instance = Reflect.field(this.component.value,field.name);
			var type1 = database.getNativeType(field.type);
			var inspector = database.createFieldInspector(this,instance,field,type1);
			this.addField(inspector);
			this.m_typeFields.push(inspector);
		}
	}
	,onLightTypeChange: function(e) {
		this.setType(e.value);
	}
	,initTypeToFields: function() {
		var database = ursine_editor_Editor.instance.componentDatabase;
		ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum = database.getNativeType(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeName).enumValue;
		ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields = new haxe_ds_IntMap();
		var k = database.getEnumValue(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeDirectional);
		var v = ["color","intensity","renderMask"];
		ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields.h[k] = v;
		v;
		var k1 = database.getEnumValue(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypePoint);
		var v1 = ["color","intensity","radius","renderMask"];
		ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields.h[k1] = v1;
		v1;
		var k2 = database.getEnumValue(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeSpot);
		var v2 = ["color","intensity","spotlightAngles","renderMask"];
		ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields.h[k2] = v2;
		v2;
	}
	,__class__: ursine_editor_scene_component_inspectors_components_LightInspector
});
var ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector = function(parent,owner,instance,field,type) {
	this.m_parent = parent;
	this.m_owner = owner;
	this.m_instance = instance;
	this.m_field = field;
	this.m_type = type;
	this.m_arrayItems = [];
	this.initElements();
	this.updateValue(instance);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.ArrayTypeInspector"] = ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector;
ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","ArrayTypeInspector"];
ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector.__interfaces__ = [ursine_editor_scene_component_inspectors_IFieldInspectionOwner];
ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector.prototype = {
	updateValue: function(value) {
		this.m_itemsContainer.innerHTML = "";
		this.m_arrayItems = [];
		var _g1 = 0;
		var _g = value.length;
		while(_g1 < _g) {
			var i = _g1++;
			this.arrayInsert(i,value[i]);
			this.m_arrayItems[i].opened = false;
		}
	}
	,arrayInsert: function(index,value) {
		var _g = this;
		var handler = this.inspectItem(value);
		var container = new ArrayItemContainerControl();
		handler.arrayIndex = index;
		container.opened = true;
		container.container.appendChild(handler.inspector);
		container.handler = handler;
		container.header.addEventListener("contextmenu",function(e) {
			_g.openItemContextMenu(e,handler,container);
			e.preventDefault();
			e.stopPropagation();
			e.stopImmediatePropagation();
		});
		container.addEventListener("item-removed",function() {
			_g.m_owner.entity.componentFieldArrayRemove(_g.m_owner.component.type,_g.m_field.name,handler.arrayIndex);
		});
		this.m_arrayItems.splice(index,0,container);
		if((function($this) {
			var $r;
			var b = Math.max(0,$this.m_arrayItems.length - 1);
			$r = _$UInt_UInt_$Impl_$.toFloat(index) == b;
			return $r;
		}(this))) {
			this.m_itemsContainer.appendChild(container);
			container.index = index;
		} else {
			var item = this.m_arrayItems[index + 1];
			this.m_itemsContainer.insertBefore(item,container);
			var _g1 = index;
			var _g2 = this.m_arrayItems.length;
			while(_g1 < _g2) {
				var i = _g1++;
				var element = this.m_arrayItems[i];
				element.index = i;
			}
		}
	}
	,arraySet: function(index,value) {
		var container = this.m_arrayItems[index];
		if(container == null) throw new js__$Boot_HaxeError("Invalid array index.");
		container.handler.updateValue(value);
	}
	,arrayRemove: function(index) {
		var container = this.m_arrayItems[index];
		if(container == null) throw new js__$Boot_HaxeError("Invalid array index.");
		this.m_arrayItems.splice(index,1);
		this.m_itemsContainer.removeChild(container);
		var _g1 = index;
		var _g = this.m_arrayItems.length;
		while(_g1 < _g) {
			var i = _g1++;
			var element = this.m_arrayItems[i];
			element.index = i;
			element.handler.arrayIndex = i;
		}
	}
	,initElements: function() {
		this.m_parent.classList.add("array-owner");
		var itemsWrapper;
		var _this = window.document;
		itemsWrapper = _this.createElement("div");
		itemsWrapper.classList.add("array-items-wrapper");
		this.m_parent.container.appendChild(itemsWrapper);
		var _this1 = window.document;
		this.m_itemsContainer = _this1.createElement("div");
		this.m_itemsContainer.classList.add("array-items");
		itemsWrapper.appendChild(this.m_itemsContainer);
		this.m_btnAddItem = new ButtonControl();
		this.m_btnAddItem.text = "Add " + this.m_type.arrayType;
		this.m_btnAddItem.addEventListener("click",$bind(this,this.onAddItemClicked));
		itemsWrapper.appendChild(this.m_btnAddItem);
	}
	,getFieldHandlers: function() {
		var handlers = this.m_owner.getFieldHandlers();
		var _g = 0;
		var _g1 = this.m_arrayItems;
		while(_g < _g1.length) {
			var item = _g1[_g];
			++_g;
			handlers.push(item.handler);
		}
		return handlers;
	}
	,ownerNotifyChanged: function(handler,field,value) {
		this.m_owner.entity.componentFieldArrayUpdate(this.m_owner.component.type,field.name,handler.arrayIndex,value);
	}
	,inspectItem: function(value) {
		var database = ursine_editor_Editor.instance.componentDatabase;
		var arrayType = database.getNativeType(this.m_type.arrayType);
		return database.createFieldInspector(this,value,this.m_field,arrayType);
	}
	,openItemContextMenu: function(e,handler,container) {
		var _g = this;
		var menu = new ContextMenuControl();
		var moveUp = menu.addItem("Move Up",function() {
			_g.m_owner.entity.componentFieldArraySwap(_g.m_owner.component.type,_g.m_field.name,handler.arrayIndex,handler.arrayIndex - 1);
		});
		moveUp.icon = "arrow-up";
		moveUp.disabled = handler.arrayIndex == 0;
		var moveDown = menu.addItem("Move Down",function() {
			_g.m_owner.entity.componentFieldArraySwap(_g.m_owner.component.type,_g.m_field.name,handler.arrayIndex,handler.arrayIndex + 1);
		});
		moveDown.icon = "arrow-down";
		moveDown.disabled = handler.arrayIndex == this.m_arrayItems.length - 1;
		menu.addSeparator();
		var $delete = menu.addItem("Delete",function() {
			_g.m_owner.entity.componentFieldArrayRemove(_g.m_owner.component.type,_g.m_field.name,handler.arrayIndex);
		});
		$delete.icon = "remove";
		menu.open(e.clientX,e.clientY);
	}
	,onAddItemClicked: function(e) {
		this.m_owner.entity.componentFieldArrayPush(this.m_owner.component.type,this.m_field.name,{ });
	}
	,__class__: ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector
};
var ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector = function(owner,instance,field,type) {
	var _g = this;
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	this.m_bool = new CheckBoxInputControl();
	this.m_bool.addEventListener("change",function() {
		_g.notifyChanged(_g.m_field,_g.m_bool.checked);
	});
	this.inspector.container.appendChild(this.m_bool);
	this.updateValue(instance);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.BooleanFieldInspector"] = ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector;
ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","BooleanFieldInspector"];
ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	updateValue: function(value) {
		this.m_bool.checked = value;
		this.m_instance = value;
	}
	,__class__: ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector
});
var ursine_editor_scene_component_inspectors_fields_ColorFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	var previewContainer;
	var _this = window.document;
	previewContainer = _this.createElement("div");
	previewContainer.classList.add("color-preview");
	this.inspector.container.appendChild(previewContainer);
	var _this1 = window.document;
	this.m_colorPreview = _this1.createElement("div");
	this.m_colorPreview.addEventListener("click",$bind(this,this.onPreviewClick));
	previewContainer.appendChild(this.m_colorPreview);
	this.m_colorInput = null;
	this.updateValue(instance);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.ColorFieldInspector"] = ursine_editor_scene_component_inspectors_fields_ColorFieldInspector;
ursine_editor_scene_component_inspectors_fields_ColorFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","ColorFieldInspector"];
ursine_editor_scene_component_inspectors_fields_ColorFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_ColorFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	updateValue: function(value) {
		var red = Math.round(Reflect.field(value,"r") * 255);
		var green = Math.round(Reflect.field(value,"g") * 255);
		var blue = Math.round(Reflect.field(value,"b") * 255);
		var alpha = Reflect.field(value,"a").toPrecision(4);
		this.m_colorPreview.style.background = "rgba(" + red + ", " + green + ", " + blue + ", " + alpha + ")";
		this.m_instance = value;
	}
	,remove: function() {
		this.clearColorInput();
		ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype.remove.call(this);
	}
	,onPreviewClick: function(e) {
		if(this.m_colorInput != null) return;
		this.m_colorInput = new ColorInputControl(this.m_instance);
		this.m_colorInput.addEventListener("color-changed",$bind(this,this.onColorChanged));
		this.m_colorInput.addEventListener("closed",$bind(this,this.onColorClosed));
		window.document.body.appendChild(this.m_colorInput);
		this.m_colorInput.show(e.clientX,e.clientY);
		var handlers = this.m_owner.getFieldHandlers();
		var _g = 0;
		while(_g < handlers.length) {
			var handler = handlers[_g];
			++_g;
			if(js_Boot.__instanceof(handler,ursine_editor_scene_component_inspectors_fields_ColorFieldInspector) && handler != this) {
				var colorHandler = handler;
				colorHandler.clearColorInput();
			}
		}
	}
	,onColorChanged: function(e) {
		this.notifyChanged(this.m_field,e.detail.color);
	}
	,onColorClosed: function(e) {
		this.m_colorInput = null;
	}
	,clearColorInput: function() {
		if(this.m_colorInput != null) {
			window.document.body.removeChild(this.m_colorInput);
			this.m_colorInput = null;
		}
	}
	,__class__: ursine_editor_scene_component_inspectors_fields_ColorFieldInspector
});
var ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	if(type.isArray) this.initArray(); else if(type.enumValue != null) this.initEnum(); else this.initDefaultStruct();
	this.updateValue(instance);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.DefaultFieldInspector"] = ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector;
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","DefaultFieldInspector"];
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.__interfaces__ = [ursine_editor_scene_component_inspectors_IFieldInspectionOwner];
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	updateValue: function(value) {
		this.m_instance = value;
		if(this.m_isEnum) {
			if(this.m_isBitMaskEditor) this.loadEnumBitMaskValue(value); else this.m_comboInput.value = ursine_editor_Editor.instance.componentDatabase.getEnumNumberValue(this.m_type.enumValue,value);
		} else if(this.m_arrayInspector != null) this.m_arrayInspector.updateValue(value); else {
			var _g = 0;
			var _g1 = this.m_type.fields;
			while(_g < _g1.length) {
				var field = _g1[_g];
				++_g;
				var handler = this.m_structFieldHandlers.get(field.name);
				if(handler == null) throw new js__$Boot_HaxeError("Unable to find handler for field " + field.name);
				var fieldInstance = Reflect.field(value,field.name);
				handler.updateValue(fieldInstance);
			}
		}
	}
	,arrayInsert: function(index,value) {
		if(this.m_arrayInspector != null) this.m_arrayInspector.arrayInsert(index,value);
	}
	,arraySet: function(index,value) {
		if(this.m_arrayInspector != null) this.m_arrayInspector.arraySet(index,value);
	}
	,arrayRemove: function(index) {
		if(this.m_arrayInspector != null) this.m_arrayInspector.arrayRemove(index);
	}
	,getFieldHandlers: function() {
		var handlers = [];
		var $it0 = this.m_structFieldHandlers.iterator();
		while( $it0.hasNext() ) {
			var handler = $it0.next();
			handlers.push(handler);
		}
		return handlers;
	}
	,ownerNotifyChanged: function(handler,field,value) {
		if(this.m_isEnum) ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype.notifyChanged.call(this,field,value); else {
			this.m_instance[field.name] = value;
			this.m_owner.ownerNotifyChanged(this,this.m_field,this.m_instance);
		}
	}
	,initArray: function() {
		this.m_arrayInspector = new ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector(this.inspector,this.m_owner,this.m_instance,this.m_field,this.m_type);
	}
	,initEnum: function() {
		var _g = this;
		this.m_isEnum = true;
		this.m_comboInput = new ComboInputControl();
		this.m_enumValueOptions = new haxe_ds_StringMap();
		var _g1 = 0;
		var _g11 = this.m_type.enumValue;
		while(_g1 < _g11.length) {
			var entry = _g11[_g1];
			++_g1;
			var option;
			var _this = window.document;
			option = _this.createElement("option");
			option.text = entry.key;
			option.value = entry.value;
			this.m_comboInput.appendChild(option);
			{
				this.m_enumValueOptions.set(entry.key,option);
				option;
			}
		}
		this.m_isBitMaskEditor = Reflect.field(this.m_field.meta,"BitMaskEditor") != null;
		if(this.m_isBitMaskEditor) {
			this.m_comboInput.multiple = true;
			this.m_comboInput.size = Std["int"](Math.min(10,this.m_type.enumValue.length));
		}
		this.m_comboInput.addEventListener("change",function(e) {
			_g.m_instance = _g.getEnumBitMaskValue();
			_g.notifyChanged(_g.m_field,_g.m_instance);
		});
		this.inspector.container.appendChild(this.m_comboInput);
	}
	,initDefaultStruct: function() {
		this.inspector.classList.add("default-struct");
		this.m_structFieldHandlers = new haxe_ds_StringMap();
		var componentOwner = this.m_owner;
		var database = ursine_editor_Editor.instance.componentDatabase;
		var _g = 0;
		var _g1 = this.m_type.fields;
		while(_g < _g1.length) {
			var field = _g1[_g];
			++_g;
			var fieldInstance = Reflect.field(this.m_instance,field.name);
			var fieldType = database.getNativeType(field.type);
			var fieldInspector = database.createFieldInspector(this,fieldInstance,field,fieldType);
			this.inspector.container.appendChild(fieldInspector.inspector);
			{
				this.m_structFieldHandlers.set(field.name,fieldInspector);
				fieldInspector;
			}
		}
	}
	,loadEnumBitMaskValue: function(value) {
		var _g = 0;
		var _g1 = this.m_type.enumValue;
		while(_g < _g1.length) {
			var entry = _g1[_g];
			++_g;
			var option = this.m_enumValueOptions.get(entry.key);
			option.selected = (value & entry.value) == entry.value;
		}
	}
	,getEnumBitMaskValue: function() {
		var value = 0;
		var values = this.m_comboInput.selectedOptions;
		var _g = 0;
		while(_g < values.length) {
			var option = values[_g];
			++_g;
			if(option.selected) value |= Std.parseInt(option.value);
		}
		return value;
	}
	,__class__: ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector
});
var ursine_editor_scene_component_inspectors_fields_EntitySystemSelectorInspector = function(owner,instance,field,type) {
	var _g = this;
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	this.m_entitySystems = ursine_native_Extern.SceneGetEntitySystems();
	var _this = window.document;
	this.m_displayText = _this.createElement("div");
	this.m_displayText.classList.add("entity-system-selector");
	this.m_displayText.addEventListener("click",function(e) {
		var selector = new ItemSelectionPopupControl(_g.m_entitySystems);
		selector.addEventListener("item-selected",$bind(_g,_g.onEntitySystemSelected));
		window.document.body.appendChild(selector);
		selector.show(e.clientX,e.clientY);
	});
	this.inspector.container.appendChild(this.m_displayText);
	this.updateValue(instance);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.EntitySystemSelectorInspector"] = ursine_editor_scene_component_inspectors_fields_EntitySystemSelectorInspector;
ursine_editor_scene_component_inspectors_fields_EntitySystemSelectorInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","EntitySystemSelectorInspector"];
ursine_editor_scene_component_inspectors_fields_EntitySystemSelectorInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_EntitySystemSelectorInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	updateValue: function(value) {
		this.m_displayText.innerText = value;
		this.m_displayText.classList.toggle("empty",value.length == 0);
		this.m_instance = value;
	}
	,onEntitySystemSelected: function(e) {
		this.notifyChanged(this.m_field,e.detail.item);
	}
	,__class__: ursine_editor_scene_component_inspectors_fields_EntitySystemSelectorInspector
});
var ursine_editor_scene_component_inspectors_fields_NumberFieldInspector = function(owner,instance,field,type) {
	var _g = this;
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	this.m_number = new NumberInputControl();
	if(Object.prototype.hasOwnProperty.call(field.meta,ursine_native_Property.InputRange)) {
		this.m_number.slider = true;
		var range = Reflect.field(field.meta,ursine_native_Property.InputRange);
		this.m_number.format = range.format;
		this.m_number.min = range.min;
		this.m_number.max = range.max;
		if(range.step > 0.0) if(range.step == null) this.m_number.step = "null"; else this.m_number.step = "" + range.step;
	}
	if(type.name.indexOf("unsigned") != -1) this.m_number.min = "0";
	var changeHandler = function() {
		var value = _g.m_number.valueAsNumber;
		if((function($this) {
			var $r;
			var f = value;
			$r = isNaN(f);
			return $r;
		}(this))) value = 0;
		if(!(_g.m_type.name == "float" || _g.m_type.name == "double")) value = Std["int"](value);
		_g.notifyChanged(_g.m_field,value);
	};
	this.m_number.addEventListener("change",changeHandler);
	if(this.m_number.type == "range") this.m_number.addEventListener("input",changeHandler);
	this.m_number.addEventListener("focus",function(e) {
		_g.m_number.select();
		e.preventDefault();
	});
	this.updateValue(instance);
	this.inspector.container.appendChild(this.m_number);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.NumberFieldInspector"] = ursine_editor_scene_component_inspectors_fields_NumberFieldInspector;
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","NumberFieldInspector"];
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	updateValue: function(value) {
		var number;
		if(this.m_type.name == "float" || this.m_type.name == "double") number = Math.toMaxPrecision(value,5); else number = Std["int"](value);
		this.m_number.value = number;
	}
	,__class__: ursine_editor_scene_component_inspectors_fields_NumberFieldInspector
});
var ursine_editor_scene_component_inspectors_fields_ResourceReferenceInspector = function(owner,instance,field,type) {
	var _g = this;
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	var resourceType = Reflect.field(field.meta,ursine_native_Property.ResourceType);
	if(resourceType == null) {
		var error = new NotificationControl(2,"Field <strong class=\"highlight\">" + field.name + "</strong> missing meta property <strong class=\"highlight\">ResourceType<strong>","Error");
		error.show();
	} else this.m_resourceType = resourceType.typeName;
	var _this = window.document;
	this.m_displayText = _this.createElement("div");
	this.m_displayText.classList.add("resource-reference");
	this.m_displayText.addEventListener("click",function(e) {
		var resources = ursine_native_Extern.ProjectGetResourcesByType(_g.m_resourceType);
		var selector = new ResourceReferenceSelectionPopupControl(resources);
		selector.addEventListener("resource-selected",$bind(_g,_g.onResourceSelected));
		window.document.body.appendChild(selector);
		selector.show(e.clientX,e.clientY);
	});
	this.inspector.container.appendChild(this.m_displayText);
	this.updateValue(instance);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.ResourceReferenceInspector"] = ursine_editor_scene_component_inspectors_fields_ResourceReferenceInspector;
ursine_editor_scene_component_inspectors_fields_ResourceReferenceInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","ResourceReferenceInspector"];
ursine_editor_scene_component_inspectors_fields_ResourceReferenceInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_ResourceReferenceInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	updateValue: function(value) {
		var resource = ursine_native_Extern.ProjectGetResource(value.guid);
		var valid = resource != null;
		if(valid) this.m_displayText.innerText = resource.displayName; else this.m_displayText.innerText = "";
		this.m_displayText.classList.toggle("invalid",!valid);
		this.m_instance = value;
	}
	,onResourceSelected: function(e) {
		this.notifyChanged(this.m_field,{ guid : e.detail.resource.guid});
	}
	,__class__: ursine_editor_scene_component_inspectors_fields_ResourceReferenceInspector
});
var ursine_editor_scene_component_inspectors_fields_StringFieldInspector = function(owner,instance,field,type) {
	var _g = this;
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	this.m_string = new TextInputControl();
	if(Object.prototype.hasOwnProperty.call(field.meta,ursine_native_Property.MultiLineEditor)) {
	}
	this.m_string.addEventListener("change",function() {
		_g.notifyChanged(_g.m_field,_g.m_string.value);
	});
	this.m_string.addEventListener("focus",function(e) {
		haxe_Timer.delay(function() {
			_g.m_string.select();
		},50);
		e.preventDefault();
	});
	this.inspector.container.appendChild(this.m_string);
	this.updateValue(instance);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.StringFieldInspector"] = ursine_editor_scene_component_inspectors_fields_StringFieldInspector;
ursine_editor_scene_component_inspectors_fields_StringFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","StringFieldInspector"];
ursine_editor_scene_component_inspectors_fields_StringFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_StringFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	updateValue: function(value) {
		this.m_string.value = value;
		this.m_instance = value;
	}
	,__class__: ursine_editor_scene_component_inspectors_fields_StringFieldInspector
});
var ursine_editor_scene_component_inspectors_fields_UnknownTypeInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	var message;
	var _this = window.document;
	message = _this.createElement("div");
	message.classList.add("invalid-type");
	message.innerHTML = "Unknown field type. Is this type exposed in reflection?";
	this.inspector.container.appendChild(message);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.UnknownTypeInspector"] = ursine_editor_scene_component_inspectors_fields_UnknownTypeInspector;
ursine_editor_scene_component_inspectors_fields_UnknownTypeInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","UnknownTypeInspector"];
ursine_editor_scene_component_inspectors_fields_UnknownTypeInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_UnknownTypeInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	__class__: ursine_editor_scene_component_inspectors_fields_UnknownTypeInspector
});
var ursine_editor_scene_component_inspectors_fields_VectorFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	var _this = window.document;
	this.m_fieldsContainer = _this.createElement("div");
	this.m_fieldsContainer.classList.add("vector-fields");
	this.inspector.container.appendChild(this.m_fieldsContainer);
	this.m_fields = new haxe_ds_StringMap();
	var _g = 0;
	var _g1 = type.fields;
	while(_g < _g1.length) {
		var field1 = _g1[_g];
		++_g;
		this.createVectorField(field1);
	}
	this.updateValue(instance);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.VectorFieldInspector"] = ursine_editor_scene_component_inspectors_fields_VectorFieldInspector;
ursine_editor_scene_component_inspectors_fields_VectorFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","VectorFieldInspector"];
ursine_editor_scene_component_inspectors_fields_VectorFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_VectorFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	updateValue: function(value) {
		var $it0 = this.m_fields.keys();
		while( $it0.hasNext() ) {
			var field = $it0.next();
			this.updateVectorField(field,Reflect.field(value,field));
		}
	}
	,createVectorField: function(field) {
		var _g = this;
		var number = new NumberInputControl();
		number.step = "0.1";
		number.addEventListener("change",function() {
			var value = number.valueAsNumber;
			if(isNaN(value)) value = 0;
			_g.m_instance[field.name] = value;
			_g.notifyChanged(_g.m_field,_g.m_instance);
		});
		number.addEventListener("focus",function(e) {
			haxe_Timer.delay(function() {
				number.select();
			},50);
			e.preventDefault();
		});
		{
			this.m_fields.set(field.name,number);
			number;
		}
		this.m_fieldsContainer.appendChild(number);
	}
	,updateVectorField: function(name,value) {
		var field = this.m_fields.get(name);
		field.value = Math.toMaxPrecision(value,5);
	}
	,__class__: ursine_editor_scene_component_inspectors_fields_VectorFieldInspector
});
var ursine_utils_IEventContainer = function() { };
$hxClasses["ursine.utils.IEventContainer"] = ursine_utils_IEventContainer;
ursine_utils_IEventContainer.__name__ = ["ursine","utils","IEventContainer"];
var ursine_editor_scene_entity_Entity = function(uniqueID) {
	this.events = new ursine_utils_EventManager();
	this.uniqueID = uniqueID;
	this.m_handler = new EntityHandler(uniqueID);
	ursine_editor_Editor.instance.broadcastManager.getChannel("EntityManager").on(ursine_editor_scene_entity_EntityEvent.ComponentAdded,$bind(this,this.onComponentAdded)).on(ursine_editor_scene_entity_EntityEvent.ComponentRemoved,$bind(this,this.onComponentRemoved)).on(ursine_editor_scene_entity_EntityEvent.ComponentChanged,$bind(this,this.onComponentChanged)).on(ursine_editor_scene_entity_EntityEvent.ComponentArrayInserted,$bind(this,this.onComponentArrayInserted)).on(ursine_editor_scene_entity_EntityEvent.ComponentArraySet,$bind(this,this.onComponentArraySet)).on(ursine_editor_scene_entity_EntityEvent.ComponentArrayRemove,$bind(this,this.onComponentArrayRemove));
};
$hxClasses["ursine.editor.scene.entity.Entity"] = ursine_editor_scene_entity_Entity;
ursine_editor_scene_entity_Entity.__name__ = ["ursine","editor","scene","entity","Entity"];
ursine_editor_scene_entity_Entity.__interfaces__ = [ursine_utils_IEventContainer];
ursine_editor_scene_entity_Entity.create = function() {
	return new ursine_editor_scene_entity_Entity(ursine_native_Extern.CreateEntity());
};
ursine_editor_scene_entity_Entity.createCopyName = function(name) {
	var copyMatch = new EReg("\\(([0-9]+)\\)$","i");
	if(copyMatch.match(name)) {
		var index = Std.parseInt(copyMatch.matched(1));
		name = "" + copyMatch.matchedLeft() + " (" + (index + 1) + ")";
	} else name += " (1)";
	return name;
};
ursine_editor_scene_entity_Entity.prototype = {
	isValid: function() {
		return this.m_handler.isValid();
	}
	,isRemovalEnabled: function() {
		return this.m_handler.isRemovalEnabled();
	}
	,isHierarchyChangeEnabled: function() {
		return this.m_handler.isHierarchyChangeEnabled();
	}
	,isVisibleInEditor: function() {
		return this.m_handler.isVisibleInEditor();
	}
	,remove: function() {
		this.m_handler.remove();
	}
	,getName: function() {
		return this.m_handler.getName();
	}
	,setName: function(name) {
		this.m_handler.setName(name);
	}
	,select: function() {
		if(!this.m_handler.hasComponent("Selected")) {
			this.m_handler.addComponent("Selected");
			return true;
		}
		return false;
	}
	,deselect: function() {
		if(this.m_handler.hasComponent("Selected")) this.m_handler.removeComponent("Selected");
	}
	,inspect: function() {
		return this.m_handler.inspect();
	}
	,inspectComponent: function(component) {
		return this.m_handler.inspectComponent(component);
	}
	,hasComponent: function(name) {
		return this.m_handler.hasComponent(name);
	}
	,addComponent: function(name) {
		this.m_handler.addComponent(name);
	}
	,removeComponent: function(name) {
		this.m_handler.removeComponent(name);
	}
	,componentSet: function(componentName,value) {
		this.m_handler.componentSet(componentName,value);
	}
	,componentFieldUpdate: function(componentName,fieldName,value) {
		this.m_handler.componentFieldUpdate(componentName,fieldName,value);
	}
	,componentFieldArrayUpdate: function(componentName,fieldName,index,value) {
		this.m_handler.componentFieldArrayUpdate(componentName,fieldName,index,value);
	}
	,componentFieldArrayInsert: function(componentName,fieldName,index,value) {
		this.m_handler.componentFieldArrayInsert(componentName,fieldName,index,value);
	}
	,componentFieldArrayPush: function(componentName,fieldName,value) {
		this.m_handler.componentFieldArrayInsert(componentName,fieldName,this.m_handler.componentFieldArrayGetLength(componentName,fieldName),value);
	}
	,componentFieldArrayRemove: function(componentName,fieldName,index) {
		this.m_handler.componentFieldArrayRemove(componentName,fieldName,index);
	}
	,componentFieldArraySwap: function(componentName,fieldName,index1,index2) {
		this.m_handler.componentFieldArraySwap(componentName,fieldName,index1,index2);
	}
	,componentButtonInvoke: function(componentName,buttonName) {
		this.m_handler.componentButtonInvoke(componentName,buttonName);
	}
	,getChildren: function() {
		var children = this.m_handler.getChildren();
		if(children == false) return [];
		return children.map(function(uid) {
			return new ursine_editor_scene_entity_Entity(uid);
		});
	}
	,getParent: function() {
		var parentUniqueID = this.m_handler.getParent();
		if(parentUniqueID == null) return null; else return new ursine_editor_scene_entity_Entity(parentUniqueID);
	}
	,setParent: function(parent) {
		return this.m_handler.setParent(parent == null?null:parent.uniqueID);
	}
	,getSiblingIndex: function() {
		return this.m_handler.getSiblingIndex();
	}
	,setSiblingIndex: function(index) {
		this.m_handler.setSiblingIndex(index);
	}
	,saveAsArchetype: function() {
		this.m_handler.saveAsArchetype();
	}
	,clone: function() {
		return new ursine_editor_scene_entity_Entity(this.m_handler.clone());
	}
	,onComponentAdded: function(e) {
		if(e.uniqueID == this.uniqueID) this.events.trigger(ursine_editor_scene_entity_EntityEvent.ComponentAdded,e);
	}
	,onComponentRemoved: function(e) {
		if(e.uniqueID == this.uniqueID) this.events.trigger(ursine_editor_scene_entity_EntityEvent.ComponentRemoved,e);
	}
	,onComponentChanged: function(e) {
		if(e.uniqueID == this.uniqueID) this.events.trigger(ursine_editor_scene_entity_EntityEvent.ComponentChanged,e);
	}
	,onComponentArrayInserted: function(e) {
		if(e.uniqueID == this.uniqueID) this.events.trigger(ursine_editor_scene_entity_EntityEvent.ComponentArrayInserted,e);
	}
	,onComponentArraySet: function(e) {
		if(e.uniqueID == this.uniqueID) this.events.trigger(ursine_editor_scene_entity_EntityEvent.ComponentArraySet,e);
	}
	,onComponentArrayRemove: function(e) {
		if(e.uniqueID == this.uniqueID) this.events.trigger(ursine_editor_scene_entity_EntityEvent.ComponentArrayRemove,e);
	}
	,__class__: ursine_editor_scene_entity_Entity
};
var ursine_editor_scene_entity_EntityEvent = function() { };
$hxClasses["ursine.editor.scene.entity.EntityEvent"] = ursine_editor_scene_entity_EntityEvent;
ursine_editor_scene_entity_EntityEvent.__name__ = ["ursine","editor","scene","entity","EntityEvent"];
var ursine_editor_windows_EntityInspector = function() {
	this.m_inspectedEntity = null;
	ursine_editor_windows_EntityInspector.instance = this;
	ursine_editor_WindowHandler.call(this);
	this.m_componentHandlers = new haxe_ds_StringMap();
	this.m_openCache = new haxe_ds_StringMap();
	this.m_componentClipboard = new haxe_ds_StringMap();
	this.window.heading = "Inspector";
	this.initWindow();
};
$hxClasses["ursine.editor.windows.EntityInspector"] = ursine_editor_windows_EntityInspector;
ursine_editor_windows_EntityInspector.__name__ = ["ursine","editor","windows","EntityInspector"];
ursine_editor_windows_EntityInspector.__super__ = ursine_editor_WindowHandler;
ursine_editor_windows_EntityInspector.prototype = $extend(ursine_editor_WindowHandler.prototype,{
	inspect: function(entity) {
		this.clearOldInspection();
		this.m_inspectedEntity = entity;
		this.initializeInspection();
	}
	,onInspectedEntityComponentAdded: function(e) {
		var inspection = { type : e.component, value : e.value, buttons : e.buttons};
		this.inspectComponent(inspection);
	}
	,onInspectedEntityComponentRemoved: function(e) {
		var handler = this.m_componentHandlers.get(e.component);
		if(handler != null) this.removeInspector(handler);
		this.m_componentHandlers.remove(e.component);
	}
	,onInspectedEntityComponentChanged: function(e) {
		var handler = this.m_componentHandlers.get(e.component);
		if(handler != null) handler.updateField(e.field,e.value);
	}
	,onInspectedEntityComponentArrayInserted: function(e) {
		var handler = this.m_componentHandlers.get(e.component);
		if(handler != null) handler.arrayInsert(e.field,e.index,e.value);
	}
	,onInspectedEntityComponentArrayItemSet: function(e) {
		var handler = this.m_componentHandlers.get(e.component);
		if(handler != null) handler.arraySet(e.field,e.index,e.value);
	}
	,onInspectedEntityComponentArrayItemRemoved: function(e) {
		var handler = this.m_componentHandlers.get(e.component);
		if(handler != null) handler.arrayRemove(e.field,e.index);
	}
	,clearOldInspection: function() {
		if(this.m_inspectedEntity != null) this.m_inspectedEntity.events.off(ursine_editor_scene_entity_EntityEvent.ComponentAdded,$bind(this,this.onInspectedEntityComponentAdded)).off(ursine_editor_scene_entity_EntityEvent.ComponentRemoved,$bind(this,this.onInspectedEntityComponentRemoved)).off(ursine_editor_scene_entity_EntityEvent.ComponentChanged,$bind(this,this.onInspectedEntityComponentChanged)).off(ursine_editor_scene_entity_EntityEvent.ComponentArrayInserted,$bind(this,this.onInspectedEntityComponentArrayInserted)).off(ursine_editor_scene_entity_EntityEvent.ComponentArraySet,$bind(this,this.onInspectedEntityComponentArrayItemSet)).off(ursine_editor_scene_entity_EntityEvent.ComponentArrayRemove,$bind(this,this.onInspectedEntityComponentArrayItemRemoved));
		var $it0 = this.m_componentHandlers.iterator();
		while( $it0.hasNext() ) {
			var handler = $it0.next();
			this.removeInspector(handler);
		}
		this.m_componentHandlers = new haxe_ds_StringMap();
	}
	,initializeInspection: function() {
		if(this.m_inspectedEntity == null) {
			this.m_headerToolbar.style.display = "none";
			this.m_btnAddComponent.style.display = "none";
			return;
		}
		this.m_headerToolbar.style.display = "block";
		this.m_btnAddComponent.style.display = "block";
		this.m_inspectedEntity.events.on(ursine_editor_scene_entity_EntityEvent.ComponentAdded,$bind(this,this.onInspectedEntityComponentAdded)).on(ursine_editor_scene_entity_EntityEvent.ComponentRemoved,$bind(this,this.onInspectedEntityComponentRemoved)).on(ursine_editor_scene_entity_EntityEvent.ComponentChanged,$bind(this,this.onInspectedEntityComponentChanged)).on(ursine_editor_scene_entity_EntityEvent.ComponentArrayInserted,$bind(this,this.onInspectedEntityComponentArrayInserted)).on(ursine_editor_scene_entity_EntityEvent.ComponentArraySet,$bind(this,this.onInspectedEntityComponentArrayItemSet)).on(ursine_editor_scene_entity_EntityEvent.ComponentArrayRemove,$bind(this,this.onInspectedEntityComponentArrayItemRemoved));
		var inspection = this.m_inspectedEntity.inspect();
		var _g = 0;
		while(_g < inspection.length) {
			var component = inspection[_g];
			++_g;
			this.inspectComponent(component);
		}
	}
	,createComponentInspector: function(component) {
		var _g = this;
		var database = ursine_editor_Editor.instance.componentDatabase;
		var type = database.getComponentType(component.type);
		var handler = database.createComponentInspector(this.m_inspectedEntity,component);
		handler.inspector.header.addEventListener("contextmenu",function(e) {
			_g.createComponentContextMenu(handler,e);
			e.preventDefault();
			e.stopPropagation();
			e.stopImmediatePropagation();
			return false;
		});
		handler.inspector.canRemove = !Object.prototype.hasOwnProperty.call(type.meta,ursine_native_Property.DisableComponentRemoval);
		handler.inspector.opened = this.m_openCache.get(component.type) == true;
		handler.inspector.addEventListener("removed",(function(f,a1) {
			return function(e1) {
				f(a1,e1);
			};
		})($bind(this,this.onRemoveComponentClicked),component));
		handler.inspector.addEventListener("open-changed",(function(f1,a11) {
			return function(e2) {
				f1(a11,e2);
			};
		})($bind(this,this.onOpenChanged),component));
		return handler;
	}
	,inspectComponent: function(component) {
		if(this.m_componentHandlers.get(component.type) != null) return;
		var database = ursine_editor_Editor.instance.componentDatabase;
		var type = database.getComponentType(component.type);
		if(Object.prototype.hasOwnProperty.call(type.meta,ursine_native_Property.HiddenInInspector)) return;
		var handler = this.createComponentInspector(component);
		{
			this.m_componentHandlers.set(component.type,handler);
			handler;
		}
		this.m_inspectorsContainer.appendChild(handler.inspector);
	}
	,removeInspector: function(inspector) {
		inspector.remove();
		{
			this.m_componentHandlers.set(inspector.component.type,null);
			null;
		}
	}
	,getAvailableComponentTypes: function(entity) {
		var db = ursine_editor_Editor.instance.componentDatabase;
		return db.getComponentTypes().filter(function(type) {
			var componentType = db.getComponentType(type);
			var isHidden = Object.prototype.hasOwnProperty.call(componentType.meta,ursine_native_Property.HiddenInInspector) || Object.prototype.hasOwnProperty.call(componentType.meta,ursine_native_Property.HiddenInSelector);
			return !entity.hasComponent(type) && !isHidden;
		});
	}
	,createComponentContextMenu: function(inspector,e) {
		var _g = this;
		var menu = new ContextMenuControl();
		menu.addItem("Copy",function() {
			var v = inspector.copyInstance();
			_g.m_componentClipboard.set(inspector.component.type,v);
			v;
		}).icon = "copy";
		var clipboard = this.m_componentClipboard.get(inspector.component.type);
		var paste = menu.addItem("Paste",function() {
			inspector.entity.componentSet(inspector.component.type,clipboard.value);
		});
		paste.icon = "paste";
		paste.disabled = clipboard == null;
		menu.open(e.clientX,e.clientY);
	}
	,onArchetypeSaveClicked: function(e) {
		this.m_inspectedEntity.saveAsArchetype();
	}
	,onCopyEntityClicked: function(e) {
		ursine_editor_windows_SceneOutline.instance.clearSelectedEntities();
		var entity = this.m_inspectedEntity.clone();
		entity.setName(ursine_editor_scene_entity_Entity.createCopyName(this.m_inspectedEntity.getName()));
		entity.select();
	}
	,onAddComponentClicked: function(e) {
		var types = this.getAvailableComponentTypes(this.m_inspectedEntity);
		var selector = new ItemSelectionPopupControl(types);
		selector.addEventListener("item-selected",$bind(this,this.onAddComponentTypeSelected));
		window.document.body.appendChild(selector);
		selector.show(e.clientX,e.clientY);
	}
	,onRemoveComponentClicked: function(component,e) {
		this.m_inspectedEntity.removeComponent(component.type);
		e.stopPropagation();
	}
	,onAddComponentTypeSelected: function(e) {
		var componentType = e.detail.item;
		if(this.m_inspectedEntity.hasComponent(componentType)) {
			var notification = new NotificationControl(2,"Entity already has component type <strong class=\"highlight\">" + componentType + "</strong>","Error");
			notification.show();
			return;
		}
		this.m_inspectedEntity.addComponent(componentType);
	}
	,onOpenChanged: function(component,e) {
		var v = e.detail.open;
		this.m_openCache.set(component.type,v);
		v;
	}
	,onWindowResized: function() {
		this.window.classList.toggle("small-width",this.window.container.offsetWidth < ursine_editor_windows_EntityInspector.m_smallWindowWidth);
	}
	,initWindow: function() {
		this.window.classList.add("entity-inspector-window");
		this.window.addEventListener("resize",$bind(this,this.onWindowResized));
		window.addEventListener("resize",$bind(this,this.onWindowResized));
		var _this = window.document;
		this.m_headerToolbar = _this.createElement("div");
		this.m_headerToolbar.classList.add("header-toolbar");
		this.window.container.appendChild(this.m_headerToolbar);
		this.m_btnSaveArchetype = new ButtonControl();
		this.m_btnSaveArchetype.classList.add("save-archetype");
		this.m_btnSaveArchetype.addEventListener("click",$bind(this,this.onArchetypeSaveClicked));
		this.m_headerToolbar.appendChild(this.m_btnSaveArchetype);
		this.m_btnCopyEntity = new ButtonControl();
		this.m_btnCopyEntity.classList.add("copy-entity");
		this.m_btnCopyEntity.addEventListener("click",$bind(this,this.onCopyEntityClicked));
		this.m_headerToolbar.appendChild(this.m_btnCopyEntity);
		this.m_headerToolbar.style.display = "none";
		var _this1 = window.document;
		this.m_inspectorsContainer = _this1.createElement("div");
		this.window.container.appendChild(this.m_inspectorsContainer);
		this.m_btnAddComponent = new ButtonControl();
		this.m_btnAddComponent.text = "Add Component";
		this.m_btnAddComponent.classList.add("add-component");
		this.m_btnAddComponent.style.display = "none";
		this.m_btnAddComponent.addEventListener("click",$bind(this,this.onAddComponentClicked));
		this.window.container.appendChild(this.m_btnAddComponent);
	}
	,__class__: ursine_editor_windows_EntityInspector
});
var ursine_editor_windows_ProjectBrowser = function() {
	ursine_editor_WindowHandler.call(this);
	this.window.heading = "Project";
	this.window.classList.add("project-browser-window");
	this.m_browser = new ProjectBrowserControl(ursine_native_Extern.ProjectGetResourceTree());
	this.m_browser.addEventListener("resource-dblclick",$bind(this,this.onResourceDblClick));
	this.m_browser.addEventListener("resource-contextmenu",$bind(this,this.onResourceContextMenu));
	this.window.container.appendChild(this.m_browser);
	ursine_editor_Editor.instance.broadcastManager.getChannel("ResourcePipeline").on("ResourceAdded",$bind(this,this.onResourceAdded)).on("ResourceModified",$bind(this,this.onResourceModified));
};
$hxClasses["ursine.editor.windows.ProjectBrowser"] = ursine_editor_windows_ProjectBrowser;
ursine_editor_windows_ProjectBrowser.__name__ = ["ursine","editor","windows","ProjectBrowser"];
ursine_editor_windows_ProjectBrowser.__super__ = ursine_editor_WindowHandler;
ursine_editor_windows_ProjectBrowser.prototype = $extend(ursine_editor_WindowHandler.prototype,{
	onResourceAdded: function(data) {
		var resource = ursine_native_Extern.ProjectGetResource(data.guid);
		if(resource == null) throw new js__$Boot_HaxeError("Invalid resource added.");
		this.m_browser.addResource(resource);
		var notification = new NotificationControl(3,"<strong class=\"highlight\">" + resource.relativePathDisplayName + "</div>","Resource Imported");
		notification.show();
	}
	,onResourceModified: function(data) {
	}
	,onResourceDblClick: function(e) {
		var extension = e.detail.resource.extension.substr(1).toLowerCase();
		var handler = Reflect.field(this,"" + extension + "_DblClickHandler");
		if(handler != null) handler(e);
	}
	,onResourceContextMenu: function(e) {
		var resource = e.detail.resource;
		var extension = HxOverrides.substr(resource.extension,1,null).toLowerCase();
		var handler = Reflect.field(this,"" + extension + "_ContextMenuHandler");
		if(handler != null) {
			e.detail.menu.addSeparator();
			handler(e);
		}
	}
	,uworld_DblClickHandler: function(e) {
		var resource = e.detail.resource;
		ursine_native_Extern.SceneSetActiveWorld(resource.guid);
	}
	,__class__: ursine_editor_windows_ProjectBrowser
});
var ursine_editor_windows_SceneOutline = function() {
	this.m_selectedEntities = null;
	ursine_editor_windows_SceneOutline.instance = this;
	ursine_editor_WindowHandler.call(this);
	this.window.heading = "Outline";
	this.window.classList.add("scene-outline-window");
	this.m_rootView = new TreeViewControl();
	this.m_rootView.setAsRoot(true);
	this.m_rootView.enableModification = true;
	this.m_entityItems = new haxe_ds_IntMap();
	this.m_selectedEntities = [];
	this.window.container.appendChild(this.m_rootView);
	this.resetScene();
	ursine_editor_Editor.instance.broadcastManager.getChannel("SceneManager").on("WorldChanged",$bind(this,this.resetScene));
	ursine_editor_Editor.instance.broadcastManager.getChannel("EntityManager").on(ursine_editor_scene_entity_EntityEvent.EntityAdded,$bind(this,this.onEntityAdded)).on(ursine_editor_scene_entity_EntityEvent.EntityRemoved,$bind(this,this.onEntityRemoved)).on(ursine_editor_scene_entity_EntityEvent.EntityNameChanged,$bind(this,this.onEntityNameChanged)).on(ursine_editor_scene_entity_EntityEvent.EntityParentChanged,$bind(this,this.onEntityParentChanged)).on(ursine_editor_scene_entity_EntityEvent.ComponentAdded,$bind(this,this.onComponentAdded)).on(ursine_editor_scene_entity_EntityEvent.ComponentRemoved,$bind(this,this.onComponentRemoved));
	this.window.addEventListener("keydown",$bind(this,this.onWindowKeyDown));
};
$hxClasses["ursine.editor.windows.SceneOutline"] = ursine_editor_windows_SceneOutline;
ursine_editor_windows_SceneOutline.__name__ = ["ursine","editor","windows","SceneOutline"];
ursine_editor_windows_SceneOutline.__super__ = ursine_editor_WindowHandler;
ursine_editor_windows_SceneOutline.prototype = $extend(ursine_editor_WindowHandler.prototype,{
	clearSelectedEntities: function() {
		var _g = 0;
		var _g1 = this.m_selectedEntities;
		while(_g < _g1.length) {
			var uid = _g1[_g];
			++_g;
			var item = this.m_entityItems.h[uid];
			if(item == null) continue;
			item.entity.deselect();
		}
	}
	,deleteSelectedEntities: function() {
		var _g = 0;
		var _g1 = this.m_selectedEntities;
		while(_g < _g1.length) {
			var uid = _g1[_g];
			++_g;
			var item = this.m_entityItems.h[uid];
			if(item == null) continue;
			var entity = item.entity;
			if(entity.isRemovalEnabled()) entity.remove(); else {
			}
		}
	}
	,resetScene: function() {
		this.m_selectedEntities = [];
		this.m_rootView.innerHTML = "";
		this.m_entityItems = new haxe_ds_IntMap();
		ursine_editor_windows_EntityInspector.instance.inspect(null);
		var entities = ursine_native_Extern.SceneGetRootEntities();
		var event_uniqueID = 0;
		var _g = 0;
		while(_g < entities.length) {
			var uniqueID = entities[_g];
			++_g;
			var entity = new ursine_editor_scene_entity_Entity(uniqueID);
			this.initEntity(entity);
		}
	}
	,initEntity: function(entity) {
		this.addEntity(entity);
		var _g = 0;
		var _g1 = entity.getChildren();
		while(_g < _g1.length) {
			var child = _g1[_g];
			++_g;
			this.initEntity(child);
		}
	}
	,onWindowKeyDown: function(e) {
		var _g = e.keyCode;
		switch(_g) {
		case 46:
			this.deleteSelectedEntities();
			break;
		}
	}
	,onEntityAdded: function(e) {
		var _g = this;
		var entity = new ursine_editor_scene_entity_Entity(e.uniqueID);
		haxe_Timer.delay(function() {
			_g.addEntity(entity);
		},0);
	}
	,onEntityRemoved: function(e) {
		var item = this.m_entityItems.h[e.uniqueID];
		if(item == null) return;
		if(this.m_selectedEntities.indexOf(e.uniqueID) != -1) this.selectEntity(null);
		if(item.parentNode != null) item.parentNode.removeChild(item);
		this.m_entityItems.remove(e.uniqueID);
	}
	,onEntityNameChanged: function(e) {
		var item = this.m_entityItems.h[e.uniqueID];
		if(item == null) return;
		item.text = e.name;
		ToolTip.bind(item.textContentElement,e.name);
	}
	,onEntityParentChanged: function(e) {
		var item = this.m_entityItems.h[e.uniqueID];
		if(item == null) return;
		var entity = item.entity;
		if(item.parentNode != null) item.parentNode.removeChild(item);
		var targetContainer = null;
		if(e.newParent == null) targetContainer = this.m_rootView; else {
			var newItem = this.m_entityItems.h[e.newParent];
			if(newItem != null) targetContainer = newItem.child;
		}
		if(targetContainer != null) {
			var children = targetContainer.children;
			if(children.length == 0) targetContainer.appendChild(item); else {
				var index = Math.clamp(entity.getSiblingIndex(),0,children.length - 1);
				targetContainer.insertBefore(item,children[index]);
			}
		}
	}
	,onComponentAdded: function(e) {
		if(e.component == "Selected") {
			var item = this.m_entityItems.h[e.uniqueID];
			if(item != null) this.selectEntity(item);
		}
	}
	,onComponentRemoved: function(e) {
		if(e.component == "Selected") {
			var item = this.m_entityItems.h[e.uniqueID];
			if(item != null) this.deselectEntity(item);
		}
	}
	,addEntity: function(entity) {
		if(this.m_entityItems.h[entity.uniqueID] != null) return;
		var item = this.createEntityItem(entity);
		if(!entity.isVisibleInEditor()) item.classList.add("hidden");
		var parent = entity.getParent();
		if(parent == null) this.m_rootView.appendChild(item); else {
			var parentItem = this.m_entityItems.h[parent.uniqueID];
			if(parentItem != null) parentItem.child.appendChild(item);
		}
		if(entity.hasComponent("Selected")) this.selectEntity(item);
	}
	,createEntityItem: function(entity) {
		var _g = this;
		var item = new TreeViewItemControl();
		item.addEventListener("contextmenu",function(e) {
			_g.openContextMenu(e,item);
			e.preventDefault();
			e.stopPropagation();
			e.stopImmediatePropagation();
			return false;
		});
		item.addEventListener("drag-start",function(e1) {
			e1.stopPropagation();
			e1.stopImmediatePropagation();
			e1.preventStart = !entity.isHierarchyChangeEnabled();
		});
		item.addEventListener("drag-drop",function(e2) {
			e2.stopPropagation();
			e2.stopImmediatePropagation();
			if(!entity.isHierarchyChangeEnabled()) {
				e2.preventDrop = true;
				return;
			}
			var target = e2.detail.dropTarget.entity;
			var parent = entity.getParent();
			var targetID;
			if(target == null) targetID = -1; else targetID = target.uniqueID;
			var parentID;
			if(parent == null) parentID = -1; else parentID = parent.uniqueID;
			if(targetID == parentID) {
				e2.preventDrop = e2.detail.newParent;
				return;
			}
			entity.setParent(target);
		});
		item.addEventListener("drag-drop-after",function(e3) {
			e3.stopPropagation();
			e3.stopImmediatePropagation();
			var childIndex = ElementUtils.childIndex(item);
			entity.setSiblingIndex(childIndex);
		});
		ToolTip.bind(item.textContentElement,entity.getName());
		item.textContentElement.addEventListener("dblclick",function(e4) {
			_g.startRenamingEntity(item);
		});
		item.textContentElement.addEventListener("keydown",function(e5) {
			if(e5.keyCode == 13) {
				item.textContentElement.blur();
				e5.preventDefault();
				return false;
			}
			return true;
		});
		item.textContentElement.addEventListener("blur",function() {
			item.textContentElement.contentEditable = "false";
			entity.setName(item.textContentElement.innerText);
		});
		item.text = entity.getName();
		item.entity = entity;
		item.textElement.addEventListener("click",function(e6) {
			if(_g.m_selectedEntities.indexOf(item.entity.uniqueID) != -1) return;
			_g.clearSelectedEntities();
			item.entity.select();
		});
		{
			this.m_entityItems.h[entity.uniqueID] = item;
			item;
		}
		return item;
	}
	,openContextMenu: function(e,item) {
		var _g = this;
		var menu = new ContextMenuControl();
		menu.addItem("Rename",function() {
			_g.startRenamingEntity(item);
		}).icon = "edit";
		menu.addItem("Duplicate",function() {
			var clone = item.entity.clone();
			clone.setName(ursine_editor_scene_entity_Entity.createCopyName(item.entity.getName()));
		}).icon = "duplicate";
		menu.addSeparator();
		var $delete = menu.addItem("Delete",function() {
			item.entity.remove();
		});
		$delete.icon = "remove";
		$delete.disabled = !item.entity.isRemovalEnabled();
		menu.open(e.clientX,e.clientY);
	}
	,startRenamingEntity: function(item) {
		item.textContentElement.contentEditable = "true";
		var range = window.document.createRange();
		range.selectNodeContents(item.textContentElement);
		var selection = window.getSelection();
		selection.removeAllRanges();
		selection.addRange(range);
	}
	,selectEntity: function(item) {
		if(item == null) ursine_editor_windows_EntityInspector.instance.inspect(null); else {
			item.selected = true;
			item.scrollIntoViewIfNeeded();
			ursine_editor_windows_EntityInspector.instance.inspect(item.entity);
			this.m_selectedEntities.push(item.entity.uniqueID);
		}
	}
	,deselectEntity: function(item) {
		if(item != null) {
			item.selected = false;
			ursine_editor_windows_EntityInspector.instance.inspect(null);
			this.m_selectedEntities = this.m_selectedEntities.filter(function(x) {
				return x != item.entity.uniqueID;
			});
		}
	}
	,__class__: ursine_editor_windows_SceneOutline
});
var ursine_editor_windows_SceneView = function() {
	this.m_selector = null;
	ursine_editor_NativeCanvasWindowHandler.call(this,"SceneView");
	this.window.heading = "Scene";
	this.onViewportInvalidated();
	this.window.addEventListener("keydown",$bind(this,this.onWindowKeyDown));
};
$hxClasses["ursine.editor.windows.SceneView"] = ursine_editor_windows_SceneView;
ursine_editor_windows_SceneView.__name__ = ["ursine","editor","windows","SceneView"];
ursine_editor_windows_SceneView.__super__ = ursine_editor_NativeCanvasWindowHandler;
ursine_editor_windows_SceneView.prototype = $extend(ursine_editor_NativeCanvasWindowHandler.prototype,{
	onWindowKeyDown: function(e) {
		var _g = e.keyCode;
		switch(_g) {
		case 46:
			ursine_editor_windows_SceneOutline.instance.deleteSelectedEntities();
			break;
		case 32:
			this.openEditorCommands();
			break;
		}
		return true;
	}
	,openEditorCommands: function() {
		var _g = this;
		if(this.m_selector != null) return;
		var items = ursine_native_Extern.GetEditorCommands();
		this.m_selector = new ItemSelectionPopupControl(items);
		this.m_selector.addEventListener("item-selected",function(e) {
			eval('editor_commands_'+ e.detail.item +'()');
			_g.clearCommandSelector();
		});
		this.m_selector.filterInput.addEventListener("keydown",function(e1) {
			if(e1.keyCode == 32 && _g.m_selector.currentFilter.length == 0) {
				_g.m_selector.close();
				e1.preventDefault();
				e1.stopPropagation();
				e1.stopImmediatePropagation();
				return false;
			}
			return true;
		});
		this.m_selector.addEventListener("closed",$bind(this,this.clearCommandSelector));
		haxe_Timer.delay(function() {
			if(_g.m_selector == null) return;
			window.document.body.appendChild(_g.m_selector);
			var bounds = _g.window.container.getBoundingClientRect();
			_g.m_selector.show(bounds.left + (bounds.width - 225) * 0.5,bounds.height * 0.35);
		},10);
	}
	,clearCommandSelector: function() {
		this.m_selector = null;
		haxe_Timer.delay(($_=this.window,$bind($_,$_.focus)),1);
	}
	,__class__: ursine_editor_windows_SceneView
});
var ursine_native_Extern = function() { };
$hxClasses["ursine.native.Extern"] = ursine_native_Extern;
ursine_native_Extern.__name__ = ["ursine","native","Extern"];
ursine_native_Extern.ProcessOpen = function(path,relative) {
	if(relative == null) relative = false;
	return ProcessOpen(path, relative);
};
ursine_native_Extern.GetEditorCommands = function() {
	return editor_commands_GetEditorCommands();
};
ursine_native_Extern.CreateEntity = function() {
	return CreateEntity();
};
ursine_native_Extern.GetNativeComponentDatabase = function() {
	return GetNativeComponentDatabase();
};
ursine_native_Extern.ProjectGetResourceTree = function() {
	return ProjectGetResourceTree();
};
ursine_native_Extern.ProjectGetResourcesByType = function(type) {
	return ProjectGetResourcesByType(type);
};
ursine_native_Extern.ProjectGetResource = function(guid) {
	return ProjectGetResource(guid);
};
ursine_native_Extern.ProjectSetEmptyScene = function() {
	return ProjectSetEmptyScene();
};
ursine_native_Extern.SceneSaveWorld = function() {
	return SceneSaveWorld();
};
ursine_native_Extern.SceneSaveWorldAs = function() {
	return SceneSaveWorldAs();
};
ursine_native_Extern.SceneSetActiveWorld = function(guid) {
	return SceneSetActiveWorld(guid);
};
ursine_native_Extern.SceneGetRootEntities = function() {
	return SceneGetRootEntities();
};
ursine_native_Extern.ScenePlayStart = function() {
	return ScenePlayStart();
};
ursine_native_Extern.SceneSetPlayState = function(playing) {
	return SceneSetPlayState(playing);
};
ursine_native_Extern.SceneStep = function() {
	return SceneStep();
};
ursine_native_Extern.ScenePlayStop = function() {
	return ScenePlayStop();
};
ursine_native_Extern.SceneGetEntitySystems = function() {
	return SceneGetEntitySystems();
};
ursine_native_Extern.NotificationButtonCallback = function(id,buttonID) {
	return ursine_NotificationButtonCallback(id, buttonID);
};
ursine_native_Extern.NotificationCloseCallback = function(id) {
	return ursine_NotificationCloseCallback(id);
};
ursine_native_Extern.WaypointUnidirectionalConnector = function() {
	return WaypointUnidirectionalConnector();
};
ursine_native_Extern.WaypointBidirectionalConnector = function() {
	return WaypointBidirectionalConnector();
};
ursine_native_Extern.WaypointEnableDrawLines = function() {
	return WaypointEnableDrawLines();
};
ursine_native_Extern.WaypointDisableDrawLines = function() {
	return WaypointDisableDrawLines();
};
var ursine_native_Property = function() { };
$hxClasses["ursine.native.Property"] = ursine_native_Property;
ursine_native_Property.__name__ = ["ursine","native","Property"];
var ursine_utils_EventManager = function() {
	this.m_events = new haxe_ds_StringMap();
};
$hxClasses["ursine.utils.EventManager"] = ursine_utils_EventManager;
ursine_utils_EventManager.__name__ = ["ursine","utils","EventManager"];
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
	,off: function(event,handler) {
		var handlers = this.m_events.get(event);
		if(handlers != null) {
			var x = handler;
			HxOverrides.remove(handlers,x);
		}
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
	,__class__: ursine_utils_EventManager
};
var ursine_utils_MetaUtils = function() { };
$hxClasses["ursine.utils.MetaUtils"] = ursine_utils_MetaUtils;
ursine_utils_MetaUtils.__name__ = ["ursine","utils","MetaUtils"];
ursine_utils_MetaUtils.getDerivedClasses = function(baseType) {
	var classTypeNames = Object.keys( $hxClasses );
	var resolvedBaseType = Type.resolveClass(Type.getClassName(baseType));
	var derivedTypes = [];
	var _g = 0;
	while(_g < classTypeNames.length) {
		var className = classTypeNames[_g];
		++_g;
		var classType = Type.resolveClass(className);
		var base = Type.getSuperClass(classType);
		if(base == resolvedBaseType) derivedTypes.push(classType);
	}
	return derivedTypes;
};
function $iterator(o) { if( o instanceof Array ) return function() { return HxOverrides.iter(o); }; return typeof(o.iterator) == 'function' ? $bind(o,o.iterator) : o.iterator; }
var $_, $fid = 0;
function $bind(o,m) { if( m == null ) return null; if( m.__id__ == null ) m.__id__ = $fid++; var f; if( o.hx__closures__ == null ) o.hx__closures__ = {}; else f = o.hx__closures__[m.__id__]; if( f == null ) { f = function(){ return f.method.apply(f.scope, arguments); }; f.scope = o; f.method = m; o.hx__closures__[m.__id__] = f; } return f; }
$hxClasses.Math = Math;
String.prototype.__class__ = $hxClasses.String = String;
String.__name__ = ["String"];
$hxClasses.Array = Array;
Array.__name__ = ["Array"];
var Int = $hxClasses.Int = { __name__ : ["Int"]};
var Dynamic = $hxClasses.Dynamic = { __name__ : ["Dynamic"]};
var Float = $hxClasses.Float = Number;
Float.__name__ = ["Float"];
var Bool = Boolean;
Bool.__ename__ = ["Bool"];
var Class = $hxClasses.Class = { __name__ : ["Class"]};
var Enum = { };
var __map_reserved = {}
js_Boot.__toStr = {}.toString;
ursine_editor_NativeCanvasWindowHandler.m_forwardedEvents = ["focus","blur","mouseover","mouseout"];
ursine_editor_menus_DebugMenu.__meta__ = { obj : { menuIndex : [3]}, statics : { doEditorReload : { mainMenuItem : ["Debug/Editor UI/Reload"]}, doEditorDebugTools : { mainMenuItem : ["Debug/Editor UI/Inspect"]}}};
ursine_editor_menus_EditMenu.__meta__ = { obj : { menuIndex : [1]}, statics : { doUndo : { mainMenuItem : ["Edit/Undo"]}, doRedo : { mainMenuItem : ["Edit/Redo"]}}};
ursine_editor_menus_EntityMenu.__meta__ = { obj : { menuIndex : [2]}, statics : { doCreateEmpty : { mainMenuItem : ["Entity/Create/Empty"]}, doCreateFromArchetype : { mainMenuItem : ["Entity/Create/From Archetype"]}, doCreatePlane : { mainMenuItem : ["Entity/Create/Plane",true]}, doCreateBox : { mainMenuItem : ["Entity/Create/Box"]}, doCreateCylinder : { mainMenuItem : ["Entity/Create/Cylinder"]}, doCreateSphere : { mainMenuItem : ["Entity/Create/Sphere"]}, doCreatePointLight : { mainMenuItem : ["Entity/Create/Point Light",true]}, doCreateSpotLight : { mainMenuItem : ["Entity/Create/Spot Light"]}, doCreateDirectionalLight : { mainMenuItem : ["Entity/Create/Directional Light"]}}};
ursine_editor_menus_FileMenu.__meta__ = { obj : { menuIndex : [0]}, statics : { doNewWorld : { mainMenuItem : ["File/New World",false,true]}, doSaveWorld : { mainMenuItem : ["File/Save World",false,false]}, doSaveWorldAs : { mainMenuItem : ["File/Save World As",false,false]}, doSaveProject : { mainMenuItem : ["File/Save Project",true,false]}}};
ursine_editor_menus_HelpMenu.__meta__ = { obj : { menuIndex : [4]}, statics : { doOpenGettingStarted : { mainMenuItem : ["Help/Editor Documentation"]}}};
ursine_editor_menus_ToolsMenu.__meta__ = { obj : { menuIndex : [5]}, statics : { uniConnector : { mainMenuItem : ["Tools/Waypoint Connector/Unidirectional Connections"]}, biConnector : { mainMenuItem : ["Tools/Waypoint Connector/Bidirectional Connections"]}, enableLines : { mainMenuItem : ["Tools/Waypoint Connector/Debug Lines/Enable"]}, disableLines : { mainMenuItem : ["Tools/Waypoint Connector/Debug Lines/Disable"]}}};
ursine_editor_scene_component_ComponentDatabase.m_componentInspectorMeta = "componentInspector";
ursine_editor_scene_component_ComponentDatabase.m_fieldInspectorMeta = "fieldInspector";
ursine_editor_scene_component_inspectors_FieldInspectionHandler.m_fieldNameRegex = new EReg("([A-Z](?=[A-Z][a-z])|[^A-Z](?=[A-Z])|[a-zA-Z](?=[^a-zA-Z]))","g");
ursine_editor_scene_component_inspectors_components_LightInspector.__meta__ = { obj : { componentInspector : ["Light"]}};
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeName = "ursine::ecs::LightType";
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName = "type";
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeDirectional = "Directional";
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypePoint = "Point";
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeSpot = "Spot";
ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector.__meta__ = { obj : { fieldInspector : ["bool"]}};
ursine_editor_scene_component_inspectors_fields_ColorFieldInspector.__meta__ = { obj : { fieldInspector : ["ursine::Color"]}};
ursine_editor_scene_component_inspectors_fields_EntitySystemSelectorInspector.__meta__ = { obj : { fieldInspector : ["EntitySystemSelector"]}};
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.__meta__ = { obj : { fieldInspector : ["int","unsigned int","float","double"]}};
ursine_editor_scene_component_inspectors_fields_ResourceReferenceInspector.__meta__ = { obj : { fieldInspector : ["ResourceReference"]}};
ursine_editor_scene_component_inspectors_fields_StringFieldInspector.__meta__ = { obj : { fieldInspector : ["std::string"]}};
ursine_editor_scene_component_inspectors_fields_UnknownTypeInspector.__meta__ = { obj : { fieldInspector : ["UNKNOWN"]}};
ursine_editor_scene_component_inspectors_fields_VectorFieldInspector.__meta__ = { obj : { fieldInspector : ["ursine::Vec2","ursine::Vec3","ursine::SVec3","ursine::Vec4","ursine::SVec4","ursine::SQuat"]}};
ursine_editor_scene_entity_EntityEvent.EntityAdded = "EntityAdded";
ursine_editor_scene_entity_EntityEvent.EntityRemoved = "EntityRemoved";
ursine_editor_scene_entity_EntityEvent.EntityNameChanged = "EntityNameChanged";
ursine_editor_scene_entity_EntityEvent.EntityParentChanged = "EntityParentChanged";
ursine_editor_scene_entity_EntityEvent.ComponentAdded = "ComponentAdded";
ursine_editor_scene_entity_EntityEvent.ComponentRemoved = "ComponentRemoved";
ursine_editor_scene_entity_EntityEvent.ComponentChanged = "ComponentChanged";
ursine_editor_scene_entity_EntityEvent.ComponentArrayInserted = "ComponentArrayInserted";
ursine_editor_scene_entity_EntityEvent.ComponentArraySet = "ComponentArraySet";
ursine_editor_scene_entity_EntityEvent.ComponentArrayRemove = "ComponentArrayRemove";
ursine_editor_windows_EntityInspector.m_smallWindowWidth = 245;
ursine_native_Property.DisableComponentRemoval = "DisableComponentRemoval";
ursine_native_Property.HiddenInInspector = "HiddenInInspector";
ursine_native_Property.HiddenInSelector = "HiddenInSelector";
ursine_native_Property.ForceEditorType = "ForceEditorType";
ursine_native_Property.InputRange = "InputRange";
ursine_native_Property.MultiLineEditor = "MultiLineEditor";
ursine_native_Property.Annotation = "Annotation";
ursine_native_Property.ResourceType = "ResourceType";
EditorMain.main();
})(typeof window != "undefined" ? window : typeof global != "undefined" ? global : typeof self != "undefined" ? self : this);
