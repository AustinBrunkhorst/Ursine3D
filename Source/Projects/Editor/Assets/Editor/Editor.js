(function (console) { "use strict";
var Editor = function() { };
Editor.main = function() {
	console.log("what");
};
Editor.__meta__ = { obj : { include : null}};
Editor.main();
})(typeof console != "undefined" ? console : {log:function(){}});
