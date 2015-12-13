using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using SGD;



namespace AnimationEditor2._0
{
    public partial class Form1 : Form
    {
        bool looping;
        int imageID;
        int imageWidth;
        int imageHeight;
        int selection;
        string fileDirectory;
        string image;
        Rectangle currRenderRect;
        Rectangle currCollisionRect;
        Point currAnchorPoint;
        Point initMousePos;

        List<Animation> theAnimations;

public bool Looping
{
  get { return looping; }
  set { looping = value; }
}

public string Image
{
    get { return image; }
    set { image = value; }
}

public string FileDirectory
{
    get { return fileDirectory; }
    set { fileDirectory = value; }
}

public int ImageID
{
    get { return imageID; }
    set { imageID = value; }
}

public int ImageWidth
{
    get { return imageWidth; }
    set { imageWidth = value; }
}
public int ImageHeight
{
    get { return imageHeight; }
    set { imageHeight = value; }
}

public int Selection
{
    get { return selection; }
    set { selection = value; }
}

public Rectangle CurrRenderRect
{
    get { return currRenderRect; }
    set { currRenderRect = value; }
}
public Rectangle CurrCollisionRect
{
    get { return currCollisionRect; }
    set { currCollisionRect = value; }
}
public Point CurrAnchorPoint
{
    get { return currAnchorPoint; }
    set { currAnchorPoint = value; }
}

public Point InitMousePos
{
    get { return initMousePos; }
    set { initMousePos = value; }
}
        public Form1()
        {
            InitializeComponent();
            SGD.ManagedDirect3D.Instance.InitManagedDirect3D( PreviewPanel, true);
            SGD.ManagedTextureManager.Instance.InitManagedTextureManager(SGD.ManagedDirect3D.Instance.Device, SGD.ManagedDirect3D.Instance.Sprite);
            Looping = true;
            Image = null;
            FileDirectory = null;
            ImageID = -1;
            ImageHeight = 0;
            ImageWidth = 0;
            Selection = 0;
            CurrAnchorPoint = Point.Empty;
            CurrCollisionRect = Rectangle.Empty;
            CurrRenderRect = Rectangle.Empty;
            theAnimations = new List<Animation>();

        }

        public void Render()
        {
            SGD.ManagedDirect3D.Instance.Clear(255, 255, 255);
            SGD.ManagedDirect3D.Instance.DeviceBegin();
            SGD.ManagedDirect3D.Instance.SpriteBegin();
            
            if (ImageID > -1)
            {
                SGD.ManagedTextureManager.Instance.Draw(ImageID, 0, 0, 1, 1, Rectangle.FromLTRB(0, 0, ImageWidth, ImageHeight), 0, 0, 0, Color.FromArgb(255, 255, 255).ToArgb() );
            }

            if (!CurrRenderRect.IsEmpty)
            {
                SGD.ManagedDirect3D.Instance.DrawRect(CurrRenderRect, 0, 0, 0);
                SGD.ManagedDirect3D.Instance.DrawRect(Rectangle.FromLTRB(CurrRenderRect.X + 2, CurrRenderRect.Y + 2, CurrRenderRect.X + CurrRenderRect.Width - 2, CurrRenderRect.Y + CurrRenderRect.Height - 2), 255, 255, 255);
            }
            
            if (!CurrCollisionRect.IsEmpty)
            {
                SGD.ManagedDirect3D.Instance.DrawRect(CurrCollisionRect, 255, 0, 0);
                SGD.ManagedDirect3D.Instance.DrawRect(Rectangle.FromLTRB(CurrCollisionRect.X + 1, CurrCollisionRect.Y + 1, CurrCollisionRect.X + CurrCollisionRect.Width - 1, CurrCollisionRect.Y + CurrCollisionRect.Height - 1), 255, 255, 255);
            }
            
            if (!CurrAnchorPoint.IsEmpty)
            {
                SGD.ManagedDirect3D.Instance.DrawRect(Rectangle.FromLTRB(CurrAnchorPoint.X - 3, CurrAnchorPoint.Y - 3, CurrAnchorPoint.X + 3, CurrAnchorPoint.Y + 3), 0, 255, 0);
            }
            
            SGD.ManagedDirect3D.Instance.SpriteEnd();
            SGD.ManagedDirect3D.Instance.DeviceEnd();
            SGD.ManagedDirect3D.Instance.Present();
        }

        private void setDirectoryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog FBD = new FolderBrowserDialog();

            if (DialogResult.OK == FBD.ShowDialog())
                FileDirectory = FBD.SelectedPath;

            if (ImageID == -1 && Image != null)
                if (File.Exists(FileDirectory + Image))
                    ImageID = SGD.ManagedTextureManager.Instance.LoadTexture(FileDirectory + Image, 0);
        }

        private void loadImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog OFD = new OpenFileDialog();
            OFD.Filter = "All Files |*.*| PNG |*.png| BMP |*.bmp";
            OFD.FilterIndex = 2;

            if (FileDirectory != null)
                OFD.InitialDirectory = FileDirectory;

            if (DialogResult.OK == OFD.ShowDialog())
            {
                ImageID = SGD.ManagedTextureManager.Instance.LoadTexture(OFD.FileName, 0);
                ImageHeight = SGD.ManagedTextureManager.Instance.GetTextureHeight(ImageID);
                ImageWidth = SGD.ManagedTextureManager.Instance.GetTextureWidth(ImageID);
                Image = OFD.SafeFileName;
                
                Size newSize = new Size();
                newSize.Height = ImageHeight;
                newSize.Width = imageWidth;

                MainContainer.Panel2.AutoScrollMinSize = newSize;
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Looping = false;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Looping = false;
        }

        private void MainContainer_Panel2_Scroll(object sender, ScrollEventArgs e)
        {
            MainContainer.Panel2.Refresh();
        }

        private void PreviewPanel_Paint(object sender, PaintEventArgs e)
        {
            Render();
        }

        private void PreviewPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Point from = InitMousePos;
                Point to = e.Location;

                if (from.X > to.X)
                {
                    int oldX = to.X;
                    to.X = from.X;
                    from.X = oldX;
                }

                if (from.Y > to.Y)
                {
                    int oldY = to.Y;
                    to.Y = from.Y;
                    from.Y = oldY;
                }

                if (Selection == 0)
                {
                    CurrRenderRect = Rectangle.FromLTRB(from.X, from.Y, to.X, to.Y);
                    if (FrameList.SelectedIndex != -1)
                    {
                        Animation.Frame changeFrame = theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex];
                        changeFrame.RenderRect = CurrRenderRect;
                        theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex] = changeFrame;
                    }
                }

                if (Selection == 1)
                {
                    CurrCollisionRect = Rectangle.FromLTRB(from.X, from.Y, to.X, to.Y);
                    if (FrameList.SelectedIndex != -1)
                    {
                        Animation.Frame changeFrame = theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex];
                        changeFrame.CollisionRect = CurrCollisionRect;
                        theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex] = changeFrame;
                    }
                }
            }
        }
        
        private void PreviewPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                InitMousePos = e.Location;

                if (Selection == 2)
                {
                    CurrAnchorPoint = InitMousePos;
                    if (FrameList.SelectedIndex != -1)
                    {
                        Animation.Frame changeFrame = theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex];
                        changeFrame.AnchorPoint = CurrAnchorPoint;
                        theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex] = changeFrame;
                    }
                }
            }
            else if (e.Button == MouseButtons.Right)
            {
                ++Selection;
                
                if (Selection > 2)
                {
                    Selection = 0;
                    RenderRectRadio.Checked = true;
                }
                else if (Selection == 1)
                    CollisionRectRadioButton.Checked = true;
                else if (Selection == 2)
                    AnchorPointRadioButton.Checked = true;
            }
        }

        private void RenderRectRadio_CheckedChanged(object sender, EventArgs e)
        {
            Selection = 0;
        }

        private void CollisionRectRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            Selection = 1;
        }

        private void AnchorPointRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            Selection = 2;
        }

        private void AddAnimationButton_Click(object sender, EventArgs e)
        {
            Animation newAnimation = new Animation(AnimationName.Text);
            theAnimations.Add(newAnimation);
            AnimationList.Items.Add(newAnimation);
            AnimationList.SelectedIndex = AnimationList.Items.Count - 1;
        }

        private void RemoveAnimationButton_Click(object sender, EventArgs e)
        {
            if (AnimationList.SelectedIndex != -1)
            {
                theAnimations.RemoveAt(AnimationList.SelectedIndex);
                AnimationList.Items.RemoveAt(AnimationList.SelectedIndex);
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewFile();
        }

        private void AddFrameButton_Click(object sender, EventArgs e)
        {
            if(AnimationList.SelectedIndex != -1)
            {
                FrameList.Items.Add(theAnimations[AnimationList.SelectedIndex].AddFrame(CurrRenderRect, CurrCollisionRect, CurrAnchorPoint));
                FrameList.SelectedIndex = FrameList.Items.Count - 1;
            }
        }

        private void AnimationList_SelectedIndexChanged(object sender, EventArgs e)
        {
            FrameList.Items.Clear();

            if (AnimationList.SelectedIndex != -1)
            {
                foreach (Animation.Frame X in theAnimations[AnimationList.SelectedIndex].TheFrames)
                {
                    FrameList.Items.Add(X);
                }
            }
        }

        private void FrameList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (FrameList.SelectedIndex != -1)
            {
                CurrRenderRect = theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex].RenderRect;
                CurrAnchorPoint = theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex].AnchorPoint;
                CurrCollisionRect = theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex].CollisionRect;
                numericUpDown1.Value = (decimal)theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex].Duration;
                TriggerBox.Text = theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex].Trigger;
            }
        }

        private void RemoveFrameButton_Click(object sender, EventArgs e)
        {
            if (FrameList.SelectedIndex != -1)
            {
                theAnimations[AnimationList.SelectedIndex].RemoveFrame(FrameList.SelectedIndex - 1);
                FrameList.Items.RemoveAt(FrameList.SelectedIndex);
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog Savedlg = new SaveFileDialog();
            Savedlg.DefaultExt = ".anim";

            if (FileDirectory != null)
                Savedlg.InitialDirectory = FileDirectory;

            if (DialogResult.OK == Savedlg.ShowDialog())
            {
                Stream newStream = new FileStream(Savedlg.FileName, FileMode.Create, FileAccess.Write);
                BinaryWriter BIW = new BinaryWriter(newStream);

                BIW.Write(Image);
                BIW.Write(theAnimations.Count);

                foreach (Animation Animation in theAnimations)
                {
                    BIW.Write(Animation.Name);
                    BIW.Write(Animation.TheFrames.Count);

                    foreach (Animation.Frame Frame in Animation.TheFrames)
                    {
                        BIW.Write(Frame.RenderRect.X);
                        BIW.Write(Frame.RenderRect.Y);
                        BIW.Write(Frame.RenderRect.X + Frame.RenderRect.Width);
                        BIW.Write(Frame.RenderRect.Y + Frame.RenderRect.Height);

                        BIW.Write(Frame.CollisionRect.X);
                        BIW.Write(Frame.CollisionRect.Y);
                        BIW.Write(Frame.CollisionRect.X + Frame.CollisionRect.Width);
                        BIW.Write(Frame.CollisionRect.Y + Frame.CollisionRect.Height);

                        BIW.Write(Frame.AnchorPoint.X);
                        BIW.Write(Frame.AnchorPoint.Y);

                        BIW.Write(Frame.Duration);
                        BIW.Write(Frame.Trigger);
                    }
                }
                
                BIW.Close();
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog Opendlg = new OpenFileDialog();

            Opendlg.DefaultExt = ".anim";

            if (DialogResult.OK == Opendlg.ShowDialog())
            {
                if (!Opendlg.FileName.EndsWith(".anim"))
                {
                    MessageBox.Show("Incorrect File Format");
                    return;
                }

                NewFile();
                Stream newStream = new FileStream(Opendlg.FileName, FileMode.Open, FileAccess.Read);
                BinaryReader BIR = new BinaryReader(newStream);

                Image = BIR.ReadString();
                int NumberOfAnimations = BIR.ReadInt32();

                for (int index = 0; index < NumberOfAnimations; ++index)
                {
                    Animation newAnimation = new Animation(BIR.ReadString());

                    int NumberOfFrames = BIR.ReadInt32();

                    for (int Jay = 0; Jay < NumberOfFrames; ++Jay)
                    {
                        Rectangle render = Rectangle.FromLTRB(BIR.ReadInt32(), BIR.ReadInt32(), BIR.ReadInt32(), BIR.ReadInt32());
                        Rectangle collision = Rectangle.FromLTRB(BIR.ReadInt32(), BIR.ReadInt32(), BIR.ReadInt32(), BIR.ReadInt32());
                        Point AnchorPoint = Point.Empty;
                        AnchorPoint.X = BIR.ReadInt32();
                        AnchorPoint.Y = BIR.ReadInt32();

                        newAnimation.AddFrame(render, collision, AnchorPoint);

                        Animation.Frame stupid = newAnimation.TheFrames[Jay];
                        stupid.Duration = BIR.ReadDouble();
                        stupid.Trigger = BIR.ReadString();
                        newAnimation.TheFrames[Jay] = stupid;
                    }

                    theAnimations.Add(newAnimation);
                    AnimationList.Items.Add(newAnimation);
                }
                BIR.Close();

                if (FileDirectory != null)
                {
                    if (File.Exists(FileDirectory + Image))
                        ImageID = SGD.ManagedTextureManager.Instance.LoadTexture(FileDirectory + Image, 0);
                    else
                        MessageBox.Show(Image + " could not be located at: " + FileDirectory);
                }
                else
                    MessageBox.Show("Could not load image, no file directory currently selected.");
            }
        }

        public void NewFile()
        {
            if (ImageID > -1)
                SGD.ManagedTextureManager.Instance.ReleaseTexture(ImageID);
            
            ImageID = -1;
            Image = null;
            CurrAnchorPoint = Point.Empty;
            CurrRenderRect = Rectangle.Empty;
            CurrCollisionRect = Rectangle.Empty;
            theAnimations.Clear();
            AnimationList.Items.Clear();
            FrameList.Items.Clear();
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            Animation.Frame FUCKINGSTUPID = theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex];
            FUCKINGSTUPID.Duration = (double)numericUpDown1.Value;
            theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex] = FUCKINGSTUPID;
        }

        private void TriggerBox_TextChanged(object sender, EventArgs e)
        {
            Animation.Frame FUCKINGSTUPID = theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex];
            FUCKINGSTUPID.Trigger = TriggerBox.Text;
            theAnimations[AnimationList.SelectedIndex].TheFrames[FrameList.SelectedIndex] = FUCKINGSTUPID;
        }
    }
}