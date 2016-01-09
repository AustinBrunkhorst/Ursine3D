using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProjectGenerator.Classes
{
    public class CMakeProject
    {
        /// <summary>
        /// CMake Source directory for the instance of an application
        /// </summary>
        public static string SourceDirectory { get; set; }

        /// <summary>
        /// CMake module directory for the instance of an application
        /// </summary>
        public static string ModuleDirectory { get; set; }

        /// <summary>
        /// Absolute path to the CMakeLists.txt file
        /// </summary>
        public string Path { get; private set; }

        public string[] PathDirectories;

        /// <summary>
        /// Path relative to the root directory, without the
        /// CMakeLists.txt file
        /// </summary>
        public string DisplayName { get; private set; }

        public CMakeProject(string path)
        {
            Path = path.Replace( '/', System.IO.Path.PathSeparator );

            DisplayName = path
                .Replace( SourceDirectory + "\\", "" )
                .Replace( "\\", "/")
                .Replace( "/CMakeLists.txt", "" );

            PathDirectories = DisplayName.Split( '/' ).ToArray( );
        }
    }
}
