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
Application.__name__ = ["Application"];
Application.main = function() {
	window.addEventListener("WebComponentsReady",Application.initWindows);
};
Application.initWindows = function() {
	var editor = new ursine_editor_Editor();
	var mainDockContainer = window.document.body.querySelector("#main-dock-container");
	var mainDock = new DockContainerControl();
	mainDock.style.width = "100%";
	mainDock.style.height = "100%";
	mainDockContainer.appendChild(mainDock);
	var sceneView = new ursine_editor_windows_SceneView();
	var leftColumn = mainDock.addColumn();
	leftColumn.style.width = "15%";
	var row = leftColumn.addRow();
	row.style.height = "100%";
	var column = row.addColumn();
	column.style.width = "100%";
	column.appendChild(new ursine_editor_windows_EntityInspector().window);
	var middleColumn = mainDock.addColumn();
	middleColumn.style.width = "70%";
	var row1 = middleColumn.addRow();
	row1.style.height = "100%";
	var column1 = row1.addColumn();
	column1.style.width = "100%";
	column1.appendChild(sceneView.window);
	var rightColumn = mainDock.addColumn();
	rightColumn.style.width = "15%";
	var row2 = rightColumn.addRow();
	row2.style.height = "100%";
	var column2 = row2.addColumn();
	column2.style.width = "100%";
	column2.appendChild(new ursine_editor_windows_SceneOutline().window);
	sceneView.onViewportInvalidated();
};
var HxOverrides = function() { };
$hxClasses["HxOverrides"] = HxOverrides;
HxOverrides.__name__ = ["HxOverrides"];
HxOverrides.cca = function(s,index) {
	var x = s.charCodeAt(index);
	if(x != x) return undefined;
	return x;
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
});
var js_Boot = function() { };
$hxClasses["js.Boot"] = js_Boot;
js_Boot.__name__ = ["js","Boot"];
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
		var btnToggle = window.document.querySelector("#simulation-toggle");
		var btnStep = window.document.querySelector("#simulation-step");
		btnToggle.addEventListener("click",function() {
			btnToggle.classList.toggle("running");
			var running = btnToggle.classList.contains("running");
			btnStep.classList.toggle("disabled",running);
			ursine_native_Extern.ScenePlay(running);
		});
		btnStep.addEventListener("click",function() {
			if(btnStep.classList.contains("disabled")) return;
			ursine_native_Extern.SceneStep();
		});
	}
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
};
var ursine_editor_WindowHandler = function() {
	this.window = new EditorWindowControl();
};
$hxClasses["ursine.editor.WindowHandler"] = ursine_editor_WindowHandler;
ursine_editor_WindowHandler.__name__ = ["ursine","editor","WindowHandler"];
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
};
var ursine_editor_menus_DebugMenu = function() { };
$hxClasses["ursine.editor.menus.DebugMenu"] = ursine_editor_menus_DebugMenu;
ursine_editor_menus_DebugMenu.__name__ = ["ursine","editor","menus","DebugMenu"];
ursine_editor_menus_DebugMenu.doEditorReload = function() {
};
ursine_editor_menus_DebugMenu.doEditorDebugTools = function() {
	ursine_native_Extern.DebugEditorUI();
};
ursine_editor_menus_DebugMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_DebugMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
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
});
var ursine_editor_menus_EntityMenu = function() { };
$hxClasses["ursine.editor.menus.EntityMenu"] = ursine_editor_menus_EntityMenu;
ursine_editor_menus_EntityMenu.__name__ = ["ursine","editor","menus","EntityMenu"];
ursine_editor_menus_EntityMenu.doCreateEmpty = function() {
	ursine_editor_menus_EntityMenu.createEntity("Empty Entity");
};
ursine_editor_menus_EntityMenu.doCreateFromArchetype = function() {
	ursine_editor_scene_entity_Entity.createFromArchetype();
};
ursine_editor_menus_EntityMenu.doCreatePlane = function() {
	var entity = ursine_editor_menus_EntityMenu.createEntityWithComponents("Plane",["BoxCollider"]);
	entity.componentFieldUpdate("Transform","scale",{ x : 5, y : 1, z : 5});
};
ursine_editor_menus_EntityMenu.doCreateBox = function() {
	var entity = ursine_editor_menus_EntityMenu.createEntityWithComponents("Box",["BoxCollider"]);
};
ursine_editor_menus_EntityMenu.doCreateCylinder = function() {
	var entity = ursine_editor_menus_EntityMenu.createEntityWithComponents("Cylinder",["CylinderCollider"]);
};
ursine_editor_menus_EntityMenu.doCreateSphere = function() {
	var entity = ursine_editor_menus_EntityMenu.createEntityWithComponents("Sphere",["SphereCollider"]);
};
ursine_editor_menus_EntityMenu.doCreatePointLight = function() {
	var entity = ursine_editor_menus_EntityMenu.createLight("Point Light","Point");
};
ursine_editor_menus_EntityMenu.doCreateSpotLight = function() {
	var entity = ursine_editor_menus_EntityMenu.createLight("Spot Light","Spot");
};
ursine_editor_menus_EntityMenu.doCreateDirectionalLight = function() {
	var entity = ursine_editor_menus_EntityMenu.createLight("Directional Light","Directional");
};
ursine_editor_menus_EntityMenu.createEntity = function(name) {
	var entity = ursine_editor_scene_entity_Entity.create();
	entity.setName(name);
	ursine_editor_windows_SceneOutline.instance.clearSelectedEntities();
	entity.select();
	return entity;
};
ursine_editor_menus_EntityMenu.createEntityWithComponents = function(name,components) {
	var entity = ursine_editor_scene_entity_Entity.create();
	entity.setName(name);
	var _g = 0;
	while(_g < components.length) {
		var comp = components[_g];
		++_g;
		entity.addComponent(comp);
	}
	return entity;
};
ursine_editor_menus_EntityMenu.createLight = function(name,type) {
	var entity = ursine_editor_menus_EntityMenu.createEntity(name);
	entity.addComponent("Light");
	var lightType = ursine_editor_Editor.instance.componentDatabase.getNativeType("ursine::ecs::LightType").enumValue;
	entity.componentFieldUpdate("Light","Type",Reflect.field(lightType,type));
	return entity;
};
ursine_editor_menus_EntityMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_EntityMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
});
var ursine_editor_menus_FileMenu = function() { };
$hxClasses["ursine.editor.menus.FileMenu"] = ursine_editor_menus_FileMenu;
ursine_editor_menus_FileMenu.__name__ = ["ursine","editor","menus","FileMenu"];
ursine_editor_menus_FileMenu.doNew = function() {
	ursine_native_Extern.SceneLoad();
};
ursine_editor_menus_FileMenu.doOpen = function() {
	ursine_native_Extern.SceneSave();
};
ursine_editor_menus_FileMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_FileMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
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
};
var ursine_editor_scene_component_inspectors_ComponentInspectionHandler = function(entity,component) {
	this.m_entity = entity;
	this.m_component = component;
	this.m_componentType = ursine_editor_Editor.instance.componentDatabase.getComponentType(this.m_component.type);
	this.m_fieldHandlers = new haxe_ds_StringMap();
	this.fieldChangeEvents = new ursine_utils_EventManager();
	this.inspector = new ComponentInspectorControl();
	this.inspector.heading = component.type;
};
$hxClasses["ursine.editor.scene.component.inspectors.ComponentInspectionHandler"] = ursine_editor_scene_component_inspectors_ComponentInspectionHandler;
ursine_editor_scene_component_inspectors_ComponentInspectionHandler.__name__ = ["ursine","editor","scene","component","inspectors","ComponentInspectionHandler"];
ursine_editor_scene_component_inspectors_ComponentInspectionHandler.prototype = {
	updateField: function(name,value) {
		this.m_component.value[name] = value;
		var result = this.fieldChangeEvents.trigger(name,{ name : name, value : value});
		if(result != false) {
			var handler = this.m_fieldHandlers.get(name);
			if(handler != null) handler.updateValue(value);
		}
	}
	,addButton: function(button) {
		var _g = this;
		var element = new ButtonControl();
		element.classList.add("x-component-inspector");
		element.text = button.text;
		element.addEventListener("click",function() {
			_g.m_entity.componentButtonInvoke(_g.m_component.type,button.name);
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
	,notifyChanged: function(field,value) {
		this.m_entity.componentFieldUpdate(this.m_component.type,field.name,value);
	}
	,remove: function() {
		var $it0 = this.m_fieldHandlers.iterator();
		while( $it0.hasNext() ) {
			var handler = $it0.next();
			handler.remove();
		}
		if(this.inspector.parentNode != null) this.inspector.parentNode.removeChild(this.inspector);
	}
};
var ursine_editor_scene_component_inspectors_FieldInspectionHandler = function(owner,instance,field,type) {
	this.m_owner = owner;
	this.m_instance = instance;
	this.m_field = field;
	this.m_type = type;
	this.inspector = new FieldInspectorControl();
	this.inspector.heading = field.name;
};
$hxClasses["ursine.editor.scene.component.inspectors.FieldInspectionHandler"] = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_FieldInspectionHandler.__name__ = ["ursine","editor","scene","component","inspectors","FieldInspectionHandler"];
ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype = {
	updateValue: function(value) {
		this.m_instance = value;
	}
	,remove: function() {
		if(this.inspector.parentNode != null) this.inspector.parentNode.removeChild(this.inspector);
	}
	,get_name: function() {
		return this.m_field.name;
	}
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
});
var ursine_editor_scene_component_inspectors_components_LightInspector = function(entity,component) {
	ursine_editor_scene_component_inspectors_ComponentInspectionHandler.call(this,entity,component);
	this.fieldChangeEvents.on(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName,$bind(this,this.onLightTypeChange));
	this.m_typeFields = [];
	if(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum == null) this.initTypeToFields();
	var database = ursine_editor_Editor.instance.componentDatabase;
	var typeInstance = Reflect.field(component.value,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName);
	this.addField(database.createFieldInspector(this,typeInstance,database.getComponentTypeField(this.m_componentType,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName),database.getNativeType(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeName)));
	this.setType(typeInstance);
};
$hxClasses["ursine.editor.scene.component.inspectors.components.LightInspector"] = ursine_editor_scene_component_inspectors_components_LightInspector;
ursine_editor_scene_component_inspectors_components_LightInspector.__name__ = ["ursine","editor","scene","component","inspectors","components","LightInspector"];
ursine_editor_scene_component_inspectors_components_LightInspector.__super__ = ursine_editor_scene_component_inspectors_ComponentInspectionHandler;
ursine_editor_scene_component_inspectors_components_LightInspector.prototype = $extend(ursine_editor_scene_component_inspectors_ComponentInspectionHandler.prototype,{
	setType: function(type) {
		var database = ursine_editor_Editor.instance.componentDatabase;
		var componentType = database.getComponentType(this.m_component.type);
		while(this.m_typeFields.length > 0) this.m_typeFields.pop().remove();
		var fields = ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields.h[type];
		var _g = 0;
		while(_g < fields.length) {
			var fieldName = fields[_g];
			++_g;
			var field = database.getComponentTypeField(this.m_componentType,fieldName);
			if(field.name == ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName) continue;
			var instance = Reflect.field(this.m_component.value,field.name);
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
		ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum = ursine_editor_Editor.instance.componentDatabase.getNativeType(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeName).enumValue;
		ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields = new haxe_ds_IntMap();
		var k = Reflect.field(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeDirectional);
		var v = ["Color","Direction","Intensity"];
		ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields.h[k] = v;
		v;
		var k1 = Reflect.field(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypePoint);
		var v1 = ["Color","Intensity","Radius"];
		ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields.h[k1] = v1;
		v1;
		var k2 = Reflect.field(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeEnum,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeSpot);
		var v2 = ["Color","Direction","Intensity","SpotlightAngles"];
		ursine_editor_scene_component_inspectors_components_LightInspector.m_typeToFields.h[k2] = v2;
		v2;
	}
});
var ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector = function(parent,owner,instance,field,type) {
	this.m_parent = parent;
	this.m_owner = owner;
	this.m_instance = instance;
	this.m_field = field;
	this.m_type = type;
	this.initElements();
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.ArrayTypeInspector"] = ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector;
ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","ArrayTypeInspector"];
ursine_editor_scene_component_inspectors_fields_ArrayTypeInspector.prototype = {
	initElements: function() {
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
		this.m_btnAddItem.text = "Add Item";
		this.m_btnAddItem.addEventListener("click",$bind(this,this.onAddItemClicked));
		itemsWrapper.appendChild(this.m_btnAddItem);
	}
	,onAddItemClicked: function(e) {
	}
};
var ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector = function(owner,instance,field,type) {
	var _g = this;
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	this.m_bool = new CheckBoxInputControl();
	this.m_bool.addEventListener("change",function() {
		_g.m_owner.notifyChanged(_g.m_field,_g.m_bool.checked);
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
		if(this.m_colorInput != null) window.document.body.removeChild(this.m_colorInput);
		ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype.remove.call(this);
	}
	,onPreviewClick: function(e) {
		if(this.m_colorInput != null) return;
		this.m_colorInput = new ColorInputControl(this.m_instance);
		this.m_colorInput.addEventListener("color-changed",$bind(this,this.onColorChanged));
		this.m_colorInput.addEventListener("closed",$bind(this,this.onColorClosed));
		this.m_colorInput.style.left = "" + e.clientX + "px";
		this.m_colorInput.style.top = "" + e.clientY + "px";
		window.document.body.appendChild(this.m_colorInput);
	}
	,onColorChanged: function(e) {
		this.m_owner.notifyChanged(this.m_field,e.detail.color);
	}
	,onColorClosed: function(e) {
		this.m_colorInput = null;
	}
});
var ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	if(type.isArray) this.initArray(); else if(type.enumValue != null) this.initEnum();
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.DefaultFieldInspector"] = ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector;
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","DefaultFieldInspector"];
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	updateValue: function(value) {
		this.m_instance = value;
		if(this.m_isEnum) {
			if(this.m_isBitMaskEditor) this.loadEnumBitMaskValue(value); else this.m_comboInput.value = value;
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
		var values = Reflect.fields(this.m_type.enumValue);
		var _g1 = 0;
		while(_g1 < values.length) {
			var key = values[_g1];
			++_g1;
			var option;
			var _this = window.document;
			option = _this.createElement("option");
			option.text = key;
			option.value = Reflect.field(this.m_type.enumValue,key);
			this.m_comboInput.appendChild(option);
			{
				this.m_enumValueOptions.set(key,option);
				option;
			}
		}
		this.m_isBitMaskEditor = Reflect.field(this.m_field.meta,"BitMaskEditor") != null;
		if(this.m_isBitMaskEditor) {
			this.m_comboInput.multiple = true;
			this.m_comboInput.size = Std["int"](Math.min(10,values.length));
		}
		this.m_comboInput.addEventListener("change",function(e) {
			_g.m_instance = _g.getEnumBitMaskValue();
			_g.m_owner.notifyChanged(_g.m_field,_g.m_instance);
		});
		this.inspector.container.appendChild(this.m_comboInput);
		this.updateValue(this.m_instance);
	}
	,loadEnumBitMaskValue: function(value) {
		var values = Reflect.fields(this.m_type.enumValue);
		var _g = 0;
		while(_g < values.length) {
			var key = values[_g];
			++_g;
			var option = this.m_enumValueOptions.get(key);
			var keyValue = Reflect.field(this.m_type.enumValue,key);
			option.selected = (value & keyValue) == keyValue;
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
		_g.m_owner.notifyChanged(_g.m_field,value);
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
});
var ursine_editor_scene_component_inspectors_fields_StringFieldInspector = function(owner,instance,field,type) {
	var _g = this;
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	this.m_string = new TextInputControl();
	this.m_string.addEventListener("change",function() {
		_g.m_owner.notifyChanged(_g.m_field,_g.m_string.value);
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
			_g.m_owner.notifyChanged(_g.m_field,_g.m_instance);
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
});
var ursine_utils_IEventContainer = function() { };
$hxClasses["ursine.utils.IEventContainer"] = ursine_utils_IEventContainer;
ursine_utils_IEventContainer.__name__ = ["ursine","utils","IEventContainer"];
var ursine_editor_scene_entity_Entity = function(uniqueID) {
	this.events = new ursine_utils_EventManager();
	this.uniqueID = uniqueID;
	this.m_handler = new EntityHandler(uniqueID);
	ursine_editor_Editor.instance.broadcastManager.getChannel("EntityManager").on(ursine_editor_scene_entity_EntityEvent.ComponentAdded,$bind(this,this.onComponentAdded)).on(ursine_editor_scene_entity_EntityEvent.ComponentRemoved,$bind(this,this.onComponentRemoved)).on(ursine_editor_scene_entity_EntityEvent.ComponentChanged,$bind(this,this.onComponentChanged));
};
$hxClasses["ursine.editor.scene.entity.Entity"] = ursine_editor_scene_entity_Entity;
ursine_editor_scene_entity_Entity.__name__ = ["ursine","editor","scene","entity","Entity"];
ursine_editor_scene_entity_Entity.__interfaces__ = [ursine_utils_IEventContainer];
ursine_editor_scene_entity_Entity.create = function() {
	return new ursine_editor_scene_entity_Entity(ursine_native_Extern.CreateEntity());
};
ursine_editor_scene_entity_Entity.createFromArchetype = function() {
	ursine_native_Extern.CreateEntityFromArchetype();
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
	,hasComponent: function(name) {
		return this.m_handler.hasComponent(name);
	}
	,addComponent: function(name) {
		this.m_handler.addComponent(name);
	}
	,removeComponent: function(name) {
		this.m_handler.removeComponent(name);
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
	,componentFieldArrayRemove: function(componentName,fieldName,index) {
		this.m_handler.componentFieldArrayRemove(componentName,fieldName,index);
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
	,clearOldInspection: function() {
		if(this.m_inspectedEntity != null) this.m_inspectedEntity.events.off(ursine_editor_scene_entity_EntityEvent.ComponentAdded,$bind(this,this.onInspectedEntityComponentAdded)).off(ursine_editor_scene_entity_EntityEvent.ComponentRemoved,$bind(this,this.onInspectedEntityComponentRemoved)).off(ursine_editor_scene_entity_EntityEvent.ComponentChanged,$bind(this,this.onInspectedEntityComponentChanged));
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
		this.m_inspectedEntity.events.on(ursine_editor_scene_entity_EntityEvent.ComponentAdded,$bind(this,this.onInspectedEntityComponentAdded)).on(ursine_editor_scene_entity_EntityEvent.ComponentRemoved,$bind(this,this.onInspectedEntityComponentRemoved)).on(ursine_editor_scene_entity_EntityEvent.ComponentChanged,$bind(this,this.onInspectedEntityComponentChanged));
		var inspection = this.m_inspectedEntity.inspect();
		var _g = 0;
		while(_g < inspection.length) {
			var component = inspection[_g];
			++_g;
			this.inspectComponent(component);
		}
	}
	,inspectComponent: function(component) {
		if(this.m_componentHandlers.get(component.type) != null) return;
		var database = ursine_editor_Editor.instance.componentDatabase;
		var type = database.getComponentType(component.type);
		if(Object.prototype.hasOwnProperty.call(type.meta,ursine_native_Property.HiddenInInspector)) return;
		var handler = database.createComponentInspector(this.m_inspectedEntity,component);
		handler.inspector.canRemove = !Object.prototype.hasOwnProperty.call(type.meta,ursine_native_Property.DisableComponentRemoval);
		handler.inspector.opened = this.m_openCache.get(component.type) == true;
		handler.inspector.addEventListener("removed",(function(f,a1) {
			return function(e) {
				f(a1,e);
			};
		})($bind(this,this.onRemoveComponentClicked),component));
		handler.inspector.addEventListener("open-changed",(function(f1,a11) {
			return function(e1) {
				f1(a11,e1);
			};
		})($bind(this,this.onOpenChanged),component));
		{
			this.m_componentHandlers.set(component.type,handler);
			handler;
		}
		this.m_inspectorsContainer.appendChild(handler.inspector);
	}
	,removeInspector: function(inspector) {
		inspector.remove();
	}
	,getAvailableComponentTypes: function(entity) {
		var db = ursine_editor_Editor.instance.componentDatabase;
		return db.getComponentTypes().filter(function(type) {
			var componentType = db.getComponentType(type);
			var isHidden = Object.prototype.hasOwnProperty.call(componentType.meta,ursine_native_Property.HiddenInInspector);
			return !entity.hasComponent(type) && !isHidden;
		});
	}
	,onArchetypeSaveClicked: function(e) {
		this.m_inspectedEntity.saveAsArchetype();
	}
	,onCopyEntityClicked: function(e) {
		ursine_editor_windows_SceneOutline.instance.clearSelectedEntities();
		var entity = this.m_inspectedEntity.clone();
		entity.setName(this.m_inspectedEntity.getName() + " Copy");
		entity.select();
	}
	,onAddComponentClicked: function(e) {
		var types = this.getAvailableComponentTypes(this.m_inspectedEntity);
		var selector = new ComponentTypeSelectorControl(types);
		selector.style.left = "" + e.clientX + "px";
		selector.style.top = "" + e.clientY + "px";
		selector.addEventListener("type-selected",$bind(this,this.onAddComponentTypeSelected));
		window.document.body.appendChild(selector);
	}
	,onRemoveComponentClicked: function(component,e) {
		this.m_inspectedEntity.removeComponent(component.type);
		e.stopPropagation();
	}
	,onAddComponentTypeSelected: function(e) {
		var componentType = e.detail.type;
		if(this.m_inspectedEntity.hasComponent(componentType)) throw new js__$Boot_HaxeError("Entity already has component type " + componentType);
		this.m_inspectedEntity.addComponent(componentType);
	}
	,onOpenChanged: function(component,e) {
		var v = e.detail.open;
		this.m_openCache.set(component.type,v);
		v;
	}
	,initWindow: function() {
		this.window.classList.add("entity-inspector-window");
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
});
var ursine_editor_windows_SceneOutline = function() {
	this.m_selectedEntities = null;
	ursine_editor_windows_SceneOutline.instance = this;
	ursine_editor_WindowHandler.call(this);
	this.window.heading = "Outline";
	this.window.classList.add("scene-outline-window");
	this.m_rootView = new TreeViewControl();
	this.m_rootView.setAsRoot(true);
	this.m_entityItems = new haxe_ds_IntMap();
	this.m_selectedEntities = [];
	this.window.container.appendChild(this.m_rootView);
	this.resetScene();
	ursine_editor_Editor.instance.broadcastManager.getChannel("SceneManager").on("Reset",$bind(this,this.resetScene));
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
		item.parentNode.removeChild(item);
		this.m_entityItems.remove(e.uniqueID);
	}
	,onEntityNameChanged: function(e) {
		var item = this.m_entityItems.h[e.uniqueID];
		if(item == null) return;
		item.text = e.name;
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
		item.addEventListener("drag-start",function(e) {
			e.stopPropagation();
			e.stopImmediatePropagation();
			e.preventStart = !entity.isHierarchyChangeEnabled();
		});
		item.addEventListener("drag-drop",function(e1) {
			e1.stopPropagation();
			e1.stopImmediatePropagation();
			if(!entity.isHierarchyChangeEnabled()) {
				e1.preventDrop = true;
				return;
			}
			var target = e1.detail.dropTarget.entity;
			var parent = entity.getParent();
			var targetID;
			if(target == null) targetID = -1; else targetID = target.uniqueID;
			var parentID;
			if(parent == null) parentID = -1; else parentID = parent.uniqueID;
			if(targetID == parentID) {
				e1.preventDrop = e1.detail.newParent;
				return;
			}
			entity.setParent(target);
		});
		item.addEventListener("drag-drop-after",function(e2) {
			e2.stopPropagation();
			e2.stopImmediatePropagation();
			var childIndex = ElementUtils.childIndex(item);
			entity.setSiblingIndex(childIndex);
		});
		item.textContentElement.addEventListener("dblclick",function() {
			item.textContentElement.contentEditable = "true";
			var range = window.document.createRange();
			range.selectNodeContents(item.textContentElement);
			var selection = window.getSelection();
			selection.removeAllRanges();
			selection.addRange(range);
		});
		item.textContentElement.addEventListener("keydown",function(e3) {
			if(e3.keyCode == 13) {
				item.textContentElement.blur();
				e3.preventDefault();
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
		item.textElement.addEventListener("click",function(e4) {
			_g.clearSelectedEntities();
			item.entity.select();
		});
		{
			this.m_entityItems.h[entity.uniqueID] = item;
			item;
		}
		return item;
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
});
var ursine_editor_windows_SceneView = function() {
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
		}
	}
});
var ursine_native_Extern = function() { };
$hxClasses["ursine.native.Extern"] = ursine_native_Extern;
ursine_native_Extern.__name__ = ["ursine","native","Extern"];
ursine_native_Extern.DebugEditorUI = function() {
	return DebugEditorUI();
};
ursine_native_Extern.CreateEntity = function() {
	return CreateEntity();
};
ursine_native_Extern.CreateEntityFromArchetype = function() {
	return CreateEntityFromArchetype();
};
ursine_native_Extern.GetNativeComponentDatabase = function() {
	return GetNativeComponentDatabase();
};
ursine_native_Extern.SceneGetRootEntities = function() {
	return SceneGetRootEntities();
};
ursine_native_Extern.SceneLoad = function() {
	return SceneLoad();
};
ursine_native_Extern.SceneSave = function() {
	return SceneSave();
};
ursine_native_Extern.ScenePlay = function(playing) {
	return ScenePlay(playing);
};
ursine_native_Extern.SceneStep = function() {
	return SceneStep();
};
ursine_native_Extern.NotificationButtonCallback = function(id,buttonID) {
	return ursine_NotificationButtonCallback(id, buttonID);
};
ursine_native_Extern.NotificationCloseCallback = function(id) {
	return ursine_NotificationCloseCallback(id);
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
String.__name__ = ["String"];
$hxClasses.Array = Array;
Array.__name__ = ["Array"];
var __map_reserved = {}
ursine_editor_NativeCanvasWindowHandler.m_forwardedEvents = ["focus","blur","mouseover","mouseout"];
ursine_editor_menus_DebugMenu.__meta__ = { obj : { menuIndex : [3]}, statics : { doEditorReload : { mainMenuItem : ["Debug/Editor UI/Reload"]}, doEditorDebugTools : { mainMenuItem : ["Debug/Editor UI/Inspect"]}}};
ursine_editor_menus_EditMenu.__meta__ = { obj : { menuIndex : [1]}, statics : { doUndo : { mainMenuItem : ["Edit/Undo"]}, doRedo : { mainMenuItem : ["Edit/Redo"]}}};
ursine_editor_menus_EntityMenu.__meta__ = { obj : { menuIndex : [2]}, statics : { doCreateEmpty : { mainMenuItem : ["Entity/Create/Empty"]}, doCreateFromArchetype : { mainMenuItem : ["Entity/Create/From Archetype"]}, doCreatePlane : { mainMenuItem : ["Entity/Create/Plane",true]}, doCreateBox : { mainMenuItem : ["Entity/Create/Box"]}, doCreateCylinder : { mainMenuItem : ["Entity/Create/Cylinder"]}, doCreateSphere : { mainMenuItem : ["Entity/Create/Sphere"]}, doCreatePointLight : { mainMenuItem : ["Entity/Create/Point Light",true]}, doCreateSpotLight : { mainMenuItem : ["Entity/Create/Spot Light"]}, doCreateDirectionalLight : { mainMenuItem : ["Entity/Create/Directional Light"]}}};
ursine_editor_menus_FileMenu.__meta__ = { obj : { menuIndex : [0]}, statics : { doNew : { mainMenuItem : ["File/Load Scene"]}, doOpen : { mainMenuItem : ["File/Save Scene"]}}};
ursine_editor_scene_component_ComponentDatabase.m_componentInspectorMeta = "componentInspector";
ursine_editor_scene_component_ComponentDatabase.m_fieldInspectorMeta = "fieldInspector";
ursine_editor_scene_component_inspectors_components_LightInspector.__meta__ = { obj : { componentInspector : ["Light"]}};
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeName = "ursine::ecs::LightType";
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName = "Type";
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeDirectional = "Directional";
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypePoint = "Point";
ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeSpot = "Spot";
ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector.__meta__ = { obj : { fieldInspector : ["bool"]}};
ursine_editor_scene_component_inspectors_fields_ColorFieldInspector.__meta__ = { obj : { fieldInspector : ["ursine::Color"]}};
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.__meta__ = { obj : { fieldInspector : ["int","unsigned int","float","double"]}};
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
ursine_native_Property.DisableComponentRemoval = "DisableComponentRemoval";
ursine_native_Property.HiddenInInspector = "HiddenInInspector";
ursine_native_Property.ForceEditorType = "ForceEditorType";
ursine_native_Property.InputRange = "InputRange";
Application.main();
})();
