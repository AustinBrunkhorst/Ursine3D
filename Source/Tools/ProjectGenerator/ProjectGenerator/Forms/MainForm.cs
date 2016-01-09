using System;
using System.Collections;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using ProjectGenerator.Classes;

namespace ProjectGenerator.Forms
{
    public partial class MainForm : Form
    {
        private readonly Brush brushBGEven;
        private readonly Brush brushBGOdd;

        public MainForm()
        {
            InitializeComponent( );

            brushBGEven = Brushes.White;
            brushBGOdd = new SolidBrush( Color.FromArgb( 243, 243, 243 ) );

            projectList.DrawItem += drawProjectItem;
            projectList.MouseDoubleClick += onProjectDoubleClick;

            reloadProjects( );
        }

        private void onProjectDoubleClick(object sender, MouseEventArgs e)
        {
            var index = projectList.IndexFromPoint( e.Location );

            if (index == ListBox.NoMatches)
                return;

            var project = (CMakeProject)projectList.Items[ index ];

            var form = new ProjectForm( project );

            form.ShowDialog( );
        }

        private void reloadProjects()
        {
            var projectFiles = Directory.GetFiles( 
                CMakeProject.SourceDirectory, 
                "CMakeLists.txt",
                SearchOption.AllDirectories
            );

            var projects = new ArrayList( );

            foreach (var path in projectFiles)
            {
                var project = new CMakeProject( path );

                projects.Add( project );
            }

            projectList.DataSource = projects;
        }

        private void drawProjectItem(object sender, DrawItemEventArgs e)
        {
            e.Graphics.FillRectangle( 
                (e.Index & 1) == 0 ? brushBGEven : brushBGOdd, 
                e.Bounds 
            );

            var project = (CMakeProject)projectList.Items[ e.Index ];

            var directoryBrush = Brushes.Black;
            var slashBrush = Brushes.Gray;

            float slashWidth = e.Graphics.MeasureString( "/", e.Font ).Width;
            float x = e.Bounds.X;

            int i = 0;

            foreach (var directory in project.PathDirectories)
            {
                var size = e.Graphics.MeasureString( directory, e.Font );

                e.Graphics.DrawString( 
                    directory, 
                    e.Font, 
                    directoryBrush, 
                    x, 
                    e.Bounds.Y + 5 
                );

                x += size.Width;

                if (i < project.PathDirectories.Length - 1)
                {
                    e.Graphics.DrawString(
                        "/",
                        e.Font,
                        slashBrush,
                        x,
                        e.Bounds.Y + 5
                    );

                    x += slashWidth;
                }

                ++i;
            }
        }

        private void btnRefresh_Click(object sender, EventArgs e)
        {
            reloadProjects( );
        }
    }
}
