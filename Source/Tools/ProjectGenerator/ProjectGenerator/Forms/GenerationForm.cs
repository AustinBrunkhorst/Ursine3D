using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Security.AccessControl;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ProjectGenerator.Classes;

namespace ProjectGenerator.Forms
{
    public sealed partial class GenerationForm : Form
    {
        private const string kIntDirPrefix = "Build-";

        private CMakeProject m_project;

        private string m_binaryDir;
        private string m_sourceDir;
        private Process m_cmakeProcess;

        public GenerationForm(CMakeProject project, CMakeGenerator generator, bool shouldClean)
        {
            InitializeComponent( );

            m_project = project;

            Text = "Generate " + project.DisplayName;

            m_binaryDir = Path.GetFullPath( 
                Path.Combine( kIntDirPrefix + generator.IntermediateDirectory,
                    project.DisplayName
                )
            ).Replace( '\\', '/' );

            m_sourceDir = Path.GetDirectoryName( project.Path ).Replace( '\\', '/' );

            if (shouldClean)
            {
                try
                {
                    Directory.Delete( m_binaryDir, true );
                }
                catch
                {
                    // ignored
                }
            }

            if (!Directory.Exists( m_binaryDir ))
                Directory.CreateDirectory( m_binaryDir );

            var cmakeArgs = string.Format(
                "-DCMAKE_MODULE_PATH=\"{0}\" -G \"{1}\" \"{2}\"",
                CMakeProject.ModuleDirectory.Replace( '\\', '/' ),
                generator.Name.Replace( '\\', '/' ),
                m_sourceDir
            );

            m_cmakeProcess = new Process
            {
                StartInfo =
                {
                    CreateNoWindow = true,
                    RedirectStandardError = true,
                    RedirectStandardOutput = true,
                    FileName = CMakeGenerator.ExecutableName,
                    Arguments = cmakeArgs,
                    UseShellExecute = false,
                    WorkingDirectory = m_binaryDir
                },
                EnableRaisingEvents = true
            };


            m_cmakeProcess.OutputDataReceived += cmakeOnData;
            m_cmakeProcess.ErrorDataReceived += cmakeOnError;
            m_cmakeProcess.Exited += cmakeOnExit;

            m_cmakeProcess.Start( );
            m_cmakeProcess.BeginOutputReadLine( );
            m_cmakeProcess.BeginErrorReadLine( );
        }

        private void cmakeOnData(object sender, DataReceivedEventArgs e)
        {
            log( e.Data, Color.FromArgb( 92, 122, 92 ) );
        }

        private void cmakeOnError(object sender, DataReceivedEventArgs e)
        {
            log( e.Data, Color.FromArgb( 163, 68, 68 ) );
        }

        private void cmakeOnExit(object sender, EventArgs e)
        {
            if (InvokeRequired)
            {
                Invoke( new Action<object, EventArgs>( cmakeOnExit ), new object[] { sender, e } );

                return;
            }

            btnCancel.Text = "Close";

            // make bold
            btnCancel.Font = new Font(
                btnCancel.Font.Name, 
                btnCancel.Font.Size, 
                FontStyle.Bold
            );
        }

        private void log(string text, Color color)
        {
            if (InvokeRequired)
            {
                Invoke( new Action<string, Color>( log ), new object[] { text, color } );

                return;
            }

            text += Environment.NewLine;

            logText.SelectionStart = logText.TextLength;
            logText.SelectionLength = 0;

            logText.SelectionColor = color;
            logText.AppendText( text );
            logText.SelectionColor = logText.ForeColor;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            try
            {
                if (!m_cmakeProcess.HasExited)
                    m_cmakeProcess.Kill( );
            }
            catch
            {
                // ignored
            }

            Close( );
        }

        private void btnOpenExplorer_Click(object sender, EventArgs e)
        {
            Process.Start( new ProcessStartInfo( )
            {
                FileName = m_binaryDir,
                UseShellExecute = true,
                Verb = "open"
            } );
        }

        private void openCMakeGUIToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var guiArgs = string.Format( 
                "\"{0}\"",
                m_sourceDir
            );

            Process.Start( new ProcessStartInfo( )
            {
                FileName = "cmake-gui.exe",
                UseShellExecute = true,
                Arguments = guiArgs
            } );
        }
    }
}
