/*******************************************************************************
* Copyright (C) 2020 Singular Software Limited.
* THIS SOFTWARE IS PROVIDED BY SIGNULAR SOFTWARE "AS IS" AND ANY EXPRESS OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BooloaderApp
{
    public partial class MainForm : Form
    {
        private readonly Bootloader _bootloader;
        private string _firmwareFileName;

        public MainForm()
        {
            InitializeComponent();

            _bootloader = new Bootloader();

            _firmwareFileName = "";

            Connect_button.Enabled = true;
            FileSelect_button.Enabled = false;
            Reset_button.Enabled = false;
            Start_button.Enabled = false;
            Program_button.Enabled = false;
            FileName_textBox.Enabled = false;

            // Get a list of serial port names.  
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                COM_comboBox.Items.Add(port);
            }

            if (COM_comboBox.Items.Count > 0)
            {
                COM_comboBox.SelectedIndex = 0;
            }
        }

        private void Connect_button_Click(object sender, EventArgs e)
        {
            Text = "Bootloader - ";

            if (_bootloader.Open(COM_comboBox.Text))
            {
                if (_bootloader.Connect())
                {
                    Text = "Bootloader - " + _bootloader.GetVersion();
                    Reset_button.Enabled = true;
                    Start_button.Enabled = true;
                    FileSelect_button.Enabled = true;
                    FileName_textBox.Enabled = true;
                    Program_button.Enabled = FileName_textBox.Text != "";
                }
            }
        }

        private void FileSelect_button_Click(object sender, EventArgs e)
        {
            if (_firmwareFileName != "")
            {
                string filePath = new FileInfo(_firmwareFileName).Directory.FullName;
                string hexFileName = new FileInfo(_firmwareFileName).Name;
                BinFile_openFileDialog.InitialDirectory = filePath;
                BinFile_openFileDialog.FileName = hexFileName;
            }
            else
            {
                BinFile_openFileDialog.InitialDirectory = Application.StartupPath;
                BinFile_openFileDialog.FileName = "";
            }
            BinFile_openFileDialog.Filter = "bin files (*.bin)|*.bin|All files (*.*)|*.*";
            BinFile_openFileDialog.FilterIndex = 1;
            BinFile_openFileDialog.RestoreDirectory = true;

            if (BinFile_openFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                _firmwareFileName = BinFile_openFileDialog.FileName;
            }
            FileName_textBox.Text = _firmwareFileName;
        }

        private void Program_button_Click(object sender, EventArgs e)
        {
            try
            {
                Connect_button.Enabled = false;
                FileSelect_button.Enabled = false;
                Program_button.Enabled = false;
                Start_button.Enabled = false;
                Reset_button.Enabled = false;
                FileName_textBox.Enabled = false;

                if (_bootloader.Connected() && _firmwareFileName != "")
                {
                    byte[] data = new byte[0];

                    data = File.ReadAllBytes(_firmwareFileName);

                    Bootloader.ResultUpdate resultHandler = ResultUpdate;
                    Bootloader.ProgressUpdate updateHandler = ProgressUpdate;


                    _bootloader.ProgramFlash(data, resultHandler, updateHandler);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
            finally
            {
                Connect_button.Enabled = true;
                FileSelect_button.Enabled = true;
                Program_button.Enabled = true;
                Start_button.Enabled = true;
                Reset_button.Enabled = true;
                FileName_textBox.Enabled = true;
            }
        }

        private void FileName_textBox_TextChanged(object sender, EventArgs e)
        {
            _firmwareFileName = FileName_textBox.Text;
            Program_button.Enabled = true;
        }

        private void Start_button_Click(object sender, EventArgs e)
        {
            if (_bootloader.Connected())
            {
                _bootloader.StartFrimware();
                Connect_button.Enabled = false;
                FileSelect_button.Enabled = false;
                Program_button.Enabled = false;
                Start_button.Enabled = false;
                Reset_button.Enabled = false;
                FileName_textBox.Enabled = false;
            }
        }

        private void Reset_button_Click(object sender, EventArgs e)
        {
            if (_bootloader.Connected())
            {
                _bootloader.ResetBoard();
                Connect_button.Enabled = false;
                FileSelect_button.Enabled = false;
                Program_button.Enabled = false;
                Start_button.Enabled = false;
                Reset_button.Enabled = false;
                FileName_textBox.Enabled = false;
            }
        }

        private delegate void ProgressDelegate(int min, int max, int val);
        
        private void ProgressUpdate(int minimum, int maximum, int value)
        {
            if (FlashProgrammingProgressBar.InvokeRequired)
            {
                var d = new ProgressDelegate(ProgressUpdate);
                FlashProgrammingProgressBar.Invoke(d, new object[] { minimum, maximum, value });
            }
            else
            {
                FlashProgrammingProgressBar.Minimum = minimum;
                FlashProgrammingProgressBar.Maximum = maximum;
                FlashProgrammingProgressBar.Value = value;
            }
        }
        
        private delegate void ResultDelegate(bool success);
        private void ResultUpdate(bool success)
        {
            if (FlashProgrammingProgressBar.InvokeRequired)
            {
                var d = new ResultDelegate(ResultUpdate);
                FlashProgrammingProgressBar.Invoke(d, new object[] { success });
            }
            else
            {
                FlashProgrammingProgressBar.Minimum = 0;
                FlashProgrammingProgressBar.Maximum = 0;
                FlashProgrammingProgressBar.Value = 0;
                if (success)
                {
                    MessageBox.Show("Programming succesfull");
                }
                else
                {
                    MessageBox.Show("Programming failed");
                }
            }
        }

    }
}
