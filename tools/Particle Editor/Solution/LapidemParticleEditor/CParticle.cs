using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using SGD;

namespace LapidemParticleEditor
{
    internal class CParticle
    {
        private Vector2 m_Acceleration;
        private bool m_bIsDead;
        private Color m_CurrColor;
        private float m_fCurrLife = 0f;
        private float m_fCurrRotation;
        private float m_fMaxLife;
        private float m_fScaleX;
        private float m_fScaleY;
        private CEmitter m_pEmiter;
        private Vector2 m_Position;
        private Vector2 m_Velocity;

        public float AccelX
        {
            get { return m_Acceleration.X; }
            set { m_Acceleration.X = value; }
        }

        public float AccelY
        {
            get { return m_Acceleration.Y; }
            set { m_Acceleration.Y = value; }
        }

        public Color curColor
        {
            get { return m_CurrColor; }
            set { m_CurrColor = value; }
        }

        public float CurrLife
        {
            get { return m_fCurrLife; }
            set { m_fCurrLife = value; }
        }

        public CEmitter Emitter
        {
            get { return m_pEmiter; }
            set { m_pEmiter = value; }
        }

        public bool IsDead
        {
            get { return m_bIsDead; }
            set { m_bIsDead = value; }
        }

        public float MaxLife
        {
            get { return m_fMaxLife; }
            set { m_fMaxLife = value; }
        }

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

        public float Rotation
        {
            get { return m_fCurrRotation; }
            set { m_fCurrRotation = value; }
        }

        public float ScaleX
        {
            get { return m_fScaleX; }
            set { m_fScaleX = value; }
        }

        public float ScaleY
        {
            get { return m_fScaleY; }
            set { m_fScaleY = value; }
        }

        public float VelX
        {
            get { return m_Velocity.X; }
            set { m_Velocity.X = value; }
        }

        public float VelY
        {
            get { return m_Velocity.Y; }
            set { m_Velocity.Y = value; }
        }

        public void Render()
        {
            ManagedTextureManager.Instance.Draw(m_pEmiter.ImgID, 
                ((int)m_Position.X) - ((int)(m_pEmiter.OffsetX * ScaleX)), 
                ((int)m_Position.Y) - ((int)(m_pEmiter.OffsetY * ScaleY)), 
                ScaleX, ScaleY, Rectangle.Empty, (int)(m_pEmiter.OffsetX * ScaleX), 
                (int)(m_pEmiter.OffsetY * ScaleY), m_fCurrRotation, m_CurrColor.ToArgb());
        }

        public void Update(double dElaspedTime)
        {
            m_fCurrLife += (float)dElaspedTime;
            if (m_fCurrLife < m_fMaxLife)
            {
                float s = m_fCurrLife / m_fMaxLife;
                m_Velocity += (Vector2)(m_Acceleration * ((float)dElaspedTime));
                m_Position += (Vector2)(m_Velocity * ((float)dElaspedTime));
                m_CurrColor = ColorOperator.Lerp(m_pEmiter.StartColor, m_pEmiter.EndColor, s);
                m_fScaleX = m_pEmiter.StartScaleX + ((m_pEmiter.EndScaleX - m_pEmiter.StartScaleX) * s);
                m_fScaleY = m_pEmiter.StartScaleY + ((m_pEmiter.EndScaleY - m_pEmiter.StartScaleY) * s);
                m_fCurrRotation = m_pEmiter.StartRot + ((m_pEmiter.EndRot - m_pEmiter.StartRot) * s);
            }
            else if (m_pEmiter.IsLooping)
            {
                m_fCurrLife -= m_fMaxLife;
                if ((m_pEmiter.MaxVelX - m_pEmiter.MinVelX) == 0f)
                    m_Velocity.X = (m_pEmiter.ran.Next() % ((int)((m_pEmiter.MaxVelX - 
                        m_pEmiter.MinVelX) + 1f))) + m_pEmiter.MinVelX;
                else
                    m_Velocity.X = (m_pEmiter.ran.Next() % ((int)(m_pEmiter.MaxVelX - 
                        m_pEmiter.MinVelX))) + m_pEmiter.MinVelX;

                if ((m_pEmiter.MaxVelY - m_pEmiter.MinVelY) == 0f)
                    m_Velocity.Y = (m_pEmiter.ran.Next() % ((int)
                        ((m_pEmiter.MaxVelY - m_pEmiter.MinVelY) + 1f))) + m_pEmiter.MinVelY;
                else
                    m_Velocity.Y = (m_pEmiter.ran.Next() % ((int)
                        (m_pEmiter.MaxVelY - m_pEmiter.MinVelY))) + m_pEmiter.MinVelY;

                m_Position.X = m_pEmiter.PosX + (m_pEmiter.ran.Next() % (m_pEmiter.XRS + 1));
                m_Position.Y = m_pEmiter.PosY + (m_pEmiter.ran.Next() % (m_pEmiter.YRS + 1));

                if ((m_pEmiter.MaxAccelX - m_pEmiter.MinAccelX) == 0f)
                    m_Acceleration.X = (m_pEmiter.ran.Next() % ((int)((m_pEmiter.MaxAccelX - 
                        m_pEmiter.MinAccelX) + 1f))) + m_pEmiter.MinAccelX;
                else
                    m_Acceleration.X = (m_pEmiter.ran.Next() % ((int)(m_pEmiter.MaxAccelX - 
                        m_pEmiter.MinAccelX))) + m_pEmiter.MinAccelX;

                if ((m_pEmiter.MaxAccelY - m_pEmiter.MinAccelY) == 0f)
                    m_Acceleration.Y = (m_pEmiter.ran.Next() % ((int)((m_pEmiter.MaxAccelY - 
                        m_pEmiter.MinAccelY) + 1f))) + m_pEmiter.MinAccelY;
                else
                    m_Acceleration.Y = (m_pEmiter.ran.Next() % ((int)(m_pEmiter.MaxAccelY - 
                        m_pEmiter.MinAccelY))) + m_pEmiter.MinAccelY;

                if ((m_pEmiter.MaxMaxLife - m_pEmiter.MinMaxLife) == 0.0)
                    m_fMaxLife = (float)((((double)(m_pEmiter.ran.Next() % 
                        ((int)(((m_pEmiter.MaxMaxLife - m_pEmiter.MinMaxLife) * 
                        10000000.0) + 1.0)))) / 10000000.0) + m_pEmiter.MinMaxLife);
                else
                    m_fMaxLife = (float)((((double)(m_pEmiter.ran.Next() % 
                        ((int)((m_pEmiter.MaxMaxLife - m_pEmiter.MinMaxLife) * 
                        10000000.0)))) / 10000000.0) + m_pEmiter.MinMaxLife);

                float num2 = m_fCurrLife / m_fMaxLife;
                m_Velocity += (Vector2)(m_Acceleration * ((float)dElaspedTime));
                m_Position += (Vector2)(m_Velocity * ((float)dElaspedTime));
                m_CurrColor = ColorOperator.Lerp(m_pEmiter.StartColor, m_pEmiter.EndColor, num2);
                m_fScaleX = m_pEmiter.StartScaleX + ((m_pEmiter.EndScaleX - m_pEmiter.StartScaleX) * num2);
                m_fScaleY = m_pEmiter.StartScaleY + ((m_pEmiter.EndScaleY - m_pEmiter.StartScaleY) * num2);
                m_fCurrRotation = m_pEmiter.StartRot + ((m_pEmiter.EndRot - m_pEmiter.StartRot) * num2);
            }
            else if (!m_bIsDead)
            {
                m_pEmiter.IncDeadPart();
                m_bIsDead = true;
                curColor = Color.FromArgb(0, 0, 0, 0);
            }
        }
    }
}