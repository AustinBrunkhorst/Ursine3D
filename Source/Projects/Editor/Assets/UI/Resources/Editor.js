(function () { "use strict";
function $extend(from, fields) {
	function Inherit() {} Inherit.prototype = from; var proto = new Inherit();
	for (var name in fields) proto[name] = fields[name];
	if( fields.toString !== Object.prototype.toString ) proto.toString = fields.toString;
	return proto;
}
var Application = function() { };
Application.__name__ = true;
Application.main = function() {
	var mainDockContainer = window.document.body.querySelector("#main-dock-container");
	var mainDock = new DockContainerControl();
	mainDock.style.width = "100%";
	mainDock.style.height = "100%";
	mainDockContainer.appendChild(mainDock);
	var leftColumn = mainDock.addColumn();
	leftColumn.setWidthPercent(0.20);
	var rows = [leftColumn.addRow(),leftColumn.addRow()];
	var _g = 0;
	while(_g < rows.length) {
		var row = rows[_g];
		++_g;
		row.addColumn().setWidthPercent(1.0);
		row.setHeightPercent(0.5);
	}
	var inspector = new EditorWindowControl();
	inspector.heading = "Inspector";
	rows[0].columns[0].appendChild(inspector);
	var outline = new EditorWindowControl();
	outline.heading = "Outline";
	rows[1].columns[0].appendChild(outline);
	var middleColumn = mainDock.addColumn();
	middleColumn.setWidthPercent(0.40);
	var rows1 = [middleColumn.addRow(),middleColumn.addRow()];
	var _g1 = 0;
	while(_g1 < rows1.length) {
		var row1 = rows1[_g1];
		++_g1;
		row1.addColumn();
		row1.setHeightPercent(0.5);
	}
	var content = new EditorWindowControl();
	content.heading = "Asset Browser";
	rows1[0].columns[0].setWidthPercent(1.0);
	rows1[0].columns[0].appendChild(content);
	var material = new EditorWindowControl();
	material.heading = "Material Editor";
	rows1[1].columns[0].setWidthPercent(0.5);
	rows1[1].columns[0].appendChild(material);
	var row2col2 = rows1[1].addColumn();
	var particle = new EditorWindowControl();
	particle.heading = "Particle Editor";
	row2col2.setWidthPercent(0.5);
	row2col2.appendChild(particle);
	var rightColumn = mainDock.addColumn();
	rightColumn.setWidthPercent(0.40);
	var scene = new EditorWindowControl();
	scene.heading = "Scene";
	rightColumn.addRow().appendChild(scene);
};
var HxOverrides = function() { };
HxOverrides.__name__ = true;
HxOverrides.cca = function(s,index) {
	var x = s.charCodeAt(index);
	if(x != x) return undefined;
	return x;
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
Std.parseInt = function(x) {
	var v = parseInt(x,10);
	if(v == 0 && (HxOverrides.cca(x,1) == 120 || HxOverrides.cca(x,1) == 88)) v = parseInt(x);
	if(isNaN(v)) return null;
	return v;
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
js__$Boot_HaxeError.__name__ = true;
js__$Boot_HaxeError.__super__ = Error;
js__$Boot_HaxeError.prototype = $extend(Error.prototype,{
});
var js_Boot = function() { };
js_Boot.__name__ = true;
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
var ursine_editor_Editor = function() { };
ursine_editor_Editor.__name__ = true;
var ursine_editor_MenuItemHandler = function() { };
ursine_editor_MenuItemHandler.__name__ = true;
var ursine_editor_menus_DebugMenu = function() { };
ursine_editor_menus_DebugMenu.__name__ = true;
ursine_editor_menus_DebugMenu.doEditorReload = function() {
};
ursine_editor_menus_DebugMenu.doEditorDebugTools = function() {
	ursine_native_Extern.DebugEditorUI();
};
ursine_editor_menus_DebugMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_DebugMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
});
var ursine_editor_menus_EditMenu = function() { };
ursine_editor_menus_EditMenu.__name__ = true;
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
ursine_editor_menus_EntityMenu.__name__ = true;
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
ursine_editor_menus_FileMenu.__name__ = true;
ursine_editor_menus_FileMenu.doNew = function() {
	console.log("NativeEditorHandler");
};
ursine_editor_menus_FileMenu.doOpen = function() {
	console.log("open");
};
ursine_editor_menus_FileMenu.__super__ = ursine_editor_MenuItemHandler;
ursine_editor_menus_FileMenu.prototype = $extend(ursine_editor_MenuItemHandler.prototype,{
});
var ursine_editor_scene_component_ComponentDatabase = function() { };
ursine_editor_scene_component_ComponentDatabase.__name__ = true;
ursine_editor_scene_component_ComponentDatabase.prototype = {
	getNativeType: function(name) {
		return this.m_typeDB.get(name);
	}
	,getComponentType: function(name) {
		return this.m_db.get(name);
	}
	,createFieldInspector: function(owner,instance,field,type) {
		var handler = this.m_fieldInspectionHandlers.get(type.name);
		if(handler == null) handler = ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector;
		return Type.createInstance(handler,[owner,instance,field,type]);
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
ursine_editor_scene_component_inspectors_ComponentInspectionHandler.__name__ = true;
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
ursine_editor_scene_component_inspectors_FieldInspectionHandler.__name__ = true;
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
ursine_editor_scene_component_inspectors_components_DefaultComponentInspector.__name__ = true;
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
ursine_editor_scene_component_inspectors_components_LightInspector.__name__ = true;
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
ursine_editor_scene_component_inspectors_fields_BooleanFieldInspector.__name__ = true;
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
ursine_editor_scene_component_inspectors_fields_ColorFieldInspector.__name__ = true;
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
ursine_editor_scene_component_inspectors_fields_DefaultFieldInspector.__name__ = true;
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
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.__name__ = true;
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.__super__ = ursine_editor_scene_component_inspectors_FieldInspectionHandler;
ursine_editor_scene_component_inspectors_fields_NumberFieldInspector.prototype = $extend(ursine_editor_scene_component_inspectors_FieldInspectionHandler.prototype,{
});
var ursine_editor_scene_component_inspectors_fields_StringFieldInspector = function(owner,instance,field,type) {
	ursine_editor_scene_component_inspectors_FieldInspectionHandler.call(this,owner,instance,field,type);
};
ursine_editor_scene_component_inspectors_fields_StringFieldInspector.__name__ = true;
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
ursine_editor_scene_component_inspectors_fields_VectorFieldInspector.__name__ = true;
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
ursine_utils_IEventContainer.__name__ = true;
var ursine_editor_scene_entity_Entity = function() { };
ursine_editor_scene_entity_Entity.__name__ = true;
ursine_editor_scene_entity_Entity.__interfaces__ = [ursine_utils_IEventContainer];
ursine_editor_scene_entity_Entity.prototype = {
	updateComponentField: function(componentName,fieldName,value) {
		this.m_handler.updateComponentField(componentName,fieldName,value);
	}
};
var ursine_native_Extern = function() { };
ursine_native_Extern.__name__ = true;
ursine_native_Extern.DebugEditorUI = function() {
	return DebugEditorUI();
};
var ursine_utils_EventManager = function() {
	this.m_events = new haxe_ds_StringMap();
};
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
var $_, $fid = 0;
function $bind(o,m) { if( m == null ) return null; if( m.__id__ == null ) m.__id__ = $fid++; var f; if( o.hx__closures__ == null ) o.hx__closures__ = {}; else f = o.hx__closures__[m.__id__]; if( f == null ) { f = function(){ return f.method.apply(f.scope, arguments); }; f.scope = o; f.method = m; o.hx__closures__[m.__id__] = f; } return f; }
String.__name__ = true;
Array.__name__ = true;
var __map_reserved = {}
ursine_editor_menus_DebugMenu.__meta__ = { statics : { doEditorReload : { mainMenuItem : ["Debug/Editor UI/Reload"]}, doEditorDebugTools : { mainMenuItem : ["Debug/Editor UI/Inspect"]}}};
ursine_editor_menus_EditMenu.__meta__ = { obj : { menuIndex : [1]}, statics : { doUndo : { mainMenuItem : ["Edit/Undo"]}, doRedo : { mainMenuItem : ["Edit/Redo"]}}};
ursine_editor_menus_EntityMenu.__meta__ = { obj : { menuIndex : [2]}, statics : { doCreateEmpty : { mainMenuItem : ["Entity/Create/Empty"]}, doCreatePlane : { mainMenuItem : ["Entity/Create/Plane",true]}, doCreateBox : { mainMenuItem : ["Entity/Create/Box"]}, doCreateCylinder : { mainMenuItem : ["Entity/Create/Cylinder"]}, doCreateSphere : { mainMenuItem : ["Entity/Create/Sphere"]}, doCreatePointLight : { mainMenuItem : ["Entity/Create/Point Light",true]}, doCreateSpotLight : { mainMenuItem : ["Entity/Create/Spot Light"]}, doCreateDirectionalLight : { mainMenuItem : ["Entity/Create/Directional Light"]}}};
ursine_editor_menus_FileMenu.__meta__ = { obj : { menuIndex : [0]}, statics : { doNew : { mainMenuItem : ["File/New"]}, doOpen : { mainMenuItem : ["File/Open"]}}};
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
Application.main();
})();
