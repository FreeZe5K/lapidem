using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Data;
using System.Drawing;

namespace AnimationEditor2._0
{
    class Animation
    {
       public struct Frame
        {
            int number;
            double duration;
            string trigger;
            Rectangle renderRect;
            Rectangle collisionRect;
            Point anchorPoint;

            public Rectangle RenderRect
            {
                get { return renderRect; }
                set { renderRect = value; }
            }

            public Rectangle CollisionRect
            {
                get { return collisionRect; }
                set { collisionRect = value; }
            }

            public Point AnchorPoint
            {
                get { return anchorPoint; }
                set { anchorPoint = value;}
            }

            public int Number
            {
                get { return number; }
                set { number = value; }
            }

            public double Duration
            {
                get { return duration; }
                set { duration = value; }
            }

            public string Trigger
            {
                get { return trigger; }
                set { trigger = value; }
            }

            public override string ToString()
            {
                return Number.ToString();
            }


        }

        string name;
        int numberOfFrames;
        List<Frame> theFrames;


        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public int NumberOfFrames
        {
            get { return numberOfFrames; }
            set { numberOfFrames = value; }
        }

        public List<Frame> TheFrames
        {
            get { return theFrames; }
        }


        public Animation(string name)
        {
            NumberOfFrames = 0;
            Name = name;
            theFrames = new List<Frame>();
        }

        public Frame AddFrame(Rectangle render, Rectangle collision, Point anchorPoint)
        {
            Frame newFrame = new Frame();
            newFrame.RenderRect = render;
            newFrame.CollisionRect = collision;
            newFrame.AnchorPoint = anchorPoint;
            newFrame.Number = ++NumberOfFrames;
            newFrame.Duration = .15;
            newFrame.Trigger = "No_Trigger";
            TheFrames.Add(newFrame);
            return newFrame;
        }

        public void RemoveFrame(int frameNumber)
        {
            bool removed = false;
            for (int index = 0; index < TheFrames.Count; ++index)
            {
                if (removed)
                {
                    Frame changeFrame = TheFrames[index];
                    changeFrame.Number -= 1;
                    TheFrames[index] = changeFrame;
                }
                if (!removed && TheFrames[index].Number == frameNumber)
                {
                    TheFrames.RemoveAt(index);
                    removed = true;
                    --NumberOfFrames;
                }
            }
        }

        public override string ToString()
        {
            return Name;
        }
    }
}
