using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using SGD;

namespace LapidemWorldEditor
{

    enum Types { EMPTY, BOUNDARY, ROCK, WATER, LAVA, AIR, MAX_TERRAIN };
    enum EventTypes { NONE, WATER_THR, ENEMY_SPW, END_POINT, ENTRY_POINT, AIR_CURRENT,POWERUP, MAX_EVENTS };

    struct Base
    {
        
        int tileID;

        public int TileID
        {
            get { return tileID; }
            set { tileID = value; }
        }
        int type;

        public int Type
        {
            get { return type; }
            set { type = value; }
        }

        int direction;

        public int Direction
        {
            get { return direction; }
            set { direction = value; }
        }

    };

    struct Event
    {

        int eventType;

        public int EventType
        {
            get { return eventType; }
            set { eventType = value; }
        }
        int data;

        public int Data
        {
            get { return data; }
            set { data = value; }
        }

    };

    public partial class Form1 : Form
    {
        int tmTileSet;

        public int TmTileSet
        {
            get { return tmTileSet; }
            set { tmTileSet = value; }
        }
        Bitmap tileSet;
        string filePath;

        int tileSelection;
        int typeSelection;
        int eventSelection;

        public int EventSelection
        {
            get { return eventSelection; }
            set { eventSelection = value; }
        }

        public int TypeSelection
        {
            get { return typeSelection; }
            set { typeSelection = value; }
        }

        int baseTile;

        Base[] tiles;
        Event[] eTiles;

        internal Event[] ETiles
        {
            get { return eTiles; }
            set { eTiles = value; }
        }

        Point mousePastLoc;

        int brushType;

        public int BrushType
        {
            get { return brushType; }
            set { brushType = value; }
        }



        public Point MousePastLoc
        {
            get { return mousePastLoc; }
            set { mousePastLoc = value; }
        }

        internal Base[] Tiles
        {
            get { return tiles; }
            set { tiles = value; }
        }



        Rectangle camera;

        public int BaseTile
        {
            get { return baseTile; }
            set { baseTile = value; }
        }
        public Rectangle Camera
        {
            get { return camera; }
            set { camera = value; }
        }

        public int TileSelection
        {
            get { return tileSelection; }
            set { tileSelection = value; }
        }


        public int TileWidth
        {
            get { return (int)nUD_TileWidth.Value; }
            set { nUD_TileWidth.Value = value; }
        }
        public int TileHeight
        {
            get { return (int)nUD_TileHeight.Value; }
            set { nUD_TileHeight.Value = value; }
        }
        public int CollumnTile
        {
            get { return (int)nUD_TilesX.Value; }
            set { nUD_TilesX.Value = value; }
        }
        public int RowTile
        {
            get { return (int)nUD_TilesY.Value; }
            set { nUD_TilesY.Value = value; }
        }

        public int WorldCollumn
        {
            get { return (int)nUD_WorldWidth.Value; }
            set { nUD_WorldWidth.Value = value; }
        }

        public int WorldRow
        {
            get { return (int)nUD_WorldHeight.Value; }
            set { nUD_WorldHeight.Value = value; }
        }

        bool firstRectClick;
        Rectangle RectBrush;
        Point rectStartPt;

        public Rectangle RectBrush1
        {
            get { return RectBrush; }
            set { RectBrush = value; }
        }

        public bool FirstRectClick
        {
            get { return firstRectClick; }
            set { firstRectClick = value; }
        }


        int bgImgID;

        public int BgImgID
        {
            get { return bgImgID; }
            set { bgImgID = value; }
        }
        string bgFilename;

        public string BgFilename
        {
            get { return bgFilename; }
            set { bgFilename = value; }
        }

        string bgMfilenamel;

        public string BgMfilenamel
        {
            get { return bgMfilenamel; }
            set { bgMfilenamel = value; }
        }
        string nextLvlFilename;

        public string NextLvlFilename
        {
            get { return nextLvlFilename; }
            set { nextLvlFilename = value; }
        }
        int powerUpSelected;

        public int PowerUpSelected
        {
            get { return powerUpSelected; }
            set { powerUpSelected = value; }
        }

        public Form1()
        {
            InitializeComponent();

            ManagedDirect3D.Instance.InitManagedDirect3D(splitContainer1.Panel2, false);
            ManagedTextureManager.Instance.InitManagedTextureManager(ManagedDirect3D.Instance.Device, ManagedDirect3D.Instance.Sprite);

            TmTileSet = ManagedTextureManager.Instance.LoadTexture("ScA_WorldTileset.PNG", Color.FromArgb(255, 0, 255).ToArgb());
            tileSet = Properties.Resources.ScA_WorldTileset;

            showGridToolStripMenuItem.Checked = true;
            TileWidth = 16;
            TileHeight = 16;

            CollumnTile = 100;
            RowTile = 100;
            Camera = new Rectangle(new Point(0, 0), new Size(splitContainer1.Panel2.Width, splitContainer1.Panel2.Height));

            panel_TileSet.Size = tileSet.Size;

            panelEmptyTerrain.Size = new Size(TileWidth, TileHeight);
            panelCurrentTerrain.Size = new Size(TileWidth, TileHeight);

            tiles = new Base[WorldCollumn * WorldRow];
            eTiles = new Event[WorldCollumn * WorldRow];
            for (int i = 0; i < WorldCollumn * WorldRow; ++i)
            {
                Tiles[i].TileID = 0;
                Tiles[i].Type = 0;
            }

            BaseTile = 0;

            radioButton1.Select();


            BrushType = 0;

            showEvents.Checked = true;
            RectBrush1 = new Rectangle(0, 0, -1, -1);

            BgImgID = -1;
            BgFilename = " ";
            filePath = " ";
            NextLvlFilename = " ";
            bgMfilenamel = " ";
        }

        void Render()
        {
            ManagedDirect3D.Instance.Clear(0,0,0);
            ManagedDirect3D.Instance.DeviceBegin();
            ManagedDirect3D.Instance.SpriteBegin();
            int panelWidth = Camera.Width;

            int panelHeight = Camera.Height;

            panelWidth /= TileWidth;
            panelHeight /= TileHeight;

            int startI = Camera.X;
            int startJ = Camera.Y;

            startI /= TileWidth;
            startJ /= TileHeight;

            if (BgImgID != -1)
                ManagedTextureManager.Instance.Draw(BgImgID, 0, 0, 1.0f, 1.0f, Rectangle.Empty , 0, 0, 0.0f, 0);

            for (int i = 0; i < WorldCollumn; ++i)
            {
                for (int j = 0; j < WorldRow; ++j)
                {
                    Rectangle r = new Rectangle(i * TileWidth - Camera.X, j * TileHeight - Camera.Y, TileWidth, TileHeight);

                    if (Tiles[i + j * WorldCollumn].TileID != BaseTile && r.Contains( new Point(i*TileWidth-Camera.X , j*TileHeight-Camera.Y ) ))
                    {
                        ManagedTextureManager.Instance.Draw(tmTileSet, i * TileWidth - Camera.X , j * TileHeight - Camera.Y, 1.0f, 1.0f, GetRectFromId_TileSet(Tiles[i + j * WorldCollumn].TileID), 0, 0, 0.0f, 0);
                       // e.Graphics.DrawImage(tileSet, GetRectFromId_WorldMap(i + j * WorldCollumn), GetRectFromId_TileSet(Tiles[i + j * WorldCollumn].TileID), GraphicsUnit.Pixel);

                        if (showType.Checked)
                            switch (Tiles[i + j * WorldCollumn].Type)
                            {
                                case (int)Types.BOUNDARY:
                                    ManagedDirect3D.Instance.DrawText("B", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 150,150, 220);
                                   // e.Graphics.DrawString("B", new Font("Arial", 12), Brushes.White, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                    break;
                                case (int)Types.ROCK:
                                    ManagedDirect3D.Instance.DrawText("R", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 200, 150, 150);
                                    //e.Graphics.DrawString("R", new Font("Arial", 12), Brushes.White, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                    break;
                                case (int)Types.LAVA:
                                    ManagedDirect3D.Instance.DrawText("L", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 225, 50, 50);

                                   // e.Graphics.DrawString("L", new Font("Arial", 12), Brushes.White, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                    break;
                                case (int)Types.WATER:
                                    ManagedDirect3D.Instance.DrawText("W", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 200, 200, 220);

                                    //e.Graphics.DrawString("W", new Font("Arial", 12), Brushes.White, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                    break;
                            };

                        



                    }
                    if (showEvents.Checked)
                        switch (ETiles[i + j * WorldCollumn].EventType)
                        {
                            case (int)EventTypes.AIR_CURRENT:
                                ManagedDirect3D.Instance.DrawText("AC", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 100, 220, 150);

                                //e.Graphics.DrawString("AC", new Font("Arial", TileWidth >> 1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                            case (int)EventTypes.WATER_THR:
                                ManagedDirect3D.Instance.DrawText("WT", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 100,100, 220);

                               // e.Graphics.DrawString("WT", new Font("Arial", TileWidth >> 1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                            case (int)EventTypes.ENEMY_SPW:
                                ManagedDirect3D.Instance.DrawText("SPW", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 100, 100, 100);
   
                            // e.Graphics.DrawString("SPW", new Font("Arial", TileWidth >> 1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                            case (int)EventTypes.ENTRY_POINT:
                                ManagedDirect3D.Instance.DrawText("Str", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 200, 200, 200);
                                //e.Graphics.DrawString("Strt", new Font("Arial", TileWidth >> 1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                            case (int)EventTypes.END_POINT:
                                ManagedDirect3D.Instance.DrawText("End", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 200, 200, 202);
                                //e.Graphics.DrawString("End", new Font("Arial", TileWidth >> 1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                            case (int)EventTypes.POWERUP:
                                ManagedDirect3D.Instance.DrawText("PWR", i * TileWidth - Camera.X, j * TileHeight - Camera.Y, 200, 200, 202);
                                //e.Graphics.DrawString("End", new Font("Arial", TileWidth >> 1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                        };
                    //Rectangle r = new Rectangle(i * TileWidth - Camera.X, j * TileHeight - Camera.Y, TileWidth, TileHeight);
                    //ManagedDirect3D.Instance.DrawRect(r, 0,0,0);
                    //e.Graphics.DrawRectangle(Pens.Black, GetRectFromId_WorldMap(i + j * WorldCollumn));
                }
            }
            ManagedDirect3D.Instance.SpriteEnd();
            if (showGridToolStripMenuItem.Checked)
            {
                for (int i = 0; i <= WorldCollumn; ++i)
                {
                    if( (i * TileWidth - Camera.X) > 0 && (i * TileWidth - Camera.X) < splitContainer1.Panel2.Width)
                    ManagedDirect3D.Instance.DrawLine(i * TileWidth - Camera.X, 0 - Camera.Y, i * TileWidth - Camera.X, WorldRow * TileHeight - Camera.Y, 220, 220, 220);

                }
                for (int j = 0; j <= WorldRow; ++j)
                {
                    if ((j* TileHeight - Camera.Y ) > 0 && (j * TileHeight - Camera.Y) < splitContainer1.Panel2.Height )

                    ManagedDirect3D.Instance.DrawLine(0 - Camera.X, j * TileHeight - Camera.Y, WorldCollumn * TileWidth - Camera.X, j * TileHeight - Camera.Y, 220, 220, 220);

                }
            }
            if (RectBrush1.Width >= 0)
            {
                ManagedDirect3D.Instance.DrawLine(RectBrush1.X * TileWidth - Camera.X ,
                    RectBrush1.Y * TileHeight - Camera.Y ,
                    (RectBrush1.Width + 1) * TileWidth - Camera.X,
                    RectBrush1.Y * TileHeight - Camera.Y,
                    225, 0, 0);
                ManagedDirect3D.Instance.DrawLine(RectBrush1.X * TileWidth - Camera.X,
                    (RectBrush1.Height + 1) * TileHeight - Camera.Y,
                    (RectBrush1.Width + 1) * TileWidth - Camera.X,
                    (RectBrush1.Height + 1) * TileHeight - Camera.Y,
                    225, 0, 0);

                ManagedDirect3D.Instance.DrawLine(RectBrush1.X * TileWidth - Camera.X,
                    RectBrush1.Y * TileHeight - Camera.Y,
                    RectBrush1.X * TileWidth - Camera.X,
                    (RectBrush1.Height + 1) * TileHeight - Camera.Y,
                    225, 0, 0);
                ManagedDirect3D.Instance.DrawLine((RectBrush1.Width + 1) * TileWidth - Camera.X,
                    RectBrush1.Y * TileHeight - Camera.Y,
                    (RectBrush1.Width + 1) * TileWidth - Camera.X,
                    (RectBrush1.Height + 1) * TileHeight - Camera.Y,
                    225, 0, 0);
            }
            ManagedDirect3D.Instance.DeviceEnd();
            ManagedDirect3D.Instance.Present();

        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }
        //  

        private void splitContainer1_Panel2_Paint(object sender, PaintEventArgs e)
        {
            /*
            int panelWidth = Camera.Width;

            int panelHeight = Camera.Height;

            panelWidth /= TileWidth;
            panelHeight /= TileHeight;

            int startI = Camera.X;
            int startJ = Camera.Y;

            startI /= TileWidth;
            startJ /= TileHeight;


            for (int i = 0; i < WorldCollumn; ++i)
            {
                for (int j = 0; j < WorldRow; ++j)
                {
                    if (Tiles[i + j * WorldCollumn].TileID != BaseTile)
                    {
                        e.Graphics.DrawImage(tileSet, GetRectFromId_WorldMap(i + j * WorldCollumn), GetRectFromId_TileSet(Tiles[i + j * WorldCollumn].TileID), GraphicsUnit.Pixel);

                        if (showType.Checked)
                            switch (Tiles[i + j * WorldCollumn].Type)
                            {
                                case (int)Types.BOUNDARY:
                                    e.Graphics.DrawString("B", new Font("Arial", 12), Brushes.White, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                    break;
                                case (int)Types.ROCK:
                                    e.Graphics.DrawString("R", new Font("Arial", 12), Brushes.White, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                    break;
                                case (int)Types.LAVA:
                                    e.Graphics.DrawString("L", new Font("Arial", 12), Brushes.White, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                    break;
                                case (int)Types.WATER:
                                    e.Graphics.DrawString("W", new Font("Arial", 12), Brushes.White, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                    break;
                            };

                        


                    }
                    if (showEvents.Checked)
                        switch (ETiles[i + j * WorldCollumn].EventType)
                        {
                            case (int)EventTypes.AIR_CURRENT:
                                e.Graphics.DrawString("AC", new Font("Arial", TileWidth>>1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                            case (int)EventTypes.WATER_THR:
                                e.Graphics.DrawString("WT", new Font("Arial", TileWidth>>1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                            case (int)EventTypes.ENEMY_SPW:
                                e.Graphics.DrawString("SPW", new Font("Arial", TileWidth>>1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                            case (int)EventTypes.ENTRY_POINT:
                                e.Graphics.DrawString("Strt", new Font("Arial", TileWidth>>1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                            case (int)EventTypes.END_POINT:
                                e.Graphics.DrawString("End", new Font("Arial", TileWidth>>1), Brushes.AliceBlue, ((i + j * WorldCollumn) % WorldCollumn) * TileWidth - Camera.X, ((i + j * WorldCollumn) / WorldCollumn * TileWidth) - Camera.Y);
                                break;
                        };
                    e.Graphics.DrawRectangle(Pens.Black, GetRectFromId_WorldMap(i + j * WorldCollumn));
                }
            }
             */
        }



        private void nUD_TileWidth_ValueChanged(object sender, EventArgs e)
        {
            if (nUD_TileWidth.Value < 16)
                nUD_TileWidth.Value = 16;
            // graphPanel1.Invalidate();
        }

        private void nUD_TileHeight_ValueChanged(object sender, EventArgs e)
        {
            if (nUD_TileHeight.Value < 16)
                nUD_TileHeight.Value = 16;
            // graphPanel1.Invalidate();

        }


        private void panel_TileSet_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(tileSet, 0.0f, 0.0f);

            e.Graphics.DrawRectangle(Pens.Black, new Rectangle(new Point(0, 0), panel_TileSet.Size));
            Rectangle r = GetRectFromId_TileSet(TileSelection);
            r.X -= panel_TileFrame.AutoScrollOffset.X;
            r.Y -= panel_TileFrame.AutoScrollOffset.Y;

            e.Graphics.DrawRectangle(Pens.Red, r);




        }

        private void panel_TileSet_MouseClick(object sender, MouseEventArgs e)
        {
            Point p = new Point(e.Location.X, e.Location.Y);

            p.X -= panel_TileFrame.AutoScrollOffset.X;
            p.Y -= panel_TileFrame.AutoScrollOffset.Y;

            p.X /= TileWidth;
            p.Y /= TileHeight;

            TileSelection = p.X + p.Y * CollumnTile;

            panelCurrentTerrain.Invalidate();
            panel_TileSet.Invalidate();

            //label14.Text = tileSelection.ToString();
        }

        private void panelCurrentTerrain_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(tileSet, new Rectangle(0, 0, TileWidth, TileHeight), GetRectFromId_TileSet(TileSelection), GraphicsUnit.Pixel);
        }

        Rectangle GetRectFromId_TileSet(int tileID)
        {
            return new Rectangle((tileID % CollumnTile) * TileWidth, (tileID / CollumnTile) * TileHeight, TileWidth, TileHeight);

        }
        Rectangle GetRectFromId_WorldMap(int tileID)
        {
            int X;
            int Y;

            X = (tileID % WorldCollumn) * TileWidth - Camera.X;
            Y = (tileID / WorldCollumn) * TileHeight - Camera.Y;





            return new Rectangle(X, Y, TileWidth, TileHeight);

        }

        private void splitContainer1_Panel2_MouseClick(object sender, MouseEventArgs e)
        {
            Point p = new Point(e.Location.X, e.Location.Y);

            p.X += Camera.X;
            p.Y += Camera.Y;

            p.X /= TileWidth;
            p.Y /= TileHeight;

            int indexSelected = p.X + p.Y * CollumnTile;

            Tiles[indexSelected].TileID = TileSelection;

            //splitContainer1.Panel2.Invalidate(new Rectangle(e.Location.X - TileWidth, e.Location.Y - TileHeight, TileWidth * 2, TileHeight * 2));

        }

        private void button5_Click(object sender, EventArgs e)
        {
            BaseTile = TileSelection;
            panelEmptyTerrain.Invalidate();
           // splitContainer1.Panel2.Invalidate();
        }

        private void splitContainer1_Panel2_MouseMove(object sender, MouseEventArgs e)
        {


            if (MouseButtons.Left == e.Button)
            {
                if (tabPage3.ContainsFocus)
                {
                    Point p = new Point(e.Location.X, e.Location.Y);

                    p.X += Camera.X;
                    p.Y += Camera.Y;

                    p.X /= TileWidth;
                    p.Y /= TileHeight;

                    int indexSelected = p.X + p.Y * WorldCollumn;
                    if (indexSelected >= 0 && indexSelected < WorldCollumn * WorldRow)
                    {
                        ETiles[indexSelected].EventType  = EventSelection ;
                        switch (EventSelection)
                        {
                            case (int)EventTypes.AIR_CURRENT:
                                ETiles[indexSelected].Data = (int)nUD_AirDirection.Value;
                                break;
                            case (int)EventTypes.WATER_THR:
                                ETiles[indexSelected].Data = (int)nUD_WaterThreshold.Value;
                                break;
                            case (int)EventTypes.POWERUP:
                                ETiles[indexSelected].Data = PowerUpSelected;
                                break;

                        };
                       // ETiles[indexSelected].Type = TypeSelection;

                    }

                    p.X *= Camera.X;
                    p.Y *= camera.Y;

                }else
                switch (BrushType)
                {
                    case 0:
                        {
                            Point p = new Point(e.Location.X, e.Location.Y);

                            p.X += Camera.X;
                            p.Y += Camera.Y;

                            p.X /= TileWidth;
                            p.Y /= TileHeight;

                            int indexSelected = p.X + p.Y * WorldCollumn;
                            if (indexSelected >= 0 && indexSelected < WorldCollumn * WorldRow)
                            {
                                Tiles[indexSelected].TileID = TileSelection;
                                Tiles[indexSelected].Type = TypeSelection;

                            }

                            p.X *= Camera.X;
                            p.Y *= camera.Y;

                            //splitContainer1.Panel2.Invalidate(new Rectangle(e.Location.X - TileWidth, e.Location.Y - TileHeight, TileWidth * 2, TileHeight * 2));
                        }
                        break;
                    case 1:
                        {
                            Point p = new Point(e.Location.X, e.Location.Y);

                            p.X += Camera.X;
                            p.Y += Camera.Y;

                            p.X /= TileWidth;
                            p.Y /= TileHeight;

                            if (!FirstRectClick)
                            {
                                rectStartPt = p;
                                RectBrush1 = new Rectangle( p.X , p.Y, p.X , p.Y );
                                
                            }
                            FirstRectClick = true;

                            RectBrush1 = new Rectangle(Math.Min(rectStartPt.X, p.X), Math.Min(rectStartPt.Y, p.Y), Math.Max(rectStartPt.X, p.X), Math.Max(rectStartPt.Y, p.Y));

                            //if (p.X < RectBrush1.X )
                            //{
                            //    RectBrush1 =new Rectangle( p.X, RectBrush1.Y, RectBrush1.Width, RectBrush1.Height);
                            //}
                            //else if (p.X > RectBrush1.Width)
                            //{
                            //    RectBrush1 = new Rectangle(RectBrush1.X, RectBrush1.Y, p.X, RectBrush1.Height);

                            //}

                            //if (p.Y < RectBrush1.Y)
                            //{
                            //    RectBrush1 = new Rectangle(RectBrush1.X, p.Y, RectBrush1.Width, RectBrush1.Height);
                            //}else if (p.Y > RectBrush1.Height )
                            //{
                            //    RectBrush1 = new Rectangle(RectBrush1.X, RectBrush1.Y , RectBrush1.Width, p.Y );
                            //}





                        }
                        break;
                    
                    case 2:
                        {
                            Point p = new Point(e.Location.X, e.Location.Y);

                            p.X += Camera.X;
                            p.Y += Camera.Y;

                            p.X /= TileWidth;
                            p.Y /= TileHeight;

                            int Ytemp = p.Y;
                            int IDSelectedOnWorld;

                            bool changeDir = false;

                            int indexSelected = p.X + p.Y * WorldCollumn;
                            if (indexSelected >= 0 && indexSelected < WorldCollumn * WorldRow)
                            {
                                IDSelectedOnWorld = Tiles[indexSelected].TileID;

                                while (p.Y > 0)
                                {

                                    Tiles[indexSelected].TileID = TileSelection;
                                    Tiles[indexSelected].Type = TypeSelection;

                                    FillHorizontalLine(indexSelected, p.X, IDSelectedOnWorld );
                                    //FillVerticalLine(indexSelected, p.Y);

                                    if (p.Y < WorldRow - 1 && !changeDir)
                                    {
                                        p.Y += 1;
                                        indexSelected = p.X + p.Y * WorldCollumn;
                                        if (Tiles[indexSelected].TileID != IDSelectedOnWorld )
                                        {
                                            p.Y = Ytemp;
                                            changeDir = true;
                                        }


                                    }
                                    else
                                    {
                                        changeDir = true;
                                        p.Y -= 1;
                                        indexSelected = p.X + p.Y * WorldCollumn;
                                        if (Tiles[indexSelected].TileID != IDSelectedOnWorld )
                                            break;
                                    }
                                    indexSelected = p.X + p.Y * WorldCollumn;
                                }


                            }




                        }
                        break;

                };

               // splitContainer1.Panel2.Invalidate();

            }
            else if (MouseButtons.Right == e.Button)
            {
                Point p = new Point(e.Location.X - MousePastLoc.X, e.Location.Y - MousePastLoc.Y);

                Camera = new Rectangle(Camera.Location.X - p.X, Camera.Location.Y - p.Y, Camera.Width, Camera.Width);



                //splitContainer1.Panel2.Invalidate();
            }
          
            MousePastLoc = e.Location;
        }

        void DrawTerrainTile_IJ(int i, int j)
        {
            
            int indexSelected = i + j * WorldCollumn;

            if (indexSelected > 0 && indexSelected < WorldCollumn * WorldRow)
            {

                Tiles[indexSelected].TileID = TileSelection;
                Tiles[indexSelected].Type = TypeSelection;

            }

        }

        void DrawTerrainTile_Indx(int indexSelected)
        {

            Tiles[indexSelected].TileID = TileSelection;
            Tiles[indexSelected].Type = TypeSelection;

        }

        void FillVerticalLine(int indexSelected, int pY)
        {
            for (int j = 1; j < WorldRow - pY; ++j)
            {
                if ((indexSelected - j * WorldCollumn) > 0 && (indexSelected - j * WorldCollumn) < WorldRow * WorldCollumn)
                {
                    if (Tiles[indexSelected - j * WorldCollumn].Type == Tiles[indexSelected].Type)
                        break;

                    Tiles[indexSelected - j * WorldCollumn].TileID = TileSelection;
                    Tiles[indexSelected - j * WorldCollumn].Type = TypeSelection;
                }
            }
            for (int j = 1; j < WorldRow - pY; ++j)
            {
                if ((indexSelected + j * WorldCollumn) > 0 && (indexSelected - j * WorldCollumn) < WorldRow * WorldCollumn)
                {
                    if (Tiles[indexSelected + j * WorldCollumn].Type == Tiles[indexSelected].Type)
                        break;

                    Tiles[indexSelected + j * WorldCollumn].TileID = TileSelection;
                    Tiles[indexSelected + j * WorldCollumn].Type = TypeSelection;
                }
            }

        }
        void FillHorizontalLine(int indexSelected, int pX, int IDSelectedOnWorld )
        {
            for (int i = 1; i < WorldCollumn - pX; ++i)
            {
                if (Tiles[indexSelected - i].TileID != IDSelectedOnWorld )
                    break;

                Tiles[indexSelected - i].TileID = TileSelection;
                Tiles[indexSelected - i].Type = TypeSelection;


            }
            for (int i = 1; i < WorldCollumn - pX; ++i)
            {
                if (Tiles[indexSelected + i].TileID != IDSelectedOnWorld)
                    break;

                Tiles[indexSelected + i].TileID = TileSelection;
                Tiles[indexSelected + i].Type = TypeSelection;


            }

        }

        private void panelEmptyTerrain_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawImage(tileSet, new Rectangle(0, 0, TileWidth, TileHeight), GetRectFromId_TileSet(BaseTile), GraphicsUnit.Pixel);

        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            TypeSelection = (int)Types.BOUNDARY;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            TypeSelection = (int)Types.ROCK;

        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            TypeSelection = (int)Types.WATER;

        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            TypeSelection = (int)Types.LAVA;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            TileSelection = BaseTile;
            panelCurrentTerrain.Invalidate();
            panel_TileSet.Invalidate();
        }

        private void nUD_WorldWidth_ValueChanged(object sender, EventArgs e)
        {
            {
                Base[] newBase = new Base[WorldCollumn * WorldRow];

                //int panelWidth = Camera.Width;

                //int panelHeight = Camera.Height;

                //panelWidth /= TileWidth;
                //panelHeight /= TileHeight;



                if (Tiles != null)
                    for (int i = 0; i < Math.Min(WorldCollumn, Tiles.Length / WorldRow); ++i)
                    {
                        for (int j = 0; j < Math.Min(WorldRow, Tiles.Length / WorldCollumn); ++j)
                        {

                            newBase[i + j * WorldCollumn] = Tiles[i + j * Tiles.Length / WorldRow];

                        }
                    }

                tiles = newBase;
            }
            {
                Event[] newEvent = new Event[WorldCollumn * WorldRow];

                //int panelWidth = Camera.Width;

                //int panelHeight = Camera.Height;

                //panelWidth /= TileWidth;
                //panelHeight /= TileHeight;

                if (ETiles != null)
                    for (int i = 0; i < Math.Min(WorldCollumn, ETiles.Length / WorldRow); ++i)
                    {
                        for (int j = 0; j < Math.Min(WorldRow, ETiles.Length / WorldCollumn); ++j)
                        {

                            newEvent[i + j * WorldCollumn] = ETiles[i + j * WorldCollumn];

                        }
                    }

                eTiles = newEvent;
            }

           // splitContainer1.Panel2.Invalidate();

        }

        private void showType_CheckedChanged(object sender, EventArgs e)
        {
            //splitContainer1.Panel2.Invalidate();
        }

        private void nUD_WorldHeight_ValueChanged(object sender, EventArgs e)
        {
            {
                Base[] newBase = new Base[WorldCollumn * WorldRow];

                int panelWidth = Camera.Width;

                int panelHeight = Camera.Height;

                panelWidth /= TileWidth;
                panelHeight /= TileHeight;

                if (Tiles != null)
                    for (int i = 0; i < Math.Min(WorldCollumn, Tiles.Length / WorldRow); ++i)
                    {
                        for (int j = 0; j < Math.Min(WorldRow, Tiles.Length / WorldCollumn); ++j)
                        {

                            newBase[i + j * WorldCollumn] = Tiles[i + j * WorldCollumn];

                        }
                    }

                tiles = newBase;
            }
            {
                Event[] newEvent = new Event[WorldCollumn * WorldRow];

                int panelWidth = Camera.Width;

                int panelHeight = Camera.Height;

                panelWidth /= TileWidth;
                panelHeight /= TileHeight;

                if (ETiles != null)
                    for (int i = 0; i < Math.Min(WorldCollumn, ETiles.Length / WorldRow); ++i)
                    {
                        for (int j = 0; j < Math.Min(WorldRow, ETiles.Length / WorldCollumn); ++j)
                        {

                            newEvent[i + j * WorldCollumn] = ETiles[i + j * WorldCollumn];

                        }
                    }

                eTiles = newEvent;
            }
           // splitContainer1.Panel2.Invalidate();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            BrushType = 0;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            BrushType = 2;
        }

        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {
            EventSelection = (int)EventTypes.ENEMY_SPW;
            groupAir.Visible = false;
            groupBox9.Visible = false;
            groupBox8.Visible = false;

        }

        private void radioButton6_CheckedChanged(object sender, EventArgs e)
        {
            EventSelection = (int)EventTypes.AIR_CURRENT;
            groupBox9.Visible = false;
            groupAir.Visible = true;
            groupBox8.Visible = false;

        }

        private void radioButton9_CheckedChanged(object sender, EventArgs e)
        {
            EventSelection = (int)EventTypes.WATER_THR;
            groupAir.Visible = false;
            groupBox9.Visible = false;
            groupBox8.Visible = true;


        }

        private void radioButton7_CheckedChanged(object sender, EventArgs e)
        {
            EventSelection = (int)EventTypes.ENTRY_POINT;
            groupAir.Visible = false;
            groupBox8.Visible = false;
            groupBox9.Visible = false;

        }

        private void radioButton8_CheckedChanged(object sender, EventArgs e)
        {
            EventSelection = (int)EventTypes.END_POINT;
            groupAir.Visible = false;
            groupBox8.Visible = false;
            groupBox9.Visible = false;



        }

        private void radioButton10_CheckedChanged(object sender, EventArgs e)
        {
            EventSelection = (int)EventTypes.NONE;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Render();
        }

        private void showGridToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showGridToolStripMenuItem.Checked = !showGridToolStripMenuItem.Checked;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            BrushType = 1;
        }

        private void splitContainer1_Panel2_MouseDown(object sender, MouseEventArgs e)
        {

        }

        private void splitContainer1_Panel2_MouseUp(object sender, MouseEventArgs e)
        {
            FirstRectClick = false;

            for (int i = RectBrush1.X; i <= RectBrush1.Width; ++i)
            {
                for (int j = RectBrush1.Y; j <= RectBrush1.Height; ++j)
                {
                    DrawTerrainTile_IJ(i, j);
                }

            }

            RectBrush1 = new Rectangle(0, 0, -1, -1);

        }

        private void button6_Click(object sender, EventArgs e)
        {
            OpenFileDialog open = new OpenFileDialog();

            if (DialogResult.OK == open.ShowDialog())
            {
                if (BgImgID != -1)
                {
                    ManagedTextureManager.Instance.ReleaseTexture(BgImgID);
                }
                BgImgID = ManagedTextureManager.Instance.LoadTexture(open.FileName, 0);



                BgFilename = open.FileName;
                ///////////////////////////////
                // get relative path
                int i;
                for (i = open.FileName.Length - 1; i >= 0; --i)
                {
                    if (open.FileName[i] == '\\')
                        break;

                }
                string relative = "resource\\graphics";

                for (; i < open.FileName.Length; ++i)
                    relative += open.FileName[i];

                /////////////////////////////////
                label16.Text  = BgFilename = relative ;
                //tileSet = new Bitmap(open.FileName);
                //ManagedTextureManager.Instance.ReleaseTexture(TmTileSet);
                //TmTileSet = 

                //if (tool != null)
                {
                    //tool.UpdateTileset(tileSet, new Size((int)nTileW.Value, (int)nTileH.Value), new Size((int)nTileSetW.Value, (int)nTileSetH.Value));


                }
            }
        }

        private void importTileSetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog open = new OpenFileDialog();

            if (DialogResult.OK == open.ShowDialog())
            {
                
                tileSet = new Bitmap(open.FileName);
                ManagedTextureManager.Instance.ReleaseTexture(TmTileSet);
                TmTileSet = ManagedTextureManager.Instance.LoadTexture(open.FileName, 0);

                ///////////////////////////////
                // get relative path
                int i;
                for (i = open.FileName.Length - 1; i >= 0; --i)
                {
                    if (open.FileName[i] == '\\')
                        break;

                }
                string relative = "resource\\graphics";

                for (; i < open.FileName.Length; ++i)
                    relative += open.FileName[i];

                /////////////////////////////////
                filePath = relative;

                //if (tool != null)
                {
                    //tool.UpdateTileset(tileSet, new Size((int)nTileW.Value, (int)nTileH.Value), new Size((int)nTileSetW.Value, (int)nTileSetH.Value));


                }
            }
        }

        private void button7_Click(object sender, EventArgs e)
        {
            OpenFileDialog open = new OpenFileDialog();

            if (DialogResult.OK == open.ShowDialog())
            {
               
                NextLvlFilename  = open.FileName;
                int i;
                for ( i = open.FileName.Length - 1; i >= 0; --i)
                {
                    if (NextLvlFilename[i] == '\\')
                        break;

                }
                string relative = "resource\\graphics";

                for (; i < open.FileName.Length; ++i)
                    relative += NextLvlFilename[i];

                label17.Text = NextLvlFilename = relative;
            }

        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog save = new SaveFileDialog();
            save.Filter = "Lapidem Level Map ( *.laplvl )|*.laplvl";
            save.FileName = "";


            if (DialogResult.OK == save.ShowDialog())
            {
                FileStream output = new FileStream(save.FileName, FileMode.Create);
                BinaryWriter bWriter = new BinaryWriter(output);

                /*
                 *      Version test
                 *  World Dimension
                 *  Tile width
                 *  tile height
                 *      tileset filename
                 *      background filename
                 *      nextlevel filename
                 *      base tileID
                 *      tiles
                 *      events
                 */
                string version = "lapidem V2";

                //Version
               // bWriter.Write(version.Length);
                bWriter.Write(version);

                // World Dimension
                bWriter.Write(WorldCollumn);
                bWriter.Write(WorldRow);

                // Tile width and height
                bWriter.Write(TileWidth);
                bWriter.Write(TileHeight);

                // TileSet filename
               

                    bWriter.Write(filePath);

                // bg filename
                    bWriter.Write(BgFilename);

                // Next level filename
                    bWriter.Write(BgMfilenamel);
                    bWriter.Write(NextLvlFilename);

                    bWriter.Write(BaseTile);

                for( int i = 0; i < tiles.Length ;++i)
                {
                    bWriter.Write(Tiles[i].TileID);
                    bWriter.Write(Tiles[i].Type );
                }
                for (int i = 0; i < ETiles.Length ; ++i)
                {
                    bWriter.Write(ETiles[i].EventType);
                    bWriter.Write(ETiles[i].Data);
                }


                bWriter.Close();

            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog open = new OpenFileDialog();
            open.Filter = "Lapidem Level Map ( *.laplvl )|*.laplvl";
            open.FileName = "";

            if (DialogResult.OK == open.ShowDialog())
            {
                FileStream input = new FileStream(open.FileName, FileMode.Open);
                BinaryReader reader = new BinaryReader(input);

                string version;

                version = reader.ReadString();

                if (version != "lapidem V2")
                {
                    if (version == "lapidem")
                        MessageBox.Show("You have loaded a map from an older version.\n Some features may be damaged");
                    //return;
                }
                /*
                *      Version test
                *  World Dimension
                *  Tile width
                *  tile height
                *      tileset filename
                *      background filename
                *      nextlevel filename
                 *      base tile id
                *      tiles
                *      events
                */

                WorldCollumn = reader.ReadInt32();
                WorldRow = reader.ReadInt32();

                TileWidth = reader.ReadInt32();
                TileHeight = reader.ReadInt32();


                filePath = reader.ReadString();

                BgFilename = label16.Text = reader.ReadString();

                if (version != "lapidem")
                    BgMfilenamel = label18.Text = reader.ReadString();

                NextLvlFilename =label17.Text = reader.ReadString();

                BaseTile = reader.ReadInt32();

                Tiles = new Base[WorldRow * WorldCollumn];

                for (int i = 0; i < tiles.Length; ++i)
                {
                    Tiles[i].TileID = reader.ReadInt32();
                    Tiles[i].Type = reader.ReadInt32();
                }

                ETiles = new Event[WorldCollumn * WorldRow];

                for (int i = 0; i < ETiles.Length; ++i)
                {
                    ETiles[i].EventType = reader.ReadInt32();
                    ETiles[i].Data = reader.ReadInt32();
                }


                if (BgFilename  != " ")
                {
                    BgImgID = ManagedTextureManager.Instance.LoadTexture(BgFilename, 0);
                    label16.Text = BgFilename;
                }

                if (filePath != " ")
                {
                    tileSet = new Bitmap(filePath);
                    ManagedTextureManager.Instance.ReleaseTexture(TmTileSet);
                    TmTileSet = ManagedTextureManager.Instance.LoadTexture(filePath, 0);
                }


                panel_TileSet.Invalidate();
                reader.Close();
            }

        }

        private void radioButton11_CheckedChanged(object sender, EventArgs e)
        {
            EventSelection = (int)EventTypes.POWERUP;
            groupBox9.Visible = true; 
            groupAir.Visible = false;
            groupBox8.Visible = false;
        }

        private void radioButton12_CheckedChanged(object sender, EventArgs e)
        {
            PowerUpSelected = 1;
        }

        private void radioButton13_CheckedChanged(object sender, EventArgs e)
        {
            PowerUpSelected = 2;

        }

        private void radioButton14_CheckedChanged(object sender, EventArgs e)
        {
            PowerUpSelected = 3;
        }

        private void radioButton15_CheckedChanged(object sender, EventArgs e)
        {
            PowerUpSelected = 4;
        }

        private void button8_Click(object sender, EventArgs e)
        {
            OpenFileDialog open = new OpenFileDialog();

            if (DialogResult.OK == open.ShowDialog())
            {
                ///////////////////////////////
                // get relative path
                int i;
                for (i = open.FileName.Length - 1; i >= 0; --i)
                {
                    if (open.FileName[i] == '\\')
                        break;

                }
                string relative = "resource\\graphics";

                for (; i < open.FileName.Length; ++i)
                    relative += open.FileName[i];

                /////////////////////////////////
                BgMfilenamel = relative ;
                label18.Text = relative ;

            }


        }

       

    }
}
