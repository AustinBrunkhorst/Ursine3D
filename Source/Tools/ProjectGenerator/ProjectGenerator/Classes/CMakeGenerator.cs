using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProjectGenerator.Classes
{
    public class CMakeGenerator
    {
        public static string ExecutableName { get; set; }

        public string Name { get; set; }

        public string IntermediateDirectory { get; set; }

        public CMakeGenerator(string name, string intDirectory)
        {
            Name = name;
            IntermediateDirectory = intDirectory;
        }
    }
}
