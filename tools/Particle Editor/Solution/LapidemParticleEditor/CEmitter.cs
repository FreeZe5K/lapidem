using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Drawing;
using System.IO;
using SGD;

namespace LapidemParticleEditor
{
    public class CEmitter
    {
        private CParticle[] m_ArrayOfPart;
        private Color m_BGColor;
        private bool m_bIsLooping;
        private double m_dParticleLifeMax;
        private double m_dParticleLifeMin;
        private Vector2 m_EndAccel;
        private Color m_EndColor;
        private float m_fEndRot;
        private float m_fEndXScale;
        private float m_fEndYScale;
        private float m_fStartingRot;
        private float m_fStartXScale;
        private float m_fStartYScale;
        private string m_ImgFileName = "";
        private Vector2 m_MinVel;
        private int m_nBGImgID = -1;
        private int m_nDeadParticles;
        private int m_nDestinationBlend = 5;
        private int m_nImgID;
        private int m_nNumParticles;
        private int m_nSourceBlend = 4;
        private int m_nXSpawnRan;
        private int m_nYSpawnRan;
        private int m_OffsetX;
        private int m_OffsetY;
        private Vector2 m_Position;
        private Random m_rand = new Random();
        private Vector2 m_StartAccel;
        private Color m_StartColor;
        private Vector2 m_StartVeloity;

        public CEmitter()
        {
            ImgID = ManagedTextureManager.Instance.LoadTexture("default.jpg", 0);
            m_ImgFileName = "default.jpg";
            m_BGColor = Color.FromArgb(0, 0, 0, 0);
            m_nDeadParticles = 0;
            m_nNumParticles = 200;
            m_ArrayOfPart = new CParticle[m_nNumParticles];

            StartColor = Color.FromArgb(255, 255, 0, 0);
            EndColor = Color.FromArgb(0, 255, 128, 0);

            StartRot = 0f;
            EndRot = 0f;

            StartScaleX = 0.3f;
            StartScaleY = 0.3f;

            EndScaleX = 1f;
            EndScaleY = 1f;

            XRS = 1;
            YRS = 1;

            PosX = 350f;
            PosY = 300f;

            MinVelX = -100f;
            MinVelY = -100f;
            MaxVelX = 100f;
            MaxVelY = 100f;

            MinMaxLife = 1.0;
            MaxMaxLife = 1.0;

            MinAccelX = 0f;
            MinAccelY = 0f;
            MaxAccelX = 0f;
            MaxAccelY = 0f;

            m_bIsLooping = true;

            for (int i = 0; i < m_nNumParticles; i++)
            {
                m_ArrayOfPart[i] = new CParticle();

                m_ArrayOfPart[i].AccelX = ((float)(((double)(m_rand.Next() % 
                    ((int)(((m_StartAccel.X - m_EndAccel.X) + 1f) * 1000f)))) 
                    / 1000.0)) + m_EndAccel.X;
                m_ArrayOfPart[i].AccelY = ((float)(((double)(m_rand.Next() % 
                    ((int)(((m_StartAccel.Y - m_EndAccel.Y) + 1f) * 1000f)))) 
                    / 1000.0)) + m_EndAccel.Y;

                m_ArrayOfPart[i].curColor = m_StartColor;

                if (m_dParticleLifeMax == m_dParticleLifeMin)
                    m_ArrayOfPart[i].MaxLife = (float)m_dParticleLifeMax;
                else
                    m_ArrayOfPart[i].MaxLife = (float)((((double)(m_rand.Next() % 
                        ((int)((m_dParticleLifeMax - m_dParticleLifeMin) * 1000.0)))) 
                        / 1000.0) + m_dParticleLifeMin);

                m_ArrayOfPart[i].CurrLife = (float)(((double)(m_rand.Next() % 
                    ((int)(m_ArrayOfPart[i].MaxLife * 1000f)))) / 1000.0);
                m_ArrayOfPart[i].Emitter = this;
                m_ArrayOfPart[i].PosX = m_Position.X + (m_rand.Next() % (m_nXSpawnRan + 1));
                m_ArrayOfPart[i].PosY = m_Position.Y + (m_rand.Next() % (m_nYSpawnRan + 1));
                m_ArrayOfPart[i].ScaleX = m_fStartXScale;
                m_ArrayOfPart[i].ScaleY = m_fStartYScale;

                if (m_StartVeloity.Y == m_MinVel.Y)
                    m_ArrayOfPart[i].VelY = m_StartVeloity.Y;
                else
                    m_ArrayOfPart[i].VelY = ((m_rand.Next() % ((int)
                        (m_StartVeloity.Y - m_MinVel.Y))) + 1) + m_MinVel.Y;

                if (m_StartVeloity.X == m_MinVel.X)
                    m_ArrayOfPart[i].VelX = m_StartVeloity.X;
                else
                    m_ArrayOfPart[i].VelX = ((m_rand.Next() % ((int)
                        (m_StartVeloity.X - m_MinVel.X))) + 1) + m_MinVel.X;

                m_ArrayOfPart[i].IsDead = false;
            }
        }

        public void IncDeadPart()
        {
            m_nDeadParticles++;
        }

        public void Render()
        {
            if (m_nBGImgID != -1)
                ManagedTextureManager.Instance.Draw(m_nBGImgID, 0, 0, 1f, 1f, Rectangle.Empty, 0, 0, 0f, -1);

            ManagedDirect3D.Instance.Sprite.Flush();

            int destinationBlend = (int)ManagedDirect3D.Instance.Device.RenderState.DestinationBlend;
            int sourceBlend = (int)ManagedDirect3D.Instance.Device.RenderState.SourceBlend;

            ManagedDirect3D.Instance.Device.RenderState.DestinationBlend = (Blend)DestinationBlend;
            ManagedDirect3D.Instance.Device.RenderState.SourceBlend = (Blend)SourceBlend;

            if (m_nDeadParticles < m_nNumParticles)
                for (int i = 0; i < m_nNumParticles; i++)
                    m_ArrayOfPart[i].Render();

            ManagedDirect3D.Instance.Sprite.Flush();
            ManagedDirect3D.Instance.Device.RenderState.DestinationBlend = (Blend)destinationBlend;
            ManagedDirect3D.Instance.Device.RenderState.SourceBlend = (Blend)sourceBlend;
        }

        public void Reset()
        {
            m_ArrayOfPart = new CParticle[m_nNumParticles];
            m_nDeadParticles = 0;
            for (int i = 0; i < m_nNumParticles; i++)
            {
                m_ArrayOfPart[i] = new CParticle();
                if ((m_StartAccel.X - m_EndAccel.X) == 0f)
                    m_ArrayOfPart[i].AccelX = ((float)(((double)(m_rand.Next() % 
                        ((int)(((m_StartAccel.X - m_EndAccel.X) + 1f) * 1E+07f)))) 
                        / 10000000.0)) + m_EndAccel.X;
                else
                    m_ArrayOfPart[i].AccelX = ((float)(((double)(m_rand.Next() % 
                        ((int)((m_StartAccel.X - m_EndAccel.X) * 1E+07f)))) 
                        / 10000000.0)) + m_EndAccel.X;
                if ((m_StartAccel.Y - m_EndAccel.Y) == 0f)
                    m_ArrayOfPart[i].AccelY = ((float)(((double)(m_rand.Next() % 
                        ((int)(((m_StartAccel.Y - m_EndAccel.Y) + 1f) * 1E+07f)))) 
                        / 10000000.0)) + m_EndAccel.Y;
                else
                    m_ArrayOfPart[i].AccelY = ((float)(((double)(m_rand.Next() % 
                        ((int)((m_StartAccel.Y - m_EndAccel.Y) * 1E+07f)))) 
                        / 10000000.0)) + m_EndAccel.Y;

                m_ArrayOfPart[i].curColor = m_StartColor;

                if (m_dParticleLifeMax == m_dParticleLifeMin)
                    m_ArrayOfPart[i].MaxLife = (float)m_dParticleLifeMax;
                else
                    m_ArrayOfPart[i].MaxLife = (float)((((double)(m_rand.Next() % 
                        ((int)((m_dParticleLifeMax - m_dParticleLifeMin) * 1000000.0)))) 
                        / 10000000.0) + m_dParticleLifeMin);

                m_ArrayOfPart[i].CurrLife = (float)(((double)(m_rand.Next() % 
                    ((int)(m_ArrayOfPart[i].MaxLife * 1E+07f)))) / 10000000.0);
                m_ArrayOfPart[i].Emitter = this;
                m_ArrayOfPart[i].PosX = m_Position.X + (m_rand.Next() % (m_nXSpawnRan + 1));
                m_ArrayOfPart[i].PosY = m_Position.Y + (m_rand.Next() % (m_nYSpawnRan + 1));
                m_ArrayOfPart[i].ScaleX = m_fStartXScale;
                m_ArrayOfPart[i].ScaleY = m_fStartYScale;

                if (m_StartVeloity.Y == m_MinVel.Y)
                    m_ArrayOfPart[i].VelY = m_StartVeloity.Y;
                else
                    m_ArrayOfPart[i].VelY = ((m_rand.Next() % ((int)
                        (m_StartVeloity.Y - m_MinVel.Y))) + 1) + m_MinVel.Y;

                if (m_StartVeloity.X == m_MinVel.X)
                    m_ArrayOfPart[i].VelX = m_StartVeloity.X;
                else
                    m_ArrayOfPart[i].VelX = ((m_rand.Next() % ((int)
                        (m_StartVeloity.X - m_MinVel.X))) + 1) + m_MinVel.X;

                m_ArrayOfPart[i].IsDead = false;
            }
        }

        public void Update(double dElaspedTime)
        {
            if (m_nDeadParticles < m_nNumParticles)
                for (int i = 0; i < m_nNumParticles; i++)
                    m_ArrayOfPart[i].Update(dElaspedTime);
        }

        public Color BGColor
        {
            get { return m_BGColor; }
            set { m_BGColor = value; }
        }

        public int BGImgID
        {
            get { return m_nBGImgID; }
            set { m_nBGImgID = value; }
        }

        public int DestinationBlend
        {
            get { return m_nDestinationBlend; }
            set { m_nDestinationBlend = value; }
        }

        public Color EndColor
        {
            get { return m_EndColor; }
            set { m_EndColor = value; }
        }

        public float EndRot
        {
            get { return m_fEndRot; }
            set { m_fEndRot = value; }
        }

        public float EndScaleX
        {
            get { return m_fEndXScale; }
            set { m_fEndXScale = value; }
        }

        public float EndScaleY
        {
            get { return m_fEndYScale; }
            set { m_fEndYScale = value; }
        }

        public string ImgFileName
        {
            get { return m_ImgFileName; }
            set { m_ImgFileName = value; }
        }

        public int ImgID
        {
            get { return m_nImgID; }
            set
            {
                m_nImgID = value;
                m_OffsetX = ManagedTextureManager.Instance.GetTextureWidth(m_nImgID) / 2;
                m_OffsetY = ManagedTextureManager.Instance.GetTextureHeight(m_nImgID) / 2;
            }
        }

        public bool IsLooping
        {
            get { return m_bIsLooping; }
            set { m_bIsLooping = value; }
        }

        public float MaxAccelX
        {
            get { return m_StartAccel.X; }
            set { m_StartAccel.X = value; }
        }

        public float MaxAccelY
        {
            get { return m_StartAccel.Y; }
            set { m_StartAccel.Y = value; }
        }

        public double MaxMaxLife
        {
            get { return m_dParticleLifeMax; }
            set { m_dParticleLifeMax = value; }
        }

        public int MaxParticles
        {
            get { return m_nNumParticles; }
            set { m_nNumParticles = value; Reset(); }
        }

        public float MaxVelX
        {
            get { return m_StartVeloity.X; }
            set { m_StartVeloity.X = value; }
        }

        public float MaxVelY
        {
            get { return m_StartVeloity.Y; }
            set { m_StartVeloity.Y = value; }
        }

        public float MinAccelX
        {
            get { return m_EndAccel.X; }
            set { m_EndAccel.X = value; }
        }

        public float MinAccelY
        {
            get { return m_EndAccel.Y; }
            set { m_EndAccel.Y = value; }
        }

        public double MinMaxLife
        {
            get { return m_dParticleLifeMin; }
            set { m_dParticleLifeMin = value; }
        }

        public float MinVelX
        {
            get { return m_MinVel.X; }
            set { m_MinVel.X = value; }
        }

        public float MinVelY
        {
            get { return m_MinVel.Y; }
            set { m_MinVel.Y = value; }
        }

        public int OffsetX
        { get { return m_OffsetX; } }

        public int OffsetY
        { get { return m_OffsetY; } }

        public float PosX
        {
            get { return m_Position.X; }
            set { m_Position.X = value; }
        }

        public float PosY
        {
            get { return m_Position.Y; }
            set { m_Position.Y = value; }
        }

        public Random ran
        { get { return m_rand; } }

        public int SourceBlend
        {
            get { return m_nSourceBlend; }
            set { m_nSourceBlend = value; }
        }

        public Color StartColor
        {
            get { return m_StartColor; }
            set { m_StartColor = value; }
        }

        public float StartRot
        {
            get { return m_fStartingRot; }
            set { m_fStartingRot = value; }
        }

        public float StartScaleX
        {
            get { return m_fStartXScale; }
            set { m_fStartXScale = value; }
        }

        public float StartScaleY
        {
            get { return m_fStartYScale; }
            set { m_fStartYScale = value; }
        }

        public int XRS
        {
            get { return m_nXSpawnRan; }
            set { m_nXSpawnRan = value; }
        }

        public int YRS
        {
            get { return m_nYSpawnRan; }
            set { m_nYSpawnRan = value; }
        }
    }
} 