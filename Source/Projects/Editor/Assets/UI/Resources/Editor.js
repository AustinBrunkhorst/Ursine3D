(function (console) { "use strict";
function $extend(from, fields) {
	function Inherit() {} Inherit.prototype = from; var proto = new Inherit();
	for (var name in fields) proto[name] = fields[name];
	if( fields.toString !== Object.prototype.toString ) proto.toString = fields.toString;
	return proto;
}
var Application = function() { };
Application.main = function() {
	var editor = new ursine_Editor();
};
var ursine_Editor = function() {
	ursine_Editor.instance = this;
	this.mainMenu = new ursine_menu_MainMenu();
};
var ursine_IElementContainer = function() { };
var ursine_menu_Menu = function() {
	var _this = window.document;
	this.domContainer = _this.createElement("div");
	this.domContainer.classList.add("menu");
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
