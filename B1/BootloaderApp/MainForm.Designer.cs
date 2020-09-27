namespace BooloaderApp
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.COM_comboBox = new System.Windows.Forms.ComboBox();
            this.FileSelect_button = new System.Windows.Forms.Button();
            this.Connect_button = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.FileName_textBox = new System.Windows.Forms.TextBox();
            this.Start_button = new System.Windows.Forms.Button();
            this.Reset_button = new System.Windows.Forms.Button();
            this.Program_button = new System.Windows.Forms.Button();
            this.BinFile_openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.FlashProgrammingProgressBar = new System.Windows.Forms.ProgressBar();
            this.SuspendLayout();
            // 
            // COM_comboBox
            // 
            this.COM_comboBox.FormattingEnabled = true;
            this.COM_comboBox.Location = new System.Drawing.Point(93, 12);
            this.COM_comboBox.Name = "COM_comboBox";
            this.COM_comboBox.Size = new System.Drawing.Size(121, 21);
            this.COM_comboBox.TabIndex = 0;
            // 
            // FileSelect_button
            // 
            this.FileSelect_button.Location = new System.Drawing.Point(37, 55);
            this.FileSelect_button.Name = "FileSelect_button";
            this.FileSelect_button.Size = new System.Drawing.Size(121, 23);
            this.FileSelect_button.TabIndex = 1;
            this.FileSelect_button.Text = "Bin file Select...";
            this.FileSelect_button.UseVisualStyleBackColor = true;
            this.FileSelect_button.Click += new System.EventHandler(this.FileSelect_button_Click);
            // 
            // Connect_button
            // 
            this.Connect_button.Location = new System.Drawing.Point(229, 12);
            this.Connect_button.Name = "Connect_button";
            this.Connect_button.Size = new System.Drawing.Size(75, 23);
            this.Connect_button.TabIndex = 2;
            this.Connect_button.Text = "Connect";
            this.Connect_button.UseVisualStyleBackColor = true;
            this.Connect_button.Click += new System.EventHandler(this.Connect_button_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(34, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Com Port:";
            // 
            // FileName_textBox
            // 
            this.FileName_textBox.Location = new System.Drawing.Point(37, 84);
            this.FileName_textBox.Name = "FileName_textBox";
            this.FileName_textBox.Size = new System.Drawing.Size(267, 20);
            this.FileName_textBox.TabIndex = 4;
            this.FileName_textBox.TextChanged += new System.EventHandler(this.FileName_textBox_TextChanged);
            // 
            // Start_button
            // 
            this.Start_button.Location = new System.Drawing.Point(148, 130);
            this.Start_button.Name = "Start_button";
            this.Start_button.Size = new System.Drawing.Size(75, 23);
            this.Start_button.TabIndex = 5;
            this.Start_button.Text = "Start";
            this.Start_button.UseVisualStyleBackColor = true;
            this.Start_button.Click += new System.EventHandler(this.Start_button_Click);
            // 
            // Reset_button
            // 
            this.Reset_button.Location = new System.Drawing.Point(229, 130);
            this.Reset_button.Name = "Reset_button";
            this.Reset_button.Size = new System.Drawing.Size(75, 23);
            this.Reset_button.TabIndex = 6;
            this.Reset_button.Text = "Reset";
            this.Reset_button.UseVisualStyleBackColor = true;
            this.Reset_button.Click += new System.EventHandler(this.Reset_button_Click);
            // 
            // Program_button
            // 
            this.Program_button.Location = new System.Drawing.Point(37, 130);
            this.Program_button.Name = "Program_button";
            this.Program_button.Size = new System.Drawing.Size(75, 23);
            this.Program_button.TabIndex = 7;
            this.Program_button.Text = "Program";
            this.Program_button.UseVisualStyleBackColor = true;
            this.Program_button.Click += new System.EventHandler(this.Program_button_Click);
            // 
            // FlashProgrammingProgressBar
            // 
            this.FlashProgrammingProgressBar.Location = new System.Drawing.Point(37, 159);
            this.FlashProgrammingProgressBar.Name = "FlashProgrammingProgressBar";
            this.FlashProgrammingProgressBar.Size = new System.Drawing.Size(267, 23);
            this.FlashProgrammingProgressBar.TabIndex = 8;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(338, 187);
            this.Controls.Add(this.FlashProgrammingProgressBar);
            this.Controls.Add(this.Program_button);
            this.Controls.Add(this.Reset_button);
            this.Controls.Add(this.Start_button);
            this.Controls.Add(this.FileName_textBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.Connect_button);
            this.Controls.Add(this.FileSelect_button);
            this.Controls.Add(this.COM_comboBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "BootloaderApp";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox COM_comboBox;
        private System.Windows.Forms.Button FileSelect_button;
        private System.Windows.Forms.Button Connect_button;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox FileName_textBox;
        private System.Windows.Forms.Button Start_button;
        private System.Windows.Forms.Button Reset_button;
        private System.Windows.Forms.Button Program_button;
        private System.Windows.Forms.OpenFileDialog BinFile_openFileDialog;
        private System.Windows.Forms.ProgressBar FlashProgrammingProgressBar;
    }
}

