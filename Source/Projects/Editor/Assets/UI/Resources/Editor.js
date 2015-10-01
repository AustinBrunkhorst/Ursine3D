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
	var editor = new ursine_editor_Editor();
	editor.broadcastManager.getChannel("EntityManager").on("EntityCreated",function(data) {
		console.log(data);
	});
	var windows = [new ursine_editor_windows_EntityInspector(),new ursine_editor_windows_SceneOutline(),new ursine_editor_windows_SceneView()];
	var windowContainer = window.document.body.querySelector("#window-container");
	var _g = 0;
	while(_g < windows.length) {
		var handler = windows[_g];
		++_g;
		windowContainer.appendChild(handler.window);
	}
};
Math.__name__ = ["Math"];
var Reflect = function() { };
$hxClasses["Reflect"] = Reflect;
Reflect.__name__ = ["Reflect"];
Reflect.field = function(o,field) {
	try {
		return o[field];
	} catch( e ) {
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
var haxe_IMap = function() { };
$hxClasses["haxe.IMap"] = haxe_IMap;
haxe_IMap.__name__ = ["haxe","IMap"];
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
var ursine_editor_Editor = function() {
	ursine_editor_Editor.instance = this;
	this.broadcastManager = new ursine_editor_NativeBroadcastManager();
	this.mainMenu = new MainMenuControl();
	this.buildMenus();
	window.document.querySelector("#header-toolbar").appendChild(this.mainMenu);
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
};
var ursine_editor_MenuItemHandler = function() { };
$hxClasses["ursine.editor.MenuItemHandler"] = ursine_editor_MenuItemHandler;
ursine_editor_MenuItemHandler.__name__ = ["ursine","editor","MenuItemHandler"];
var ursine_editor_NativeBroadcastManager = function() {
	this.m_channels = new haxe_ds_StringMap();
	window.NativeBroadcast = ursine_editor_NativeBroadcastManager.onBroadcast;
};
$hxClasses["ursine.editor.NativeBroadcastManager"] = ursine_editor_NativeBroadcastManager;
ursine_editor_NativeBroadcastManager.__name__ = ["ursine","editor","NativeBroadcastManager"];
ursine_editor_NativeBroadcastManager.onBroadcast = function(target,message,data) {
	ursine_editor_Editor.instance.broadcastManager.getChannel(target).trigger(message,data);
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
var ursine_editor_menus_DebugMenu = function() { };
$hxClasses["ursine.editor.menus.DebugMenu"] = ursine_editor_menus_DebugMenu;
ursine_editor_menus_DebugMenu.__name__ = ["ursine","editor","menus","DebugMenu"];
ursine_editor_menus_DebugMenu.doEditorDebugTools = function() {
	DebugEditorUI( );
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
	console.log("create empty");
};
ursine_editor_menus_EntityMenu.doCreatePlane = function() {
	console.log("create plane");
};
ursine_editor_menus_EntityMenu.doCreateBox = function() {
	console.log("create box");
};
ursine_editor_menus_EntityMenu.doCreateCylinder = function() {
	console.log("create Cylinder");
};
ursine_editor_menus_EntityMenu.doCreateSphere = function() {
	console.log("create Sphere");
};
ursine_editor_menus_EntityMenu.doCreatePointLight = function() {
	console.log("create Point Light");
};
ursine_editor_menus_EntityMenu.doCreateSpotLight = function() {
	console.log("create Spot Light");
};
ursine_editor_menus_EntityMenu.doCreateDirectionalLight = function() {
	console.log("create Directional Light");
};
ursine_editor_menus_EntityMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_EntityMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
});
var ursine_editor_menus_FileMenu = function() { };
$hxClasses["ursine.editor.menus.FileMenu"] = ursine_editor_menus_FileMenu;
ursine_editor_menus_FileMenu.__name__ = ["ursine","editor","menus","FileMenu"];
ursine_editor_menus_FileMenu.doNew = function() {
	console.log("new");
};
ursine_editor_menus_FileMenu.doOpen = function() {
	console.log("open");
};
ursine_editor_menus_FileMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_FileMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
});
var ursine_editor_windows_EntityInspector = function() {
	ursine_editor_WindowHandler.call(this);
	this.window.heading = "Inspector";
	this.window.style.top = "0";
	this.window.style.bottom = "50%";
	this.window.style.left = "0";
	this.window.style.width = "15%";
};
$hxClasses["ursine.editor.windows.EntityInspector"] = ursine_editor_windows_EntityInspector;
ursine_editor_windows_EntityInspector.__name__ = ["ursine","editor","windows","EntityInspector"];
ursine_editor_windows_EntityInspector.__super__ = ursine_editor_WindowHandler;
ursine_editor_windows_EntityInspector.prototype = $extend(ursine_editor_WindowHandler.prototype,{
});
var ursine_editor_windows_SceneOutline = function() {
	ursine_editor_WindowHandler.call(this);
	this.window.heading = "Outline";
	this.window.style.top = "50%";
	this.window.style.bottom = "0";
	this.window.style.left = "0";
	this.window.style.width = "15%";
};
$hxClasses["ursine.editor.windows.SceneOutline"] = ursine_editor_windows_SceneOutline;
ursine_editor_windows_SceneOutline.__name__ = ["ursine","editor","windows","SceneOutline"];
ursine_editor_windows_SceneOutline.__super__ = ursine_editor_WindowHandler;
ursine_editor_windows_SceneOutline.prototype = $extend(ursine_editor_WindowHandler.prototype,{
});
var ursine_editor_windows_SceneView = function() {
	ursine_editor_WindowHandler.call(this);
	this.window.heading = "Scene";
	this.window.style.top = "0";
	this.window.style.bottom = "0";
	this.window.style.left = "15%";
	this.window.style.right = "0";
	console.log("testing");
};
$hxClasses["ursine.editor.windows.SceneView"] = ursine_editor_windows_SceneView;
ursine_editor_windows_SceneView.__name__ = ["ursine","editor","windows","SceneView"];
ursine_editor_windows_SceneView.__super__ = ursine_editor_WindowHandler;
ursine_editor_windows_SceneView.prototype = $extend(ursine_editor_WindowHandler.prototype,{
});
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
$hxClasses.Math = Math;
String.__name__ = ["String"];
$hxClasses.Array = Array;
Array.__name__ = ["Array"];
var __map_reserved = {}
ursine_editor_menus_DebugMenu.__meta__ = { statics : { doEditorDebugTools : { mainMenuItem : ["Debug/Debug Editor UI"]}}};
ursine_editor_menus_EditMenu.__meta__ = { obj : { menuIndex : [1]}, statics : { doUndo : { mainMenuItem : ["Edit/Undo"]}, doRedo : { mainMenuItem : ["Edit/Redo"]}}};
ursine_editor_menus_EntityMenu.__meta__ = { obj : { menuIndex : [2]}, statics : { doCreateEmpty : { mainMenuItem : ["Entity/Create/Empty"]}, doCreatePlane : { mainMenuItem : ["Entity/Create/Plane",true]}, doCreateBox : { mainMenuItem : ["Entity/Create/Box"]}, doCreateCylinder : { mainMenuItem : ["Entity/Create/Cylinder"]}, doCreateSphere : { mainMenuItem : ["Entity/Create/Sphere"]}, doCreatePointLight : { mainMenuItem : ["Entity/Create/Point Light",true]}, doCreateSpotLight : { mainMenuItem : ["Entity/Create/Spot Light"]}, doCreateDirectionalLight : { mainMenuItem : ["Entity/Create/Directional Light"]}}};
ursine_editor_menus_FileMenu.__meta__ = { obj : { menuIndex : [0]}, statics : { doNew : { mainMenuItem : ["File/New"]}, doOpen : { mainMenuItem : ["File/Open"]}}};
Application.main();
})();
