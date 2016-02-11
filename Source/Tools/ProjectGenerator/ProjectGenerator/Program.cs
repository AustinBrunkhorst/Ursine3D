using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using ProjectGenerator.Classes;
using ProjectGenerator.Forms;

namespace ProjectGenerator
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            var sourceDir = "";
            var moduleDir = "";
            var cmakeExecutable = "cmake.exe";

            if (args.Length > 0)
                sourceDir = args[ 0 ];

            if (args.Length > 1)
                moduleDir = args[ 1 ];

            if (args.Length > 2)
                cmakeExecutable = args[ 2 ];

            CMakeProject.SourceDirectory 
                = Path.GetFullPath( sourceDir );

            CMakeProject.ModuleDirectory 
                = Path.GetFullPath( moduleDir );

            CMakeGenerator.ExecutableName = cmakeExecutable;

            Application.EnableVisualStyles( );
            Application.SetCompatibleTextRenderingDefault( false );
            Application.Run( new MainForm( ) );
        }
    }
}
