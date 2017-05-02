#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include "protocol.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <iostream>
#include "Structure.h"

using namespace std;
using namespace cv;

const int Follow_Flag = 10;
const double delta = 40;

CvBox2D detection(IplImage* img);
uint8_t getImageFromMemory(Mat &image);

int main()
{
    //CvCapture* capture = cvCreateCameraCapture(1);
    //CvCapture* capture = cvCreateFileCapture("test11.avi");
    IplImage img;
    int framecnt = 0;// ∂±µΩµƒ÷° ˝£¨¥Û”⁄Follow_Flag ±Ω¯»Î∏˙ÀÊƒ£ Ω
    CvBox2D dt;
    Mat img0;

    while (!getImageFromMemory(img0))
    {
        img = IplImage(img0);

        CvBox2D result;
        if (framecnt >= Follow_Flag)
        {
            double kx = dt.center.x - dt.size.height / 2 - delta / 2;
            double ky = dt.center.y - dt.size.width / 2 - delta / 2;
            cvSetImageROI(&img, cvRect(kx, ky, dt.size.height + delta, dt.size.width + delta));//…Ë÷√∏––À»§«¯”Ú
            IplImage *img2 = cvCreateImage(cvGetSize(&img), img.depth, img.nChannels);//¥¥Ω®“ª∏ˆ–¬µƒ”√¿¥œ‘ ææ÷≤ø
            cvCopy(&img, img2, NULL);
            cvResetImageROI(&img);

            result = detection(img2);
            if (result.center.x == -1 && result.center.y == -1)
            {
                result = detection(&img);
                dt = result;
                framecnt = 1;
            }
            else{
                result.center.x += kx;
                result.center.y += ky;
                dt = result;
                framecnt = 11;
            }
        }
        else{
            result = detection(&img);
            double dx = dt.center.x;
            double dy = dt.center.y;
            double t = abs(dx - result.center.x) + abs(dy - result.center.y);//”Î…œ“ª÷°ÕºœÒ◊ˆ±»Ωœ
            if ((result.center.x == -1 && result.center.y == -1) || t > delta)
            {
                framecnt = 1;
            }
            else framecnt++;
            dt = result;
        }
        //cout << framecnt << endl;
        //cout << result.center.x << ' ' << result.center.y << endl;
        TarmacSend(result.center.x, result.center.y);
        if (result.center.x != -1 && result.center.y != -1)
            cvCircle(&img, Point(result.center.x, result.center.y), 2, CV_RGB(0, 255, 255), 10, 8, 0);
        cvShowImage("djshffdsssszss", &img);
        cvWaitKey(0);
    }
    return 0;
}
