using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using Microsoft.DirectX;
using System.Xml;
using Microsoft.DirectX.Direct3D;
using SGD;

namespace LapidemParticleEditor
{
    public partial class Form1 : Form
    {
        #region DATA MEMBERS

        private Color pnlBGColor;

        CEmitter m_Particles;
        AboutThis ab = new AboutThis();
        HowTo ht = new HowTo();
        LapidemRand randWindow = new LapidemRand();

        private bool m_bMove;

        private int fps;
        private int FrameCount;
        private int m_FrameTimer;
        private int m_nLastTime;
        private int m_nThisTime;
        private int x;
        private int y;

        private double m_FrcSec;

        #endregion // DATA MEMBERS

        #region INIT

        public Form1()
        {
            try
            {
                InitializeComponent();

                ManagedDirect3D.Instance.InitManagedDirect3D(pnlRender, pnlRender.Width, pnlRender.Height, true, false);
                ManagedTextureManager.Instance.InitManagedTextureManager
                    (ManagedDirect3D.Instance.Device, ManagedDirect3D.Instance.Sprite);

                m_Particles = new CEmitter();

                m_nThisTime = Environment.TickCount;
                m_FrameTimer = Environment.TickCount;

                timer1.Enabled = true;

                Source.SelectedIndex = 4;
                Destination.SelectedIndex = 6;
                StartXScale.Value = (decimal)m_Particles.StartScaleX;
                StartYScale.Value = (decimal)m_Particles.StartScaleY;
                EndXScale.Value = (decimal)m_Particles.EndScaleX;
                EndYScale.Value = (decimal)m_Particles.EndScaleY;
                MinVelX.Value = (decimal)m_Particles.MinVelX;
                MinVelY.Value = (decimal)m_Particles.MinVelY;
                MaxVelX.Value = (decimal)m_Particles.MaxVelX;
                MaxVelY.Value = (decimal)m_Particles.MaxVelY;
                MinAccelX.Value = (decimal)m_Particles.MinAccelX;
                MinAccelY.Value = (decimal)m_Particles.MinAccelY;
                MaxAccelX.Value = (decimal)m_Particles.MaxAccelX;
                MaxAccelY.Value = (decimal)m_Particles.MaxAccelY;
                StartRotation.Value = (decimal)m_Particles.StartRot;
                EndRotation.Value = (decimal)m_Particles.EndRot;
                ParticleLifeMax.Value = (decimal)m_Particles.MaxMaxLife;
                ParticleLifeMin.Value = (decimal)m_Particles.MinMaxLife;
                MaxPart.Value = m_Particles.MaxParticles;

                iptStartAlpha.Value = (decimal)m_Particles.StartColor.A;
                iptEndAlpha.Value = (decimal)m_Particles.EndColor.A;

                continuousModeToolStripMenuItem.Checked = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        #endregion // INIT

        #region MENU ITEMS

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                openFileDialog1.Filter = "Lapidem Particles ( *.lapipt )|*.lapipt|XML Document ( *.xml )|*.xml";
                openFileDialog1.FileName = "";

                if (DialogResult.OK == openFileDialog1.ShowDialog())
                {
                    if (openFileDialog1.FilterIndex == 1) // *.lapipt
                    {
                        FileStream input = new FileStream(openFileDialog1.FileName, FileMode.Open);
                        BinaryReader reader2 = new BinaryReader(input);

                        int _nimg = ManagedTextureManager.Instance.LoadTexture(m_Particles.ImgFileName, -1);

                        if (_nimg != -1)
                            m_Particles.ImgID = _nimg;

                        m_Particles.MaxParticles = reader2.ReadInt32();

                        byte _a1, _r1, _g1, _b1;
                        _a1 = reader2.ReadByte();
                        _r1 = reader2.ReadByte();
                        _g1 = reader2.ReadByte();
                        _b1 = reader2.ReadByte();
                        m_Particles.StartColor = Color.FromArgb(_a1, _r1, _g1, _b1);

                        byte _a2, _r2, _g2, _b2;
                        _a2 = reader2.ReadByte();
                        _r2 = reader2.ReadByte();
                        _g2 = reader2.ReadByte();
                        _b2 = reader2.ReadByte();
                        m_Particles.StartColor = Color.FromArgb(_a2, _r2, _g2, _b2);
                        
                        m_Particles.StartScaleX = reader2.ReadSingle();
                        m_Particles.EndScaleX = reader2.ReadSingle();
                        m_Particles.MinMaxLife = reader2.ReadInt32();

                        float fTemp1 = reader2.ReadSingle();

                        m_Particles.IsLooping = reader2.ReadBoolean();
                        m_Particles.MinMaxLife = reader2.ReadSingle();

                        m_Particles.PosX = reader2.ReadSingle();
                        m_Particles.PosY = reader2.ReadSingle();

                        float fTemp2 = reader2.ReadSingle();
                        float fTemp3 = reader2.ReadSingle();

                        m_Particles.MinVelX = reader2.ReadSingle();
                        m_Particles.MinVelY = reader2.ReadSingle();

                        m_Particles.MaxVelX = reader2.ReadSingle();
                        m_Particles.MaxVelY = reader2.ReadSingle();
                        
                        SetControls();
                    }
                    else if (openFileDialog1.FilterIndex == 2) // *.xml
                    {
                        XmlReaderSettings settings = new XmlReaderSettings();
                        settings.ConformanceLevel = ConformanceLevel.Document;
                        settings.IgnoreComments = true;
                        settings.IgnoreWhitespace = true;

                        if (openFileDialog1.FileName[openFileDialog1.FileName.Length - 1] != 'b')
                        {
                            using (XmlReader reader = XmlReader.Create(openFileDialog1.FileName, settings))
                            {
                                reader.MoveToContent();
                                if (reader.Name == "Particle")
                                {
                                    m_Particles.MaxParticles = Convert.ToInt32(reader["NumPart"]);
                                    if (reader["IsLooping"] == "False")
                                    {
                                        m_Particles.IsLooping = false;
                                        continuousModeToolStripMenuItem.Checked = false;
                                    }
                                    else
                                    {
                                        m_Particles.IsLooping = true;
                                        continuousModeToolStripMenuItem.Checked = true;
                                    }
                                }
                                else
                                {
                                    MessageBox.Show("File is not valid.");
                                    return;
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "MinVelocity"))
                                {
                                    m_Particles.MinVelX = (float)Convert.ToDouble(reader["X"]);
                                    m_Particles.MinVelY = (float)Convert.ToDouble(reader["Y"]);
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "MaxVelocity"))
                                {
                                    m_Particles.MaxVelX = (float)Convert.ToDouble(reader["X"]);
                                    m_Particles.MaxVelY = (float)Convert.ToDouble(reader["Y"]);
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "MinAccel"))
                                {
                                    m_Particles.MinAccelX = (float)Convert.ToDouble(reader["X"]);
                                    m_Particles.MinAccelY = (float)Convert.ToDouble(reader["Y"]);
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "MaxAccel"))
                                {
                                    m_Particles.MaxAccelX = (float)Convert.ToDouble(reader["X"]);
                                    m_Particles.MaxAccelY = (float)Convert.ToDouble(reader["Y"]);
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "StartScale"))
                                {
                                    m_Particles.StartScaleX = (float)Convert.ToDouble(reader["X"]);
                                    m_Particles.StartScaleY = (float)Convert.ToDouble(reader["Y"]);
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "EndScale"))
                                {
                                    m_Particles.EndScaleX = (float)Convert.ToDouble(reader["X"]);
                                    m_Particles.EndScaleY = (float)Convert.ToDouble(reader["Y"]);
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "StartColor"))
                                {
                                    m_Particles.StartColor = Color.FromArgb(Convert.ToInt32(reader["A"]), Convert.ToInt32(reader["R"]), Convert.ToInt32(reader["G"]), Convert.ToInt32(reader["B"]));
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "EndColor"))
                                {
                                    m_Particles.EndColor = Color.FromArgb(Convert.ToInt32(reader["A"]), Convert.ToInt32(reader["R"]), Convert.ToInt32(reader["G"]), Convert.ToInt32(reader["B"]));
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "StartRotation"))
                                {
                                    m_Particles.StartRot = (float)Convert.ToDouble(reader["Radians"]);
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "EndRotation"))
                                    m_Particles.EndRot = (float)Convert.ToDouble(reader["Radians"]);
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "MinPartileLife"))
                                    m_Particles.MinMaxLife = (float)Convert.ToDouble(reader["Seconds"]);
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "MaxPrticleLife"))
                                    m_Particles.MaxMaxLife = (float)Convert.ToDouble(reader["Seconds"]);
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "SourceBlend"))
                                    m_Particles.SourceBlend = Convert.ToInt32(reader["BlendMode"]);
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "DestinationBlend"))
                                    m_Particles.DestinationBlend = Convert.ToInt32(reader["BlendMode"]);
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "Spread"))
                                {
                                    m_Particles.XRS = Convert.ToInt32(reader["X"]);
                                    m_Particles.YRS = Convert.ToInt32(reader["Y"]);
                                }
                                reader.Read();

                                if (reader.IsStartElement() && (reader.Name == "FileName"))
                                {
                                    m_Particles.ImgFileName = reader["Name"];
                                    int num = ManagedTextureManager.Instance.LoadTexture(m_Particles.ImgFileName, -1);
                                    if (num != -1)
                                    {
                                        m_Particles.ImgID = num;
                                    }
                                }
                            }
                        }
                    }
                }
                SetControls();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                saveFileDialog1.Filter = "Lapidem Particles ( *.lapipt )|*.lapipt|XML Document ( *.xml )|*.xml";
                saveFileDialog1.FileName = "";

                if (DialogResult.OK == saveFileDialog1.ShowDialog())
                {
                    if (saveFileDialog1.FilterIndex == 1) // *.lapipt
                    {
                        FileStream output = new FileStream(saveFileDialog1.FileName, FileMode.Create);
                        BinaryWriter writer2 = new BinaryWriter(output);

                        writer2.Write(Convert.ToString(m_Particles.ImgFileName));
                        writer2.Write(Convert.ToInt32(m_Particles.MaxParticles));

                        writer2.Write(Convert.ToByte(m_Particles.StartColor.A));
                        writer2.Write(Convert.ToByte(m_Particles.StartColor.R));
                        writer2.Write(Convert.ToByte(m_Particles.StartColor.G));
                        writer2.Write(Convert.ToByte(m_Particles.StartColor.B));

                        writer2.Write(Convert.ToByte(m_Particles.EndColor.A));
                        writer2.Write(Convert.ToByte(m_Particles.EndColor.R));
                        writer2.Write(Convert.ToByte(m_Particles.EndColor.G));
                        writer2.Write(Convert.ToByte(m_Particles.EndColor.B));

                        writer2.Write((float)(m_Particles.StartScaleX));
                        writer2.Write((float)(m_Particles.EndScaleX));

                        if ((m_Particles.MinMaxLife - m_Particles.MaxMaxLife) <= 0)
                            writer2.Write(Convert.ToInt32((m_Particles.MinMaxLife - m_Particles.MaxMaxLife) + 1));
                        else writer2.Write(Convert.ToInt32(m_Particles.MinMaxLife - m_Particles.MaxMaxLife));

                        writer2.Write((float)(360.0));

                        writer2.Write(Convert.ToBoolean(m_Particles.IsLooping));
                        writer2.Write((float)(m_Particles.MinMaxLife));

                        writer2.Write((float)(m_Particles.PosX));
                        writer2.Write((float)(m_Particles.PosY));

                        writer2.Write((float)(m_Particles.MaxVelX - m_Particles.MinVelX));
                        writer2.Write((float)(m_Particles.MaxVelY - m_Particles.MinVelY));

                        writer2.Write((float)(m_Particles.MinVelX));
                        writer2.Write((float)(m_Particles.MinVelY));

                        writer2.Write((float)(m_Particles.MaxAccelX));
                        writer2.Write((float)(m_Particles.MaxAccelY));

                        // - - - - - - - - - - - - - - - -
                        // Fix this . . .
                        // - - - - - - - - - - - - - - - -
                        writer2.Write(Convert.ToInt32(m_Particles.SourceBlend));
                        writer2.Write(Convert.ToInt32(m_Particles.DestinationBlend));

                        writer2.Close();
                    }
                    else if (saveFileDialog1.FilterIndex == 2) // *.xml
                    {
                        XmlWriterSettings settings = new XmlWriterSettings();
                        settings.ConformanceLevel = ConformanceLevel.Document;
                        settings.Indent = true;

                        using (XmlWriter writer = XmlWriter.Create(saveFileDialog1.FileName, settings))
                        {
                            writer.WriteStartElement("Particle");
                            writer.WriteAttributeString("NumPart", m_Particles.MaxParticles.ToString());
                            writer.WriteAttributeString("IsLooping", m_Particles.IsLooping.ToString());
                            writer.WriteStartElement("MinVelocity");
                            writer.WriteAttributeString("X", m_Particles.MinVelX.ToString());
                            writer.WriteAttributeString("Y", m_Particles.MinVelY.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("MaxVelocity");
                            writer.WriteAttributeString("X", m_Particles.MaxVelX.ToString());
                            writer.WriteAttributeString("Y", m_Particles.MaxVelY.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("MinAccel");
                            writer.WriteAttributeString("X", m_Particles.MinAccelX.ToString());
                            writer.WriteAttributeString("Y", m_Particles.MinAccelY.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("MaxAccel");
                            writer.WriteAttributeString("X", m_Particles.MaxAccelX.ToString());
                            writer.WriteAttributeString("Y", m_Particles.MaxAccelY.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("StartScale");
                            writer.WriteAttributeString("X", m_Particles.StartScaleX.ToString());
                            writer.WriteAttributeString("Y", m_Particles.StartScaleY.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("EndScale");
                            writer.WriteAttributeString("X", m_Particles.EndScaleX.ToString());
                            writer.WriteAttributeString("Y", m_Particles.EndScaleY.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("StartColor");
                            writer.WriteAttributeString("A", m_Particles.StartColor.A.ToString());
                            writer.WriteAttributeString("R", m_Particles.StartColor.R.ToString());
                            writer.WriteAttributeString("G", m_Particles.StartColor.G.ToString());
                            writer.WriteAttributeString("B", m_Particles.StartColor.B.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("EndColor");
                            writer.WriteAttributeString("A", m_Particles.EndColor.A.ToString());
                            writer.WriteAttributeString("R", m_Particles.EndColor.R.ToString());
                            writer.WriteAttributeString("G", m_Particles.EndColor.G.ToString());
                            writer.WriteAttributeString("B", m_Particles.EndColor.B.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("StartRotation");
                            writer.WriteAttributeString("Radians", m_Particles.StartRot.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("EndRotation");
                            writer.WriteAttributeString("Radians", m_Particles.EndRot.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("MinPartileLife");
                            writer.WriteAttributeString("Seconds", m_Particles.MinMaxLife.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("MaxPrticleLife");
                            writer.WriteAttributeString("Seconds", m_Particles.MaxMaxLife.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("SourceBlend");
                            writer.WriteAttributeString("BlendMode", m_Particles.SourceBlend.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("DestinationBlend");
                            writer.WriteAttributeString("BlendMode", m_Particles.DestinationBlend.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("Spread");
                            writer.WriteAttributeString("X", m_Particles.XRS.ToString());
                            writer.WriteAttributeString("Y", m_Particles.YRS.ToString());
                            writer.WriteEndElement();
                            writer.WriteStartElement("FileName");
                            writer.WriteAttributeString("Name", m_Particles.ImgFileName);
                            writer.WriteEndElement();
                            writer.WriteEndElement();
                        }
                    }
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                Application.Exit();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void customizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                colorDialog1.FullOpen = true;

                if (DialogResult.OK == colorDialog1.ShowDialog())
                {
                    pnlBGColor = colorDialog1.Color;
                    pnlRender.BackColor = pnlBGColor;
                    m_Particles.BGColor = pnlRender.BackColor;
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                openFileDialog1.Filter = "All Image Files|*.jpg;*.jpeg;*.png;*.bmp;"
                    + "*.jfif;*.exif;*.tiff;*.raw;*.gif;*.ppm;*.pgm;*.pbm;*.pnm"
                    + "*.tga;*.ilbm;*.pcx;*.ecw;*.img;*.sid;*.cd5;*.fits;*.pgf;"
                    + "*.cgm;*.svg;*.odg;*.eps;*.pns;*.jps;*.mpo";

                openFileDialog1.RestoreDirectory = true;

                if (DialogResult.OK == openFileDialog1.ShowDialog())
                    m_Particles.BGImgID = ManagedTextureManager.Instance.LoadTexture(openFileDialog1.FileName, 0);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void randomizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                Random _rand = new Random();
                m_Particles.StartColor = Color.FromArgb(_rand.Next(256),
                    _rand.Next(256), _rand.Next(256), _rand.Next(256));
                StartingColor.BackColor = m_Particles.StartColor;

                m_Particles.EndColor = Color.FromArgb(_rand.Next(256),
                    _rand.Next(256), _rand.Next(256), _rand.Next(256));

                //m_Particles.DestinationBlend = _rand.Next(15);
                //m_Particles.SourceBlend = _rand.Next(15);

                m_Particles.StartScaleX = _rand.Next(0, 3);
                m_Particles.StartScaleY = _rand.Next(0, 3);

                m_Particles.XRS = _rand.Next(1, 50);
                m_Particles.YRS = _rand.Next(1, 50);

                m_Particles.StartRot = _rand.Next(5);
                m_Particles.EndRot = _rand.Next(5);

                SetControls();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void continuousModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                if (continuousModeToolStripMenuItem.Checked == true)
                    continuousModeToolStripMenuItem.Checked = false;
                else continuousModeToolStripMenuItem.Checked = true;

                m_Particles.IsLooping = continuousModeToolStripMenuItem.Checked;
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        #endregion // MENU ITEMS

        #region PARTICLE COLOR / IMAGE

        private void btnStartColor_Click(object sender, EventArgs e)
        {
            try
            {
                colorDialog1.FullOpen = true;

                if (DialogResult.OK == colorDialog1.ShowDialog())
                {
                    StartingColor.BackColor = colorDialog1.Color;
                    m_Particles.StartColor = Color.FromArgb((int)iptStartAlpha.Value, StartingColor.BackColor);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void btnEndColor_Click(object sender, EventArgs e)
        {
            try
            {
                colorDialog1.FullOpen = true;

                if (DialogResult.OK == colorDialog1.ShowDialog())
                {
                    EndingColor.BackColor = colorDialog1.Color;
                    m_Particles.EndColor = Color.FromArgb((int)iptEndAlpha.Value, EndingColor.BackColor);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void pbCurrentParticle_MouseDown(object sender, MouseEventArgs e)
        {
            try
            {
                openFileDialog1.Filter = "All Image Files|*.jpg;*.jpeg;*.png;*.bmp;"
                    + "*.jfif;*.exif;*.tiff;*.raw;*.gif;*.ppm;*.pgm;*.pbm;*.pnm"
                    + "*.tga;*.ilbm;*.pcx;*.ecw;*.img;*.sid;*.cd5;*.fits;*.pgf;"
                    + "*.cgm;*.svg;*.odg;*.eps;*.pns;*.jps;*.mpo";

                openFileDialog1.RestoreDirectory = true;

                if (DialogResult.OK == openFileDialog1.ShowDialog())
                {
                    pbCurrentParticle.BackgroundImage = Image.FromFile(openFileDialog1.FileName);
                    m_Particles.ImgID = ManagedTextureManager.Instance.LoadTexture(openFileDialog1.FileName, 0);

                    int index;
                    for (index = openFileDialog1.FileName.Length - 1; index >= 0; --index)
                    {
                        if (openFileDialog1.FileName[index] == '\\')
                            break;
                    }
                    string relative = "";
                    ++index;
                    for (; index < openFileDialog1.FileName.Length; ++index)
                        relative += openFileDialog1.FileName[index];


                    m_Particles.ImgFileName = relative;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        #endregion

        #region MOUSE INPUT

        private void pnlRender_MouseDown(object sender, MouseEventArgs e)
        {
            m_bMove = true;
            x = e.X;
            y = e.Y;
        }

        private void pnlRender_MouseMove(object sender, MouseEventArgs e)
        {
            if (((m_bMove && (e.X > (m_Particles.PosX - 150.0f))) &&
                ((e.X < ((m_Particles.PosX + m_Particles.XRS) + 150.0f)) &&
                (e.Y > (m_Particles.PosY - 150.0f)))) &&
                (e.Y < ((m_Particles.PosY + m_Particles.YRS) + 150.0f)))
            {
                m_Particles.PosX = e.X - (m_Particles.XRS / 2);
                m_Particles.PosY = e.Y - (m_Particles.YRS / 2);
            }
        }

        private void pnlRender_MouseUp(object sender, MouseEventArgs e)
        {
            m_bMove = false;
        }

        #endregion // MOUSE INPUT

        #region SIDE PANE OPTIONS
        private void contentsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                if (DialogResult.OK == ht.ShowDialog())
                {
                    // TODO::
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                if (DialogResult.OK == ab.ShowDialog())
                {
                    // TODO::
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void resetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                m_Particles.BGColor = Color.Black;
                m_Particles.BGImgID = -1;
                m_Particles.DestinationBlend = 7;
                m_Particles.EndColor = Color.FromArgb(0, 255, 128, 0);
                m_Particles.EndRot = 0;
                m_Particles.EndScaleX = 1;
                m_Particles.EndScaleY = 1;
                m_Particles.ImgFileName = "default.jpg";
                m_Particles.ImgID = ManagedTextureManager.Instance.LoadTexture("default.jpg", 0);
                m_Particles.IsLooping = true;
                m_Particles.MaxAccelX = 0;
                m_Particles.MaxAccelY = 0;
                m_Particles.MaxMaxLife = 1;
                m_Particles.MaxParticles = 200;
                m_Particles.MaxVelX = 100;
                m_Particles.MaxVelY = 100;
                m_Particles.MinAccelX = 0;
                m_Particles.MinAccelY = 0;
                m_Particles.MinMaxLife = 1;
                m_Particles.MinVelX = -100;
                m_Particles.MinVelY = -100;
                m_Particles.PosX = 350;
                m_Particles.PosY = 300;
                m_Particles.SourceBlend = 4;
                m_Particles.StartColor = Color.FromArgb(255, 255, 0, 0);
                m_Particles.StartRot = 0;
                m_Particles.StartScaleX = 0.3f;
                m_Particles.StartScaleY = 0.3f;
                m_Particles.XRS = 1;
                m_Particles.YRS = 1;

                SetControls();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void Source_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_Particles.SourceBlend = Source.SelectedIndex + 1;
        }

        private void Destination_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_Particles.DestinationBlend = Destination.SelectedIndex + 1;
        }

        private void ERect_Width_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.XRS = (int)ERect_Width.Value;
        }

        private void ERect_Height_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.YRS = (int)ERect_Height.Value;
        }

        private void MaxPart_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MaxParticles = (int)MaxPart.Value;
        }

        private void ParticleLifeMin_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MinMaxLife = (float)ParticleLifeMin.Value;
        }

        private void ParticleLifeMax_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MaxMaxLife = (float)ParticleLifeMax.Value;
        }

        private void iptStartAlpha_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.StartColor = Color.FromArgb((int)iptStartAlpha.Value, StartingColor.BackColor);
        }

        private void iptEndAlpha_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.EndColor = Color.FromArgb((int)iptEndAlpha.Value, EndingColor.BackColor);
        }

        private void MinVelX_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MinVelX = (float)MinVelX.Value;
        }

        private void MinVelY_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MinVelY = (float)MinVelY.Value;
        }

        private void MaxVelX_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MaxVelX = (float)MaxVelX.Value;
        }

        private void MaxVelY_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MaxVelY = (float)MaxVelY.Value;
        }

        private void MinAccelX_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MinAccelX = (float)MinAccelX.Value;
        }

        private void MinAccelY_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MinAccelY = (float)MinAccelY.Value;
        }

        private void MaxAccelX_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MaxAccelX = (float)MaxAccelX.Value;
        }

        private void MaxAccelY_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.MaxAccelY = (float)MaxAccelY.Value;
        }

        private void StartRotation_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.StartRot = (float)StartRotation.Value;
        }

        private void EndRotation_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.EndRot = (float)EndRotation.Value;
        }

        private void StartXScale_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.StartScaleX = (float)StartXScale.Value;
        }

        private void StartYScale_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.StartScaleY = (float)StartYScale.Value;
        }

        private void EndXScale_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.EndScaleX = (float)EndXScale.Value;
        }

        private void EndYScale_ValueChanged(object sender, EventArgs e)
        {
            m_Particles.EndScaleY = (float)EndYScale.Value;
        }

        #endregion // SIDE PANE OPTIONS

        #region HELPER / RENDER FUNCTIONS

        public void SetControls()
        {
            StartXScale.Value = (decimal)m_Particles.StartScaleX;
            StartYScale.Value = (decimal)m_Particles.StartScaleY;
            EndXScale.Value = (decimal)m_Particles.EndScaleX;
            EndYScale.Value = (decimal)m_Particles.EndScaleY;
            MinVelX.Value = (decimal)m_Particles.MinVelX;
            MinVelY.Value = (decimal)m_Particles.MinVelY;
            MaxVelX.Value = (decimal)m_Particles.MaxVelX;
            MaxVelY.Value = (decimal)m_Particles.MaxVelY;
            MinAccelX.Value = (decimal)m_Particles.MinAccelX;
            MinAccelY.Value = (decimal)m_Particles.MinAccelY;
            MaxAccelX.Value = (decimal)m_Particles.MaxAccelX;
            MaxAccelY.Value = (decimal)m_Particles.MaxAccelY;
            StartRotation.Value = (decimal)m_Particles.StartRot;
            EndRotation.Value = (decimal)m_Particles.EndRot;
            ParticleLifeMax.Value = (decimal)m_Particles.MaxMaxLife;
            ParticleLifeMin.Value = (decimal)m_Particles.MinMaxLife;
            MaxPart.Value = m_Particles.MaxParticles;
            ERect_Height.Value = m_Particles.YRS;
            ERect_Width.Value = m_Particles.XRS;

            pnlBGColor = m_Particles.BGColor;

            StartingColor.BackColor = m_Particles.StartColor;
            EndingColor.BackColor = m_Particles.EndColor;

            iptStartAlpha.Value = (decimal)m_Particles.StartColor.A;
            iptEndAlpha.Value = (decimal)m_Particles.EndColor.A;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            try
            {
                m_nLastTime = m_nThisTime;
                m_nThisTime = Environment.TickCount;
                m_FrcSec = (m_nThisTime - m_nLastTime) / 1000.0f;
                m_Particles.Update(m_FrcSec);

                ++FrameCount;

                if ((m_nThisTime - m_FrameTimer) > 1000)
                {
                    fps = FrameCount;
                    FrameCount = 0;
                    m_FrameTimer = m_nThisTime;
                }

                ManagedDirect3D.Instance.Clear(pnlBGColor.R, pnlBGColor.G, pnlBGColor.B);
                ManagedDirect3D.Instance.DeviceBegin();
                ManagedDirect3D.Instance.SpriteBegin();

                m_Particles.Render();
                this.Text = ("Lapidem Particle Editor [ FPS: " + fps.ToString() + " ]");

                ManagedDirect3D.Instance.SpriteEnd();
                ManagedDirect3D.Instance.DeviceEnd();
                ManagedDirect3D.Instance.Present();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message.ToString());
            }
        }

        private void Form1_Resize(object sender, EventArgs e)
        {

        }

        #endregion // HELPER / RENDER FUNCTIONS

    }
}