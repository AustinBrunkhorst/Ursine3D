(function (console) { "use strict";
function $extend(from, fields) {
	function Inherit() {} Inherit.prototype = from; var proto = new Inherit();
	for (var name in fields) proto[name] = fields[name];
	if( fields.toString !== Object.prototype.toString ) proto.toString = fields.toString;
	return proto;
}
var MenuWrapper = function(text,children) {
	this.text = text;
	this.children = children;
};
var Application = function() { };
Application.buildMenus = function(menus) {
	var menu = new MenuControl();
	menu.classList.add("main-menu");
	var _g = 0;
	while(_g < menus.length) {
		var item = menus[_g];
		++_g;
		Application.buildMenu(item,menu);
	}
	window.document.querySelector("#header-toolbar").appendChild(menu);
};
Application.buildMenu = function(menu,parent) {
	var item = new MenuItemControl();
	item.text = menu.text;
	parent.appendChild(item);
	var _g = 0;
	var _g1 = menu.children;
	while(_g < _g1.length) {
		var child = _g1[_g];
		++_g;
		Application.buildMenu(child,item.menu);
	}
};
Application.main = function() {
	var editor = new ursine_Editor();
	var menus = [new MenuWrapper("One",[new MenuWrapper("One One",[]),new MenuWrapper("One Two",[]),new MenuWrapper("One Three",[new MenuWrapper("One Three One",[]),new MenuWrapper("One Three Two",[]),new MenuWrapper("One Three Three",[]),new MenuWrapper("One Three Four",[])]),new MenuWrapper("One Four",[])]),new MenuWrapper("Two",[]),new MenuWrapper("Three",[])];
	Application.buildMenus(menus);
};
var ursine_Editor = function() {
	ursine_Editor.instance = this;
	this.mainMenu = new ursine_menu_MainMenu();
};
var ursine_IElementContainer = function() { };
var ursine_menu_Menu = function() {
};
ursine_menu_Menu.__interfaces__ = [ursine_IElementContainer];
var ursine_menu_MainMenu = function() {
	ursine_menu_Menu.call(this);
};
ursine_menu_MainMenu.__super__ = ursine_menu_Menu;
ursine_menu_MainMenu.prototype = $extend(ursine_menu_Menu.prototype,{
});
Application.main();
})(typeof console != "undefined" ? console : {log:function(){}});
