using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace LapidemParticleEditor
{
    public partial class LapidemRand : Form
    {
        public event EventHandler Apply;
        public delegate void ApplyEventHandler(object sender, ApplyEventArgs e);

        public class ApplyEventArgs : EventArgs
        {
        }

        public LapidemRand()
        {
            InitializeComponent();
        }

        private void btnMakeItRain_Click(object sender, EventArgs e)
        {
            if (Apply != null) Apply(this, new ApplyEventArgs());
        }

        public bool StartingColor()
        {
            if (chkStartingColor.Checked)
                return true;
            else return false;
        }

        public bool EndingColor()
        {
            if (chkEndingColor.Checked)
                return true;
            else return false;
        }

        public bool PositionX()
        {
            if (chkPosX.Checked)
                return true;
            else return false;
        }

        public bool PositionY()
        {
            if (chkPosY.Checked)
                return true;
            else return false;
        }

        public bool MinLife()
        {
            if (chkMinLife.Checked)
                return true;
            else return false;
        }

        public bool MaxLife()
        {
            if (chkMaxLife.Checked)
                return true;
            else return false;
        }

        public bool StartAlpha()
        {
            if (chkStartAlpha.Checked)
                return true;
            else return false;
        }

        public bool EndAlpha()
        {
            if (chkEndAlpha.Checked)
                return true;
            else return false;
        }

        public bool VelMinX()
        {
            if (chkVelMinX.Checked)
                return true;
            else return false;
        }

        public bool VelMinY()
        {
            if (chkVelMinY.Checked)
                return true;
            else return false;
        }

        public bool VelMaxX()
        {
            if (chkVelMaxX.Checked)
                return true;
            else return false;
        }

        public bool VelMaxY()
        {
            if (chkVelMaxY.Checked)
                return true;
            else return false;
        }

        public bool HeightOfSpawn()
        {
            if (chkSpawnHeight.Checked)
                return true;
            else return false;
        }

        public bool WidthOfSpawn()
        {
            if (chkSpawnWidth.Checked)
                return true;
            else return false;
        }
    }
}
