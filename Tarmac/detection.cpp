#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include<vector>
#include<iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

//int cnt = 0;
//double ans = 0;
const float pi = 3.1415926;
const int N = 100;
const float rn = 60.0;

bool checkred(CvScalar s)
{
	//if (s.val[0] > 100 && s.val[0]<124 && s.val[1]>43 && s.val[1]<255 && s.val[2]>46 && s.val[2] < 255)
	if ((((s.val[0] > 0) && (s.val[0]<8)) || (s.val[0]>120) && (s.val[0]<180)) && (s.val[1]>43 && s.val[1]<255) && (s.val[2]>46 && s.val[2] < 255))
		return 1;
	return 0;
}

bool checkblue(CvScalar s)
{
	if (s.val[0] > 100 && s.val[0]<124 && s.val[1]>43 && s.val[1]<255 && s.val[2]>46 && s.val[2] < 255)
		//if ((((s.val[0] > 0) && (s.val[0]<8)) || (s.val[0]>120) && (s.val[0]<180)) && (s.val[1]>43 && s.val[1]<255) && (s.val[2]>46 && s.val[2] < 255))
		return 1;
	return 0;
}

IplImage* colorFilter(IplImage *image)
{
	int i, j;
	IplImage* hsv = cvCreateImage(cvGetSize(image), 8, 3);

	cvCvtColor(image, hsv, CV_BGR2HSV);
	int width = hsv->width;
	int height = hsv->height;
	for (i = 0; i < height; i++)
	for (j = 0; j < width; j++)
	{
		CvScalar s_hsv = cvGet2D(hsv, i, j);//��ȡ���ص�Ϊ��j, i�����HSV��ֵ 
		/*
		opencv ��H��Χ��0~180����ɫ��H��Χ�����(0~8)��(160,180)
		S�Ǳ��Ͷȣ�һ���Ǵ���һ��ֵ,S���;��ǻ�ɫ���ο�ֵS>80)��
		V�����ȣ����;��Ǻ�ɫ�����߾��ǰ�ɫ(�ο�ֵ220>V>50)��
		*/
		CvScalar s;
		//if (!(((s_hsv.val[0]>0) && (s_hsv.val[0]<8)) || (s_hsv.val[0]>120) && (s_hsv.val[0]<180)))
		if (!checkred(s_hsv))
		{
			s.val[0] = 0;
			s.val[1] = 0;
			s.val[2] = 0;
			cvSet2D(hsv, i, j, s);
		}
	}
	//	IplImage* outputImage = cvCreateImage(hsv->height, hsv->width, CV_8UC3);
	//	cvConvert(hsv, outputImage);
	//cvNamedWindow("filter");
	//cvShowImage("filter", hsv);
	return hsv;
	cvReleaseImage(&hsv);
	/*cvWaitKey(0);
	cvReleaseImage(&hsv);*/
}


float L(float a, float b, float angle)
{
	return (a*b) / sqrt(a*a*sin(angle)*sin(angle) + b*b*cos(angle)*cos(angle));
}

vector<CvBox2D> findcircle(IplImage* img, CvMemStorage* storage)
{
	vector<CvBox2D> vEllipse;
	CvSeq* contours;//��Ե
	IplImage* img_canny = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* img_dilate = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* img_erode = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* timg = cvCloneImage(img);//����һ��img	
	CvBox2D result;

	//��Ե���
	cvCanny(img, img_canny, 40, 120, 3);//��Ե���  
	//cvShowImage("canny", img_canny);

	cvDilate(img_canny, img_dilate, NULL, 1); //����   
	cvErode(img_dilate, img_erode, NULL, 1); //��ʴ   
	//cvShowImage("img_dilate", img_erode);

	cvFindContours(img_erode, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
	// �����������     
	//cvDrawContours(timg, contours, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));
	//cvShowImage("����ͼ", timg);
	//cvWaitKey(10000);

	while (contours)
	{ //����αƽ�  
		if (contours->total > 10)
		{
			result = cvFitEllipse2(contours);
			//if (result.size.height > 60 && result.size.height < 150 && result.size.width>60 && result.size.width < 150)
			//if (1)
			float sigm = 0;
			double lll;
			for (int i = 1; i <= N; i++)
			{
				float fi;
				float L1 = L(result.size.height / 2, result.size.width / 2, 2.0 * pi / i);
				fi = abs(rn - L1) / L1;
				sigm += fi;
				lll = L1;
			}
			sigm /= N;
			if (sigm <= 0.35 && result.size.height > 80 && result.size.height < 160 && result.size.width > 80 && result.size.width < 160){
				//if (sigm <= 0.2){
				if (((result.size.height / result.size.width > 1) && (result.size.height / result.size.width < 1.3)) || ((result.size.width / result.size.height > 1) && (result.size.width / result.size.height < 1.3)))
				{
					//ans += lll;
					//cnt++;
					//cout << lll << endl;
					vEllipse.push_back(result);
					//cvEllipseBox(timg, result, CV_RGB(255, 255, 0), 10, 8, 0);
				}
			}
		}
		contours = contours->h_next;
	}
	//cvShowImage("����ͼ", timg);
	//cvWaitKey(10000);
	cvReleaseImage(&img_erode);
	cvReleaseImage(&img_dilate);
	cvReleaseImage(&img_canny);
	cvReleaseImage(&timg);
	return vEllipse;
}

CvBox2D detection(IplImage* img)
{
	IplImage* img_hsv = NULL;
	CvMemStorage* storage = 0;
	vector<CvBox2D> result;
	storage = cvCreateMemStorage(0);
	img_hsv = colorFilter(img);
	result = findcircle(img_hsv, storage);
	CvBox2D ans;//���ص����ĵ�����
	
	if (result.size() != 0)
	{
		double dx, dy;
		dx = result[0].center.x;
		dy = result[0].center.y;
		cvCircle(img, Point(dx, dy), 2, CV_RGB(0, 255, 255), 10, 8, 0);
					//���ĵ�����
		ans = result[0];
		//cout << result[dk].center.x << ' ' << result[dk].center.y << endl;
	}
	else{
		ans.center.x = -1;
		ans.center.y = -1;
	}
	cvShowImage("shdlfj", img);
	cvReleaseImage(&img_hsv);
	cvClearMemStorage(storage);//��մ洢
			
	return ans;
}

