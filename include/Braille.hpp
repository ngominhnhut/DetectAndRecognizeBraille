#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <dirent.h>
#include <fstream>
#include <string> 
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/types.hpp>


#include "DetectAndRegconize.h"
#include "rt_nonfinite.h"
#include "tiengviet.h"
#include "tiengviet_terminate.h"
#include "tiengviet_initialize.h"



using namespace cv;
using namespace std;
void kt()
{
	cout << "done"  << endl;
}

double GocToiUu(Mat thres3)// tìm góc tối uu voi moi buoc la 0.5 do, -10 -> 10 do
{
	Mat imageskew;
	double giatri = 0, h = 0, t = 0, theta = 0;
	for (float i = -20; i <= 20; i++)
	{
		Point2f center(thres3.cols / 2, thres3.rows / 2); // find center
		Mat matRot = getRotationMatrix2D(center, i / 2, 1);
		warpAffine(thres3, imageskew, matRot, thres3.size());
		for (int x = 0; x < imageskew.cols; x++)
		{
			for (int y = 0; y < imageskew.rows; y++)
			{
				Scalar intensity1 = imageskew.at<uchar>(y, x);
				giatri += intensity1.val[0];
			}
			if (giatri <= 3*255)
				h++;
			giatri = 0;
		}
		cout << "h" << i/2 <<"=" << h << endl;
		if (h >= t)
		{
			t = h;
			theta = i / 2;
		}
		h = 0;
	}
	cout << "goc toi uu =" << theta << endl;
	return theta;
}

Mat xoay_goc(Mat imgin)
{
	Mat imgout;
	Point2f center(imgin.cols / 2, imgin.rows / 2); // find center
	Mat matRot = getRotationMatrix2D(center,GocToiUu(imgin), 1);
	warpAffine(imgin, imgout, matRot, imgin.size());	
	return imgout;
}

int kt_so_cell(Mat in, int start)
{
	int m=0;
	cv::CascadeClassifier cascade;
	if (!cascade.load("classifier/cascade.xml")) 
	{
		std::cout << "Error when loading the cascade classfier!"
			<< std::endl;	
		return -1;
	}
	std::vector< Rect >  detections;
	for (int x = start; x < in.cols - 25; x+=30)
	{
		Mat cut = in(Rect(x, 0,25,in.rows));
		cascade.detectMultiScale(cut, detections, 1.05, 3, 0, cv::Size(24, 34), cv::Size(25, 38));
		if (detections.size() >= 1)
			m++;
	}
	return m;
}

void loi_lom(Mat In, Mat recto)
{
    int width = In.cols;
    int height = In.rows;
    int kt=0,x,y;
    int r[6]={255,255,255,130,130,130};
    for (x=0; x< width;x++)
    	for (y=0; y< height-6; y++)
	{
	   for (int i=0; i<6; i++)
	   {
		Scalar pixel = In.at<uchar>(y+i, x);
		if (pixel.val[0] == r[i])
		    kt =1;
		else
		{
		    kt =0;
		    break;
	        }
  	   }   
	   if (kt==1)
	   {
		kt =0;
		for (int i =0; i<6; i++)
		    recto.at<uchar>(Point(x,y+i)) = 255;
		y=y+5;
	   }		 
	}
}

void cat_nut(Mat In, Mat out)
{
    int width = In.cols;
    int height = In.rows;
    int kt=0,t=0,x,y;
    for (x=0; x< width;x++)
    	for (y=0; y< height; y++)
	{
	   Scalar pixel = In.at<uchar>(y, x);  
	   if (pixel.val[0] == 0)
	   {
		t=0;
		kt=0;
	   }
	   else if (pixel.val[0] != t)
	   {
	   t=pixel.val[0];
	   kt++;
	   }
	   if (kt>=3)
	   {
		kt =1; 
		In.at<uchar>(Point(x,y)) = 0;}		 
	   }
    out =In;
}

void xu_ly_2mat(Mat imgs,Mat recto)
{
//tìm vùng sáng (ngưỡng thích nghi, lọc nhiễu)  
    Mat imgada2;
    adaptiveThreshold(imgs, imgada2, 255, ADAPTIVE_THRESH_GAUSSIAN_C , THRESH_BINARY ,5,-7); 
    GaussianBlur(imgada2, imgada2, Size(7, 7), 0);
    threshold(imgada2,imgada2, 100, 255, THRESH_BINARY);
    dilate(imgada2, imgada2, Mat(), Point(-1, -1), 1, 1, 1);

//tìm vùng tối (ngưỡng thích nghi, lọc nhiễu, +125) 
    Mat imgada;
    adaptiveThreshold(imgs, imgada, 255, ADAPTIVE_THRESH_GAUSSIAN_C , THRESH_BINARY_INV ,5,7);
    GaussianBlur(imgada, imgada, Size(5, 5), 0);
    threshold(imgada,imgada, 155, 255, THRESH_BINARY);
    dilate(imgada, imgada, Mat(), Point(-1, -1), 3, 1, 1);
    imgada =imgada -125;

//ảnh có 3 mức 
    Mat imgRe = imgada2|imgada;
//phân những nút bị dính   
    cat_nut(imgRe, imgRe);
//tìm nút lồi theo mặt nạ 1x6 (255,255,255,125,125,125)   
    loi_lom(imgRe, recto);
}

void xu_ly_2mat_data(Mat imgs,Mat recto)
{
//tìm vùng sáng (ngưỡng thích nghi, lọc nhiễu)  
    Mat imgada2;
    adaptiveThreshold(imgs, imgada2, 255, ADAPTIVE_THRESH_GAUSSIAN_C , THRESH_BINARY ,5,-7); 
    GaussianBlur(imgada2, imgada2, Size(7, 7), 0);
    threshold(imgada2,imgada2, 50, 255, THRESH_BINARY);
    dilate(imgada2, imgada2, Mat(), Point(-1, -1), 1, 1, 1);

//tìm vùng tối (ngưỡng thích nghi, lọc nhiễu, +125) 
    Mat imgada;
    adaptiveThreshold(imgs, imgada, 255, ADAPTIVE_THRESH_GAUSSIAN_C , THRESH_BINARY_INV ,5,7);
    GaussianBlur(imgada, imgada, Size(5, 5), 0);
    threshold(imgada,imgada, 100, 255, THRESH_BINARY);
    dilate(imgada, imgada, Mat(), Point(-1, -1), 3, 1, 1);
    imgada =imgada -125;

//ảnh có 3 mức 
    Mat imgRe = imgada2|imgada;
//phân những nút bị dính   
    cat_nut(imgRe, imgRe);
//tìm nút lồi theo mặt nạ 1x6 (255,255,255,125,125,125)   
    loi_lom(imgRe, recto);
}

void Get(Mat imgin, double IMAGE[560])
{
	int M = imgin.size().height;	
	int N = imgin.size().width;		
	int x, y, i = 0, t;
	for (x = 0; x < N; x++)
		for (y = 0; y < M; y++)
		{
			IMAGE[i] = imgin.at<uchar>(y, x);
			i++;
		}
}

double regconize_char(Mat imgin)
{
	cv::resize(imgin, imgin, cv::Size(20,28));
	double IMAGE[560];
	Get(imgin, IMAGE);
	double result = tiengviet(IMAGE);
	waitKey(0);	
}

string printf_Recog(double res_regco)
{
	string result;
	int kt = res_regco -1;
	switch (kt)
	{
		case 0:
		{
			result = "&";
			break;
		}
		case 1:
		{
			result = "?";
			break;
		}
		case 2:
		{
			result = "l";
			break;
		}
		case 3:
		{
			result = ".";
			break;
		}
		case 4:
		{
			result = "___";
			break;
		}
		case 5:
		{
			result = " ";
			break;
		}
		case 6:
		{
			result = "r";
			break;
		}
		case 7:
		{
			result = "y";
			break;
		}
		case 8:
		{
			result = "ă";
			break;
		}
		case 9:
		{
			result = "t";
			break;
		}
		case 10:
		{
			result = "ơ";
			break;
		}
		case 11:
		{
			result = ",";
			break;
		}
		case 12:
		{
			result = "u";
			break;
		}
		case 13:
		{
			result = "q";
			break;
		}
		case 14:
		{
			result = "i";
			break;
		}
		case 15:
		{
			result = "v";
			break;
		}
		case 16:
		{
			result = "<'>";
			break;
		}
		case 17:
		{
			result = "b";
			break;
		}
		case 18:
		{
			result = "h";
			break;
		}
		case 19:
		{
			result = ";";
			break;
		}
		case 20:
		{
			result = "m";
			break;
		}
		case 21:
		{
			result = "<HiLap>";
			break;
		}
		case 22:
		{
			result = "k";
			break;
		}
		case 23:
		{
			result = "<i>";
			break;
		}
		case 24:
		{
			result = "ê";
			break;
		}
		case 25:
		{
			result = '"';
			break;
		}
		case 26:
		{
			result = "0";
			break;
		}
		case 27:
		{
			result = ":";
			break;
		}
		case 28:
		{
			result = "<.>";
			break;
		}
		case 29:
		{
			result = "<num>";
			break;
		}
		case 30:
		{
			result = "s";
			break;
		}
		case 31:
		{
			result = "<~>";
			break;
		}
		case 32:
		{
			result = "<end>";
			break;
		}
		case 33:
		{
			result = "e";
			break;
		}
		case 34:
		{
			result = "g";
			break;
		}
		case 35:
		{
			result = "n";
			break;
		}
		case 36:
		{
			result = "<Hoa>";
			break;
		}
		case 37:
		{
			result = "â";
			break;
		}
		case 38:
		{
			result = "o";
			break;
		}
		case 39:
		{
			result = '"';
			break;
		}
		case 40:
		{
			result = "d";
			break;
		}
		case 41:
		{
			result = "/";
			break;
		}
		case 42:
		{
			result = "c";
			break;
		}
		case 43:
		{
			result = "<\\>";
			break;
		}
		case 44:
		{
			result = "ư";
			break;
		}
		case 45:
		{
			result = "<B>";
			break;
		}
		case 46:
		{
			result = "!";
			break;
		}
		case 47:
		{
			result = "...";
			break;
		}
		case 48:
		{
			result = "<`>";
			break;
		}
		case 49:
		{
			result = "ô";
			break;
		}
		case 50:
		{
			result = "b";
			break;
		}
		case 51:
		{
			result = "x";
			break;
		}
		case 52:
		{
			result = "đ";
			break;
		}
		case 53:
		{
			result = "a";
			break;
		}
		default:
			result = "No_Regconize";
	}
	return result;
}

void write_txt(cv::String in)
{
  ofstream myfile ("text.txt");
  if (myfile.is_open())
  {
    myfile << in;
    myfile.close();
  }
  else cout << "Unable to open file";
}

int kt_cell(Mat imgin)
{
	int result = (imgin.rows-36)/2, giatri;
	for (int x = 0; x < imgin.rows-38; x++)
	{
		giatri=0;
		for (int y = 0; y < imgin.cols; y++)
		{
			Scalar intensity1 = imgin.at<uchar>(x, y);
			giatri += intensity1.val[0];
		}
		if (giatri > 255)
		{
			result =x-3;
			break;
		}
		for (int y = 0; y < imgin.cols; y++)
		{
			Scalar intensity1 = imgin.at<uchar>(imgin.rows-x, y);
			giatri += intensity1.val[0];
		}
		if (giatri > 255)
		{
			result = imgin.rows-(x+32);
			break;
		}
	}
	return result;
}

String Detect_Regco(Mat imgin)
{
	double result;
	cv::String te, text;
	int m = 0, t=0, t2=0;
	// tìm điểm bắt đầu (t2)
	for (int i =0; i<30;i++)
	{
		m = kt_so_cell(imgin,i);
		if (m>t)
		{
			t=m;
			t2=i;
		}
	}
	// cửa sổ chạy kt cell
	for (int x = t2; x < imgin.cols - 25; x=x+30)
	{
		Rect windows(x, 0, 25, imgin.rows);
		Mat cell = imgin( windows);
		Rect windows2(x, kt_cell(cell), 25, 35);
		cell = imgin( windows2);
		
		result = regconize_char(cell);
		cout << printf_Recog(result);
		te = text;
		text = te+printf_Recog(result);	
		rectangle(imgin, windows2, Scalar(255), 1, 8, 0);
	}
	cout << endl;
	return text+"\n";
}

void data_augment(Mat In, Mat Out, int randx, int randy, int angle)
{
    Point2f center(In.cols / 2, In.rows / 2); // find center
    Mat matRot = getRotationMatrix2D(center,angle, 1);
    warpAffine(In, In, matRot, In.size());
    int width = In.cols;
    int height = In.rows;
    for (int x=0; x< width;x++)
    	for (int y=0; y< height; y++)
	{
		Scalar pixel = In.at<uchar>(y, x);
		if (x+randx>=0 && x+randx<width && y+randy>=0 && y+randy<height)
		Out.at<uchar>(Point(x+randx,y+randy)) = pixel.val[0]; 
	}
}

void crop_hang_DR(Mat imgin)
{
	int name =0;
	cv::String text, t;
	vector<Mat> crop;
	int giatri = 0,i=0;
	for (int x = 0; x < imgin.rows; x++)
	{
		giatri=0;
		for (int y = 0; y < imgin.cols; y++)
		{
			Scalar intensity1 = imgin.at<uchar>(x, y);
			giatri += intensity1.val[0];
		}
		if (giatri > 255)
		{
			Mat aa = imgin(cv::Rect(0, x-5, imgin.cols, 45));
			crop.push_back(aa);
			t = text;
			text = t + Detect_Regco(aa);
			i++;
			x+=45;
		}
		imwrite("a/???.jpg",imgin); 
	}
	write_txt(text);
}

void am_thanh()
{
	system("espeak -v vi-vn-x-south -f /home/ngo/hk2_4/DetectAndRegconize/build/text.txt");		
}




