var fs = require( 'fs' );
var glob = require( 'glob' );
var dot = require( 'dot' );
var exec = require( 'child_process' ).exec;

dot.templateSettings.strip = false;
dot.templateSettings.varname = 'file';

var templates = dot.process( { path: './templates' });

var options = {
	cwd: 'C:/Game Development/Ursine3D/Source/Projects/',
	matchBase: true
};

var authorTable = {
	'Austin Brunkhorst': {
		name: 'Austin Brunkhorst',
		email: 'a.brunkhorst@digipen.edu'
	},
	'Joshua Shlemmer': {
		name: 'Joshua Shlemmer',
		email: 'joshua.shlemmer@digipen.edu'
	},
	'Jason Burch': {
		name: 'Jason Burch',
		email: 'j.orion@digipen.edu'
	},
	'artainex': {
		name: 'Park Hyung Jun',
		email: 'park.hyungjun@digipen.edu'
	},
	'Matt Yan': {
		name: 'Matt Yan',
		email: 'm.yan@digipen.edu'
	},
	'CJsion3': {
		name: 'CJ Payne',
		email: 'cj.payne@digipen.edu'
	},
	'Jordan Ellis': {
		name: 'Jordan Ellis',
		email: 'j.ellis@digipen.edu'
	}
};

process.chdir( options.cwd );

glob( "{*.h,*.cpp}", options, function(er, files) {
	files.forEach( function(file) { 
		var filename = file.replace( /^.*[\\\/]/, '' );

		var contents = fs.readFileSync( file, 'utf8' );

		var trimmed = contents.trim( );

		if (trimmed.indexOf( '/* -' ) == -1) {
			getAuthor( file, function(author) {
				var header = templates.header( {
					filename: filename,
					author: author
				} );

				fs.writeFile( file, header + contents, function(error) {

				} );
			} );
		}
	} );
} );


function getAuthor(file, callback) {
	var blameCommand = 'git --no-pager log -1 --first-parent --pretty=format:"%an" "'+ file +'"';

	exec( blameCommand, function(error, username, stderr) {
		var author = authorTable[ username ];

		if (!author) {
			var keys = Object.keys( authorTable );

			author = authorTable[ keys[ Math.random( ) * (keys.length - 1) ] ];
		}

		callback( author );
	} );
}