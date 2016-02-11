using System.Collections;
using System.Windows.Forms;
using ProjectGenerator.Classes;

namespace ProjectGenerator.Forms
{
    public sealed partial class ProjectForm : Form
    {
        private static readonly CMakeGenerator[] m_generators =
        {
            new CMakeGenerator( "Visual Studio 10 2010", "VS2010" ),
            new CMakeGenerator( "Visual Studio 11 2012", "VS2012" ),
            new CMakeGenerator( "Visual Studio 12 2013", "VS2013" ),
            new CMakeGenerator( "Visual Studio 14 2015", "VS2015" ),
            new CMakeGenerator( "Visual Studio 10 2010 Win64", "VS2010-x64" ),
            new CMakeGenerator( "Visual Studio 11 2012 Win64", "VS2012-x64" ),
            new CMakeGenerator( "Visual Studio 12 2013 Win64", "VS2013-x64" ),
            new CMakeGenerator( "Visual Studio 14 2015 Win64", "VS2015-x64" ),
            new CMakeGenerator( "Unix Makefiles", "UnixMake" )
        };

        private CMakeProject m_project;

        public ProjectForm(CMakeProject project)
        {
            InitializeComponent( );

            generatorList.DataSource = m_generators;

            // default to VS 2015, 64 bit
            generatorList.SelectedIndex = 7;

            m_project = project;

            Text = project.DisplayName;
        }

        private void btnCancelClick(object sender, System.EventArgs e)
        {
            Close( );
        }

        private void btnGenerateClick(object sender, System.EventArgs e)
        {
            var generator = (CMakeGenerator)generatorList.SelectedItem;

            var form = new GenerationForm( 
                m_project, 
                generator, 
                shouldCleanBuild.Checked
            );

            form.ShowDialog( );
        }
    }
}
