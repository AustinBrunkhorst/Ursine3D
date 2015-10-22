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
var haxe_ds_IntMap = function() {
	this.h = { };
};
$hxClasses["haxe.ds.IntMap"] = haxe_ds_IntMap;
haxe_ds_IntMap.__name__ = ["haxe","ds","IntMap"];
haxe_ds_IntMap.__interfaces__ = [haxe_IMap];
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
	console.log("NativeEditorHandler");
};
ursine_editor_menus_FileMenu.doOpen = function() {
	console.log("open");
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
		console.log(component.meta);
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
	,getComponentType: function(name) {
		return this.m_db.get(name);
	}
	,createComponentInspector: function(entity,inspection) {
		var handler = this.m_componentInspectionHandlers.get(inspection.type);
		if(handler == null) handler = ursine_editor_scene_component_inspectors_components_DefaultComponentInspector;
		return Type.createInstance(handler,[entity,inspection]);
	}
	,createFieldInspector: function(owner,instance,field,type) {
		var handler = this.m_fieldInspectionHandlers.get(type.name);
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
	this.fieldChangeEvents = new ursine_utils_EventManager();
	this.inspector = new ComponentInspectorControl();
	this.inspector.heading = component.type;
};
$hxClasses["ursine.editor.scene.component.inspectors.ComponentInspectionHandler"] = ursine_editor_scene_component_inspectors_ComponentInspectionHandler;
ursine_editor_scene_component_inspectors_ComponentInspectionHandler.__name__ = ["ursine","editor","scene","component","inspectors","ComponentInspectionHandler"];
ursine_editor_scene_component_inspectors_ComponentInspectionHandler.prototype = {
	updateField: function(name,value) {
		this.m_component.value[name] = value;
		this.fieldChangeEvents.trigger(name,{ name : name, value : value});
	}
	,addField: function(field) {
		this.inspector.fieldInspectors.appendChild(field.inspector);
	}
	,notifyChanged: function(field,value) {
		this.m_entity.updateComponentField(this.m_component.type,field.name,value);
	}
	,remove: function() {
		this.inspector.parentNode.removeChild(this.inspector);
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
	remove: function() {
		this.inspector.parentNode.removeChild(this.inspector);
	}
};
var ursine_editor_scene_component_inspectors_components_DefaultComponentInspector = function(entity,component) {
	ursine_editor_scene_component_inspectors_ComponentInspectionHandler.call(this,entity,component);
	var database = ursine_editor_Editor.instance.componentDatabase;
	var componentType = database.getComponentType(component.type);
	var fields = Reflect.fields(componentType.fields);
	var _g = 0;
	while(_g < fields.length) {
		var name = fields[_g];
		++_g;
		var field = Reflect.field(componentType.fields,name);
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
	this.addField(database.createFieldInspector(this,Reflect.field(this.m_component.value,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName),Reflect.field(this.m_componentType.fields,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName),ursine_editor_Editor.instance.componentDatabase.getNativeType(ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeName)));
	this.setType(Reflect.field(component.value,ursine_editor_scene_component_inspectors_components_LightInspector.m_lightTypeFieldName));
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
			var name = fields[_g];
			++_g;
			var field = Reflect.field(componentType.fields,name);
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
		var v = ["Color","Direction"];
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
var ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.BooleanFieldInspector"] = ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector;
ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","BooleanFieldInspector"];
ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
});
var ursine_editor_scene_component_inspectors_fields_ColorFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	var fields = Reflect.fields(type.fields);
	var _g = 0;
	while(_g < fields.length) {
		var name = fields[_g];
		++_g;
		var colorField = Reflect.field(type.fields,name);
		this.createColorField(colorField);
	}
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.ColorFieldInspector"] = ursine_editor_scene_component_inspectors_fields_ColorFieldInspector;
ursine_editor_scene_component_inspectors_fields_ColorFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","ColorFieldInspector"];
ursine_editor_scene_component_inspectors_fields_ColorFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_ColorFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	createColorField: function(field) {
		var _g = this;
		var number = new NumberInputControl();
		number.step = "1";
		number.min = "0";
		number.max = "255";
		number.value = Std.string(Reflect.field(this.m_instance,field.name) * 255);
		number.addEventListener("input",function() {
			_g.m_instance[field.name] = number.valueAsNumber / 255.0;
			_g.m_owner.notifyChanged(_g.m_field,_g.m_instance);
		});
		this.inspector.container.appendChild(number);
	}
});
var ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	if(type.enumValue != null) this.initEnum();
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.DefaultFieldInspector"] = ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector;
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","DefaultFieldInspector"];
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	initEnum: function() {
		var _g = this;
		var combo = new ComboInputControl();
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
			combo.appendChild(option);
		}
		combo.value = this.m_instance;
		combo.addEventListener("change",function(e) {
			_g.m_instance = Std.parseInt(combo.value);
			_g.m_owner.notifyChanged(_g.m_field,_g.m_instance);
		});
		this.inspector.container.appendChild(combo);
	}
});
var ursine_editor_scene_component_inspectors_fields_NumberFieldInspector = function(owner,instance,field,type) {
	var _g = this;
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	var number = new NumberInputControl();
	number.value = this.m_instance;
	number.addEventListener("input",function() {
		var number1 = number.valueAsNumber;
		if(isNaN(number1)) number1 = 0;
		if(_g.m_type.name == "float" || _g.m_type.name == "double") _g.m_instance = number1; else _g.m_instance = number1 | 0;
		_g.m_owner.notifyChanged(_g.m_field,_g.m_instance);
	});
	number.addEventListener("focus",function(e) {
		number.select();
		e.preventDefault();
	});
	this.inspector.container.appendChild(number);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.NumberFieldInspector"] = ursine_editor_scene_component_inspectors_fields_NumberFieldInspector;
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","NumberFieldInspector"];
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
});
var ursine_editor_scene_component_inspectors_fields_StringFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.StringFieldInspector"] = ursine_editor_scene_component_inspectors_fields_StringFieldInspector;
ursine_editor_scene_component_inspectors_fields_StringFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","StringFieldInspector"];
ursine_editor_scene_component_inspectors_fields_StringFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_StringFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
});
var ursine_editor_scene_component_inspectors_fields_VectorFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
	var fields = Reflect.fields(type.fields);
	var _g = 0;
	while(_g < fields.length) {
		var name = fields[_g];
		++_g;
		var vectorField = Reflect.field(type.fields,name);
		this.createVectorField(vectorField);
	}
};
$hxClasses["ursine.editor.scene.component.inspectors.fields.VectorFieldInspector"] = ursine_editor_scene_component_inspectors_fields_VectorFieldInspector;
ursine_editor_scene_component_inspectors_fields_VectorFieldInspector.__name__ = ["ursine","editor","scene","component","inspectors","fields","VectorFieldInspector"];
ursine_editor_scene_component_inspectors_fields_VectorFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_VectorFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
	createVectorField: function(field) {
		var _g = this;
		var number = new NumberInputControl();
		number.step = "0.1";
		number.value = Reflect.field(this.m_instance,field.name);
		number.addEventListener("input",function() {
			var number1 = number.valueAsNumber;
			if(isNaN(number1)) number1 = 0;
			_g.m_instance[field.name] = number1;
			_g.m_owner.notifyChanged(_g.m_field,_g.m_instance);
		});
		number.addEventListener("focus",function(e) {
			number.select();
			e.preventDefault();
		});
		this.inspector.container.appendChild(number);
	}
});
var ursine_utils_IEventContainer = function() { };
$hxClasses["ursine.utils.IEventContainer"] = ursine_utils_IEventContainer;
ursine_utils_IEventContainer.__name__ = ["ursine","utils","IEventContainer"];
var ursine_editor_scene_entity_Entity = function(uniqueID) {
	this.events = new ursine_utils_EventManager();
	this.uniqueID = uniqueID;
	this.m_handler = new EntityHandler(uniqueID);
	ursine_editor_Editor.instance.broadcastManager.getChannel("EntityManager").on(ursine_editor_scene_entity_EntityEvent.ComponentChanged,$bind(this,this.onComponentChanged));
};
$hxClasses["ursine.editor.scene.entity.Entity"] = ursine_editor_scene_entity_Entity;
ursine_editor_scene_entity_Entity.__name__ = ["ursine","editor","scene","entity","Entity"];
ursine_editor_scene_entity_Entity.__interfaces__ = [ursine_utils_IEventContainer];
ursine_editor_scene_entity_Entity.prototype = {
	getName: function() {
		return this.m_handler.getName();
	}
	,inspect: function() {
		return this.m_handler.inspect();
	}
	,updateComponentField: function(componentName,fieldName,value) {
		this.m_handler.updateComponentField(componentName,fieldName,value);
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
	this.window.heading = "Inspector";
	this.window.style.top = "0";
	this.window.style.bottom = "50%";
	this.window.style.left = "0";
	this.window.style.width = "15%";
	this.window.style.borderBottom = "none";
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
	,onInspectedEntityComponentChanged: function(e) {
		this.m_componentHandlers.get(e.component).updateField(e.field,e.value);
	}
	,clearOldInspection: function() {
		if(this.m_inspectedEntity != null) this.m_inspectedEntity.events.off(ursine_editor_scene_entity_EntityEvent.ComponentChanged,$bind(this,this.onInspectedEntityComponentChanged));
		var $it0 = this.m_componentHandlers.iterator();
		while( $it0.hasNext() ) {
			var handler = $it0.next();
			this.window.container.removeChild(handler.inspector);
		}
		this.m_componentHandlers = new haxe_ds_StringMap();
	}
	,initializeInspection: function() {
		this.m_inspectedEntity.events.on(ursine_editor_scene_entity_EntityEvent.ComponentChanged,$bind(this,this.onInspectedEntityComponentChanged));
		var inspection = this.m_inspectedEntity.inspect();
		var database = ursine_editor_Editor.instance.componentDatabase;
		var _g = 0;
		while(_g < inspection.length) {
			var component = inspection[_g];
			++_g;
			var type = database.getComponentType(component.type);
			if(Object.prototype.hasOwnProperty.call(type.meta,ursine_native_Property.HiddenInInspector)) continue;
			var handler = database.createComponentInspector(this.m_inspectedEntity,component);
			{
				this.m_componentHandlers.set(component.type,handler);
				handler;
			}
			this.window.container.appendChild(handler.inspector);
		}
	}
});
var ursine_editor_windows_SceneOutline = function() {
	this.m_selectedItem = null;
	ursine_editor_WindowHandler.call(this);
	this.window.heading = "Outline";
	this.window.style.top = "50%";
	this.window.style.bottom = "0";
	this.window.style.left = "0";
	this.window.style.width = "15%";
	this.m_entityList = window.document.createElement("ul");
	this.m_entityList.classList.add("entity-list");
	this.window.container.appendChild(this.m_entityList);
	this.initScene();
	ursine_editor_Editor.instance.broadcastManager.getChannel("EntityManager").on("EntityAdded",$bind(this,this.onEntityAdded));
};
$hxClasses["ursine.editor.windows.SceneOutline"] = ursine_editor_windows_SceneOutline;
ursine_editor_windows_SceneOutline.__name__ = ["ursine","editor","windows","SceneOutline"];
ursine_editor_windows_SceneOutline.__super__ = ursine_editor_WindowHandler;
ursine_editor_windows_SceneOutline.prototype = $extend(ursine_editor_WindowHandler.prototype,{
	initScene: function() {
		var entities = ursine_native_Extern.SceneGetActiveEntities();
		var event = { uniqueID : 0};
		var _g = 0;
		while(_g < entities.length) {
			var uniqueID = entities[_g];
			++_g;
			event.uniqueID = uniqueID;
			this.onEntityAdded(event);
		}
	}
	,onEntityAdded: function(e) {
		var entity = new ursine_editor_scene_entity_Entity(e.uniqueID);
		var item = this.createEntityItem(entity);
		this.m_entityList.appendChild(item);
	}
	,createEntityItem: function(entity) {
		var _g = this;
		var item = window.document.createElement("li");
		item.innerText = entity.getName();
		item.entity = entity;
		item.addEventListener("click",function(e) {
			_g.selectEntity(item);
		});
		return item;
	}
	,selectEntity: function(item) {
		if(this.m_selectedItem != null) this.m_selectedItem.classList.remove("selected");
		this.m_selectedItem = item;
		item.classList.add("selected");
		ursine_editor_windows_EntityInspector.instance.inspect(item.entity);
	}
});
var ursine_editor_windows_SceneView = function() {
	ursine_editor_NativeCanvasWindowHandler.call(this,"SceneView");
	this.window.heading = "Scene";
	this.window.style.top = "0";
	this.window.style.bottom = "0";
	this.window.style.left = "15%";
	this.window.style.right = "0";
	this.window.style.borderLeft = "none";
};
$hxClasses["ursine.editor.windows.SceneView"] = ursine_editor_windows_SceneView;
ursine_editor_windows_SceneView.__name__ = ["ursine","editor","windows","SceneView"];
ursine_editor_windows_SceneView.__super__ = ursine_editor_NativeCanvasWindowHandler;
ursine_editor_windows_SceneView.prototype = $extend(ursine_editor_NativeCanvasWindowHandler.prototype,{
});
var ursine_native_Extern = function() { };
$hxClasses["ursine.native.Extern"] = ursine_native_Extern;
ursine_native_Extern.__name__ = ["ursine","native","Extern"];
ursine_native_Extern.DebugEditorUI = function() {
	return DebugEditorUI();
};
ursine_native_Extern.GetNativeComponentDatabase = function() {
	return GetNativeComponentDatabase();
};
ursine_native_Extern.SceneGetActiveEntities = function() {
	return SceneGetActiveEntities();
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
var $_, $fid = 0;
function $bind(o,m) { if( m == null ) return null; if( m.__id__ == null ) m.__id__ = $fid++; var f; if( o.hx__closures__ == null ) o.hx__closures__ = {}; else f = o.hx__closures__[m.__id__]; if( f == null ) { f = function(){ return f.method.apply(f.scope, arguments); }; f.scope = o; f.method = m; o.hx__closures__[m.__id__] = f; } return f; }
$hxClasses.Math = Math;
String.__name__ = ["String"];
$hxClasses.Array = Array;
Array.__name__ = ["Array"];
var __map_reserved = {}
ursine_editor_NativeCanvasWindowHandler.m_forwardedEvents = ["focus","blur","mouseover","mouseout"];
ursine_editor_menus_DebugMenu.__meta__ = { statics : { doEditorReload : { mainMenuItem : ["Debug/Editor UI/Reload"]}, doEditorDebugTools : { mainMenuItem : ["Debug/Editor UI/Inspect"]}}};
ursine_editor_menus_EditMenu.__meta__ = { obj : { menuIndex : [1]}, statics : { doUndo : { mainMenuItem : ["Edit/Undo"]}, doRedo : { mainMenuItem : ["Edit/Redo"]}}};
ursine_editor_menus_EntityMenu.__meta__ = { obj : { menuIndex : [2]}, statics : { doCreateEmpty : { mainMenuItem : ["Entity/Create/Empty"]}, doCreatePlane : { mainMenuItem : ["Entity/Create/Plane",true]}, doCreateBox : { mainMenuItem : ["Entity/Create/Box"]}, doCreateCylinder : { mainMenuItem : ["Entity/Create/Cylinder"]}, doCreateSphere : { mainMenuItem : ["Entity/Create/Sphere"]}, doCreatePointLight : { mainMenuItem : ["Entity/Create/Point Light",true]}, doCreateSpotLight : { mainMenuItem : ["Entity/Create/Spot Light"]}, doCreateDirectionalLight : { mainMenuItem : ["Entity/Create/Directional Light"]}}};
ursine_editor_menus_FileMenu.__meta__ = { obj : { menuIndex : [0]}, statics : { doNew : { mainMenuItem : ["File/New"]}, doOpen : { mainMenuItem : ["File/Open"]}}};
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
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.__meta__ = { obj : { fieldInspector : ["int","float","double"]}};
ursine_editor_scene_component_inspectors_fields_StringFieldInspector.__meta__ = { obj : { fieldInspector : ["std::string"]}};
ursine_editor_scene_component_inspectors_fields_VectorFieldInspector.__meta__ = { obj : { fieldInspector : ["ursine::Vec2","ursine::Vec3","ursine::SVec3","ursine::Vec4","ursine::SVec4"]}};
ursine_editor_scene_entity_EntityEvent.ComponentChanged = "ComponentChanged";
ursine_native_Property.HiddenInInspector = "HiddenInInspector";
Application.main();
})();
