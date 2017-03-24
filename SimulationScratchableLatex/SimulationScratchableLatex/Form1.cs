using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Diagnostics;
using Microsoft.Win32;
using AForge;
using AForge.Controls;
using AForge.Video;
using AForge.Video.DirectShow;
using System.IO;

namespace SimulationScratchableLatex
{
    public partial class Form1 : Form
    {
        //屏幕宽度
        private int width;
        //屏幕高度
        private int height;
        private int widthUnit;
        private int heightUnit;
        private static int codeNum = 4;
        //密码区域
        private int[] codes;
        //九宫格区域
        private int[] sudokus;
        //是否显示结果
        private bool showResult;
        private bool showPicture;
        private Random rm;
        private HookTool hookTool = new HookTool();
        private string dirc = System.AppDomain.CurrentDomain.BaseDirectory; //截图保存的目录  
        private string tip = "";
        //录像参数
        private int count;
        private FilterInfoCollection videoDevices;
        private VideoCaptureDevice videoSource;
        private List<string> tscbxCameras;
        private VideoSourcePlayer videPlayer;
        Bitmap img;
        private Image[] imgArr;
        private bool initCamera()
        {
            try
            {
                // 枚举所有视频输入设备  
                videoDevices = new FilterInfoCollection(FilterCategory.VideoInputDevice);
                if (videoDevices.Count == 0)
                    throw new ApplicationException();

                foreach (FilterInfo device in videoDevices)
                {
                    tscbxCameras.Add(device.Name);
                }
            }
            catch (ApplicationException)
            {
                return false;
            }
            return true;
        }
        //设置引用第几个摄像头
        private void setCamera(int index)
        {
            if(index > tscbxCameras.Count() - 1)
            {
                tip = "没有这个摄像头！";
                return;
            }
            videoSource = new VideoCaptureDevice(videoDevices[index].MonikerString);
            videoSource.DesiredFrameSize = new Size(10000, 10000);
            videoSource.DesiredFrameRate = 1;
            videPlayer.VideoSource = videoSource;
            tip = "摄像头" + index + "设置成功...";
        }
        private void video_NewFrame(object sender, NewFrameEventArgs eventArgs)
        {
            Bitmap img = (Bitmap)eventArgs.Frame.Clone();
            this.img = img;
        }
        private void openCamera()
        {
            if(videoSource == null)
            {
                tip = "请先设置摄像头！";
                return;
            }
            videPlayer.Start();
            videoSource.NewFrame += new NewFrameEventHandler(video_NewFrame);
            tip = "摄像头开启成功...";
        }
        /// <summary>  
        /// 关闭摄像头  
        /// </summary>  
        private void closeCamera()
        {
            videPlayer.SignalToStop();
            videPlayer.WaitForStop();
            tip = "摄像头已经关闭...";
        }
        //获取截图
        private void getPicture()
        {
            Bitmap img = (Bitmap)this.img.Clone();
            string str = "";
            for (int i = 0; i < codeNum; i++)
            {
                //str += sudokus.ToList().IndexOf(codes[i]) + 1;
                str += codes[i];
            }
            str += "_";
            for (int i = 0; i < 9; i++)
            {
                str += sudokus[i];
            }
            string imgPath = dirc + count++ + "_" + str + ".jpg";
            img.Save(imgPath);
            tip = "图片保存成功..." + (count - 1);
        }
        public Form1()
        {
            InitializeComponent();
            hookTool.Hook_Start();
            tscbxCameras = new List<string>();
            initCamera();
            width = Screen.PrimaryScreen.Bounds.Width;
            height = Screen.PrimaryScreen.Bounds.Height;
            widthUnit = width / 16;
            heightUnit = height / 9;
            showResult = false;
            showPicture = false;
            rm = new Random();
            videPlayer = new VideoSourcePlayer();
            timer1.Interval = 2000;

            this.FormBorderStyle = FormBorderStyle.None;
            this.WindowState = FormWindowState.Maximized;
            this.TopMost = true;
            this.DoubleBuffered = true;
            getCodeRandom();
            getSudokuRandom();
            imgArr = new Image[]{
                Properties.Resources._1,
                Properties.Resources._2,
                Properties.Resources._3,
                Properties.Resources._4,
                Properties.Resources._5,
                Properties.Resources._6,
                Properties.Resources._7,
                Properties.Resources._8,
                Properties.Resources._9
            };
        }
        //获取密码区的随机数 
        private void getCodeRandom()
        {
            codes = new int[codeNum];
            for (int i = 0; i < codeNum; i++)
            {
                codes[i] = rm.Next(1, 10);
            }
        }
        //获取九宫格的随机数
        private void getSudokuRandom()
        {
            HashSet<int> set = new HashSet<int>();
            int count = 0;
            while (true)
            {
                if (set.Add(rm.Next(1, 10))) ++count;
                if (set.Count() == 9) break;
            }
            sudokus = set.ToArray();
        }
        private void drawRect(PaintEventArgs e) 
        {
            Brush brush = new SolidBrush(Color.Black);
            Pen pen = new Pen(brush);
            pen.Width = 5;
            //16 : 9
            int codeWidth = widthUnit * 3 / 4;
            int codeHeight = heightUnit;
            int sudoKuWidth = widthUnit * 4;
            int sudoKuHeight = (int)(heightUnit * 6.5 / 3);
            //密码区
            for (int i = 0; i < codeNum; i++)
            {
                //e.Graphics.DrawRectangle(pen, new Rectangle(widthUnit * 2 + codeWidth * i,
                //    (int)(heightUnit * 0.5), codeWidth, codeHeight));

                e.Graphics.DrawString(codes[i].ToString(), new Font("黑体", 90), brush,
                    new Point(widthUnit * 2 + codeWidth * i, (int)(heightUnit * 0.5)));
                //e.Graphics.DrawImage(imgArr[codes[i] - 1], new Rectangle(widthUnit * 2 + codeWidth * i, (int)(heightUnit * 0.5), 50, 100));
            }
            //九宫格
            for(int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    e.Graphics.DrawRectangle(pen, new Rectangle(widthUnit * 2 + sudoKuWidth * j, 
                        heightUnit * 2 + sudoKuHeight * i, sudoKuWidth, sudoKuHeight));

                    e.Graphics.DrawString(sudokus[j + i * 3].ToString(), new Font("黑体", 120), brush,
                        new Point((int)(widthUnit * 2 + sudoKuWidth * j + sudoKuWidth / 3.5), heightUnit * 2 + sudoKuHeight * i + sudoKuHeight / 20));
                }
            }
            //结果显示
            if(showResult)
            {
                Brush rBrush = new SolidBrush(Color.Red);
                Pen rpen = new Pen(rBrush);
                rpen.Width = 5;
                e.Graphics.DrawRectangle(rpen, new Rectangle(widthUnit * 8, (int)(heightUnit * 0.5), (int)(sudoKuWidth * 1.5), codeHeight));
                String result = "Result:";
                for (int i = 0; i < codeNum; i++)
                {
                    result += sudokus.ToList().IndexOf(codes[i]) + 1;
                }
                e.Graphics.DrawString(result, new Font("黑体", 60), rBrush, new Point(widthUnit * 8, (int)(heightUnit * 0.5)));
            }
            //写提示
            e.Graphics.DrawString(tip, new Font("黑体", 10), new SolidBrush(Color.Red), new Point(widthUnit * 14, (int)(heightUnit * 0.5)));
            if(showPicture)
            {
                if(img != null)
                {
                    e.Graphics.DrawImage((Image)img.Clone(), new Point(0, 0));
                }
            }
            
        }
        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            drawRect(e);
            //this.Invalidate();
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Tab)
            {
                getCodeRandom();
                getSudokuRandom();
                this.Invalidate();
            }
            else if(e.Control & e.KeyCode == Keys.Q) this.Close();
            else if (e.KeyCode == Keys.F1) showResult = showResult ? false : true; this.Invalidate() ;
            if (e.KeyCode == Keys.D1 || e.KeyCode == Keys.D2 || e.KeyCode == Keys.D3 || e.KeyCode == Keys.D4 || e.KeyCode == Keys.D5)
            {
                if (e.KeyCode == Keys.D1) codeNum = 1;
                else if (e.KeyCode == Keys.D2) codeNum = 2;
                else if (e.KeyCode == Keys.D3) codeNum = 3;
                else if (e.KeyCode == Keys.D4) codeNum = 4;
                else if (e.KeyCode == Keys.D5) codeNum = 5;
                getCodeRandom();
                getSudokuRandom();
                this.Invalidate();
            }
            if (e.Control & e.KeyCode == Keys.A) { openCamera(); this.Invalidate(); }
            if (e.Control & e.KeyCode == Keys.C) closeCamera();
            if (e.Control & e.KeyCode == Keys.D1) { setCamera(0); this.Invalidate(); }
            if (e.Control & e.KeyCode == Keys.D2) { setCamera(1); this.Invalidate(); }
            if (e.Control & e.KeyCode == Keys.D3) { setCamera(2); this.Invalidate(); }
            if (e.Control & e.KeyCode == Keys.P) { getPicture(); this.Invalidate(); }
            if (e.Control & e.KeyCode == Keys.T) { timer1.Start(); tip = "自动化已开启..."; this.Invalidate(); }
            if (e.Control & e.KeyCode == Keys.Q) { timer1.Stop(); }
            if (e.Control & e.KeyCode == Keys.V) { showAPic(); }
        }

        private void showAPic()
        {
            if(img == null)
            {
                return;
            }
            showPicture = showPicture ? false : true;
        }
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            hookTool.Hook_Clear();
            closeCamera();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            getCodeRandom();
            getSudokuRandom();
            getPicture();
            this.Invalidate();
        }
    }
}
