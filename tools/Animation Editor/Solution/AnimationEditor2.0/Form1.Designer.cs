namespace AnimationEditor2._0
{
    partial class Form1
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.setDirectoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainContainer = new System.Windows.Forms.SplitContainer();
            this.FrameList = new System.Windows.Forms.ListBox();
            this.AnimationList = new System.Windows.Forms.ListBox();
            this.AnimationName = new System.Windows.Forms.TextBox();
            this.RemoveFrameButton = new System.Windows.Forms.Button();
            this.RemoveAnimationButton = new System.Windows.Forms.Button();
            this.AddFrameButton = new System.Windows.Forms.Button();
            this.AddAnimationButton = new System.Windows.Forms.Button();
            this.RectDrawingSelectionPanel = new System.Windows.Forms.Panel();
            this.Seconds = new System.Windows.Forms.Label();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.AnchorPointRadioButton = new System.Windows.Forms.RadioButton();
            this.CollisionRectRadioButton = new System.Windows.Forms.RadioButton();
            this.RenderRectRadio = new System.Windows.Forms.RadioButton();
            this.PreviewPanel = new System.Windows.Forms.Panel();
            this.TriggerBox = new System.Windows.Forms.TextBox();
            this.menuStrip1.SuspendLayout();
            this.MainContainer.Panel1.SuspendLayout();
            this.MainContainer.Panel2.SuspendLayout();
            this.MainContainer.SuspendLayout();
            this.RectDrawingSelectionPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.optionsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(944, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadImageToolStripMenuItem,
            this.setDirectoryToolStripMenuItem});
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
            this.optionsToolStripMenuItem.Text = "Options";
            // 
            // loadImageToolStripMenuItem
            // 
            this.loadImageToolStripMenuItem.Name = "loadImageToolStripMenuItem";
            this.loadImageToolStripMenuItem.Size = new System.Drawing.Size(141, 22);
            this.loadImageToolStripMenuItem.Text = "Load Image";
            this.loadImageToolStripMenuItem.Click += new System.EventHandler(this.loadImageToolStripMenuItem_Click);
            // 
            // setDirectoryToolStripMenuItem
            // 
            this.setDirectoryToolStripMenuItem.Name = "setDirectoryToolStripMenuItem";
            this.setDirectoryToolStripMenuItem.Size = new System.Drawing.Size(141, 22);
            this.setDirectoryToolStripMenuItem.Text = "Set Directory";
            this.setDirectoryToolStripMenuItem.Click += new System.EventHandler(this.setDirectoryToolStripMenuItem_Click);
            // 
            // MainContainer
            // 
            this.MainContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainContainer.Location = new System.Drawing.Point(0, 24);
            this.MainContainer.Name = "MainContainer";
            // 
            // MainContainer.Panel1
            // 
            this.MainContainer.Panel1.Controls.Add(this.FrameList);
            this.MainContainer.Panel1.Controls.Add(this.AnimationList);
            this.MainContainer.Panel1.Controls.Add(this.AnimationName);
            this.MainContainer.Panel1.Controls.Add(this.RemoveFrameButton);
            this.MainContainer.Panel1.Controls.Add(this.RemoveAnimationButton);
            this.MainContainer.Panel1.Controls.Add(this.AddFrameButton);
            this.MainContainer.Panel1.Controls.Add(this.AddAnimationButton);
            this.MainContainer.Panel1.Controls.Add(this.RectDrawingSelectionPanel);
            // 
            // MainContainer.Panel2
            // 
            this.MainContainer.Panel2.Controls.Add(this.PreviewPanel);
            this.MainContainer.Panel2.Scroll += new System.Windows.Forms.ScrollEventHandler(this.MainContainer_Panel2_Scroll);
            this.MainContainer.Size = new System.Drawing.Size(944, 458);
            this.MainContainer.SplitterDistance = 289;
            this.MainContainer.TabIndex = 1;
            // 
            // FrameList
            // 
            this.FrameList.FormattingEnabled = true;
            this.FrameList.Location = new System.Drawing.Point(12, 144);
            this.FrameList.Name = "FrameList";
            this.FrameList.Size = new System.Drawing.Size(99, 69);
            this.FrameList.TabIndex = 5;
            this.FrameList.SelectedIndexChanged += new System.EventHandler(this.FrameList_SelectedIndexChanged);
            // 
            // AnimationList
            // 
            this.AnimationList.FormattingEnabled = true;
            this.AnimationList.Location = new System.Drawing.Point(12, 34);
            this.AnimationList.Name = "AnimationList";
            this.AnimationList.Size = new System.Drawing.Size(99, 69);
            this.AnimationList.TabIndex = 5;
            this.AnimationList.SelectedIndexChanged += new System.EventHandler(this.AnimationList_SelectedIndexChanged);
            // 
            // AnimationName
            // 
            this.AnimationName.Location = new System.Drawing.Point(12, 109);
            this.AnimationName.Name = "AnimationName";
            this.AnimationName.Size = new System.Drawing.Size(120, 20);
            this.AnimationName.TabIndex = 4;
            this.AnimationName.Text = "Enter Animation Name";
            // 
            // RemoveFrameButton
            // 
            this.RemoveFrameButton.Location = new System.Drawing.Point(129, 190);
            this.RemoveFrameButton.Name = "RemoveFrameButton";
            this.RemoveFrameButton.Size = new System.Drawing.Size(83, 23);
            this.RemoveFrameButton.TabIndex = 3;
            this.RemoveFrameButton.Text = "Remove Frame";
            this.RemoveFrameButton.UseVisualStyleBackColor = true;
            this.RemoveFrameButton.Click += new System.EventHandler(this.RemoveFrameButton_Click);
            // 
            // RemoveAnimationButton
            // 
            this.RemoveAnimationButton.Location = new System.Drawing.Point(138, 68);
            this.RemoveAnimationButton.Name = "RemoveAnimationButton";
            this.RemoveAnimationButton.Size = new System.Drawing.Size(83, 23);
            this.RemoveAnimationButton.TabIndex = 3;
            this.RemoveAnimationButton.Text = "Remove Animation";
            this.RemoveAnimationButton.UseVisualStyleBackColor = true;
            this.RemoveAnimationButton.Click += new System.EventHandler(this.RemoveAnimationButton_Click);
            // 
            // AddFrameButton
            // 
            this.AddFrameButton.Location = new System.Drawing.Point(129, 161);
            this.AddFrameButton.Name = "AddFrameButton";
            this.AddFrameButton.Size = new System.Drawing.Size(83, 23);
            this.AddFrameButton.TabIndex = 3;
            this.AddFrameButton.Text = "Add Frame";
            this.AddFrameButton.UseVisualStyleBackColor = true;
            this.AddFrameButton.Click += new System.EventHandler(this.AddFrameButton_Click);
            // 
            // AddAnimationButton
            // 
            this.AddAnimationButton.Location = new System.Drawing.Point(138, 39);
            this.AddAnimationButton.Name = "AddAnimationButton";
            this.AddAnimationButton.Size = new System.Drawing.Size(83, 23);
            this.AddAnimationButton.TabIndex = 3;
            this.AddAnimationButton.Text = "Add Animation";
            this.AddAnimationButton.UseVisualStyleBackColor = true;
            this.AddAnimationButton.Click += new System.EventHandler(this.AddAnimationButton_Click);
            // 
            // RectDrawingSelectionPanel
            // 
            this.RectDrawingSelectionPanel.Controls.Add(this.TriggerBox);
            this.RectDrawingSelectionPanel.Controls.Add(this.Seconds);
            this.RectDrawingSelectionPanel.Controls.Add(this.numericUpDown1);
            this.RectDrawingSelectionPanel.Controls.Add(this.AnchorPointRadioButton);
            this.RectDrawingSelectionPanel.Controls.Add(this.CollisionRectRadioButton);
            this.RectDrawingSelectionPanel.Controls.Add(this.RenderRectRadio);
            this.RectDrawingSelectionPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.RectDrawingSelectionPanel.Location = new System.Drawing.Point(0, 235);
            this.RectDrawingSelectionPanel.Name = "RectDrawingSelectionPanel";
            this.RectDrawingSelectionPanel.Size = new System.Drawing.Size(289, 223);
            this.RectDrawingSelectionPanel.TabIndex = 1;
            // 
            // Seconds
            // 
            this.Seconds.AutoSize = true;
            this.Seconds.Location = new System.Drawing.Point(12, 81);
            this.Seconds.Name = "Seconds";
            this.Seconds.Size = new System.Drawing.Size(49, 13);
            this.Seconds.TabIndex = 4;
            this.Seconds.Text = "Seconds";
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.DecimalPlaces = 2;
            this.numericUpDown1.Location = new System.Drawing.Point(67, 79);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(47, 20);
            this.numericUpDown1.TabIndex = 3;
            this.numericUpDown1.Value = new decimal(new int[] {
            15,
            0,
            0,
            131072});
            this.numericUpDown1.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
            // 
            // AnchorPointRadioButton
            // 
            this.AnchorPointRadioButton.AutoSize = true;
            this.AnchorPointRadioButton.Location = new System.Drawing.Point(12, 61);
            this.AnchorPointRadioButton.Name = "AnchorPointRadioButton";
            this.AnchorPointRadioButton.Size = new System.Drawing.Size(86, 17);
            this.AnchorPointRadioButton.TabIndex = 2;
            this.AnchorPointRadioButton.TabStop = true;
            this.AnchorPointRadioButton.Text = "Anchor Point";
            this.AnchorPointRadioButton.UseVisualStyleBackColor = true;
            this.AnchorPointRadioButton.CheckedChanged += new System.EventHandler(this.AnchorPointRadioButton_CheckedChanged);
            // 
            // CollisionRectRadioButton
            // 
            this.CollisionRectRadioButton.AutoSize = true;
            this.CollisionRectRadioButton.Location = new System.Drawing.Point(12, 38);
            this.CollisionRectRadioButton.Name = "CollisionRectRadioButton";
            this.CollisionRectRadioButton.Size = new System.Drawing.Size(89, 17);
            this.CollisionRectRadioButton.TabIndex = 1;
            this.CollisionRectRadioButton.TabStop = true;
            this.CollisionRectRadioButton.Text = "Collision Rect";
            this.CollisionRectRadioButton.UseVisualStyleBackColor = true;
            this.CollisionRectRadioButton.CheckedChanged += new System.EventHandler(this.CollisionRectRadioButton_CheckedChanged);
            // 
            // RenderRectRadio
            // 
            this.RenderRectRadio.AutoSize = true;
            this.RenderRectRadio.Location = new System.Drawing.Point(12, 15);
            this.RenderRectRadio.Name = "RenderRectRadio";
            this.RenderRectRadio.Size = new System.Drawing.Size(86, 17);
            this.RenderRectRadio.TabIndex = 0;
            this.RenderRectRadio.TabStop = true;
            this.RenderRectRadio.Text = "Render Rect";
            this.RenderRectRadio.UseVisualStyleBackColor = true;
            this.RenderRectRadio.CheckedChanged += new System.EventHandler(this.RenderRectRadio_CheckedChanged);
            // 
            // PreviewPanel
            // 
            this.PreviewPanel.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.PreviewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.PreviewPanel.Location = new System.Drawing.Point(0, 0);
            this.PreviewPanel.Name = "PreviewPanel";
            this.PreviewPanel.Size = new System.Drawing.Size(651, 458);
            this.PreviewPanel.TabIndex = 0;
            this.PreviewPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.PreviewPanel_Paint);
            this.PreviewPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.PreviewPanel_MouseMove);
            this.PreviewPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.PreviewPanel_MouseDown);
            // 
            // TriggerBox
            // 
            this.TriggerBox.Location = new System.Drawing.Point(12, 110);
            this.TriggerBox.Name = "TriggerBox";
            this.TriggerBox.Size = new System.Drawing.Size(102, 20);
            this.TriggerBox.TabIndex = 5;
            this.TriggerBox.Text = "No_Trigger";
            this.TriggerBox.TextChanged += new System.EventHandler(this.TriggerBox_TextChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(944, 482);
            this.Controls.Add(this.MainContainer);
            this.Controls.Add(this.menuStrip1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.MainContainer.Panel1.ResumeLayout(false);
            this.MainContainer.Panel1.PerformLayout();
            this.MainContainer.Panel2.ResumeLayout(false);
            this.MainContainer.ResumeLayout(false);
            this.RectDrawingSelectionPanel.ResumeLayout(false);
            this.RectDrawingSelectionPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.SplitContainer MainContainer;
        private System.Windows.Forms.Panel PreviewPanel;
        private System.Windows.Forms.ToolStripMenuItem loadImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem setDirectoryToolStripMenuItem;
        private System.Windows.Forms.Panel RectDrawingSelectionPanel;
        private System.Windows.Forms.RadioButton AnchorPointRadioButton;
        private System.Windows.Forms.RadioButton CollisionRectRadioButton;
        private System.Windows.Forms.RadioButton RenderRectRadio;
        private System.Windows.Forms.TextBox AnimationName;
        private System.Windows.Forms.Button RemoveFrameButton;
        private System.Windows.Forms.Button RemoveAnimationButton;
        private System.Windows.Forms.Button AddFrameButton;
        private System.Windows.Forms.Button AddAnimationButton;
        private System.Windows.Forms.ListBox FrameList;
        private System.Windows.Forms.ListBox AnimationList;
        private System.Windows.Forms.Label Seconds;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.TextBox TriggerBox;

    }
}

