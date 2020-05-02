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
#include "Recog_Braille.h"
#include "Recog_Braille_terminate.h"
#include "Recog_Braille_initialize.h"



using namespace cv;
using namespace std;


void imadjust(const Mat1b& src, Mat1b& dst, int tol_low = 1, int tol_upp = 0, Vec2i in = Vec2i(0, 255), Vec2i out = Vec2i(0, 255))
{
    // explain : http://matlab.izmiran.ru/help/toolbox/images/enhanc17.html
    // src : input CV_8UC1 image
    // dst : output CV_8UC1 imge
    // tol : tolerance, from 0 to 100.
    // in  : src image bounds
    // out : dst image bounds

    dst = src.clone();

    tol_low = max(0, min(100, tol_low));
    tol_upp = max(0, min(100, tol_upp));
    if (tol_low > 0 || tol_upp > 0)
    {
        // Compute in and out limits

        // Histogram
        vector<int> hist(256, 0);
        for (int r = 0; r < src.rows; ++r) {
            for (int c = 0; c < src.cols; ++c) {
                hist[src(r,c)]++;
            }
        }

        // Cumulative histogram
        vector<int> cum = hist;
        for (int i = 1; i < hist.size(); ++i) {
            cum[i] = cum[i - 1] + hist[i];
        }

        // Compute bounds
        int total = src.rows * src.cols;
        int low_bound = total * tol_low / 100;
        int upp_bound = total * (100- tol_upp) / 100;
        in[0] = distance(cum.begin(), lower_bound(cum.begin(), cum.end(), low_bound));
        in[1] = distance(cum.begin(), lower_bound(cum.begin(), cum.end(), upp_bound));

    }

    // Stretching
    float scale = float(out[1] - out[0]) / float(in[1] - in[0]);
    for (int r = 0; r < dst.rows; ++r)
    {
        for (int c = 0; c < dst.cols; ++c)
        {
            int vs = max(src(r, c) - in[0], 0);
            int vd = min(int(vs * scale + 0.5f) + out[0], out[1]);
            dst(r, c) = saturate_cast<uchar>(vd);
        }
    }
}

Mat1b Preprocessing(Mat1b ImgIn, int mode = 0, String select_ROI = "Full")
{
	cout<<"Preprocessing..."<<endl;
	Mat1b ImgOut;
	Mat ImgResize;
	if (ImgIn.cols < 550)
		cv::resize(ImgIn, ImgResize, cv::Size(), 2, 2);
	else 
		ImgResize = ImgIn;
	
	Mat ImgSelect;
	if ( select_ROI != "Full")
	{
		Rect r = selectROI(ImgResize);
    	ImgSelect = ImgResize(r);
	}
	else 
		ImgSelect = ImgResize;

	cout<<" Increasing the contrast of the image..."<<endl;	
	if ( mode == 0)
		// Adjust method (low = 1%, up = 0%)
        imadjust(ImgSelect, ImgOut, 1, 0);
	else
		// Equation: g(x,y) = alpha*f(x,y) + beta (alpha = 2, beta = -170)
		ImgOut = (2 * ImgSelect) - 170;
	GaussianBlur(ImgOut, ImgOut, Size(3, 3), 0);	
	return ImgOut; 
}

int check_line(Mat ImgIn)
{
	int count = 0, value = 0;
	int width = ImgIn.cols; 
	int height = ImgIn.rows;
	for (int i = 0; i < height ; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Scalar intensity = ImgIn.at<uchar>(i,j);
			value += intensity.val[0];
		}
		if (value < 1000)
			count ++;
		value = 0; 
	}
	return count;
}

Mat skew_d(Mat ImgIn, double degrees)
{
	Mat ImgOut;
	// find center
	Point2f center(ImgIn.cols / 2, ImgIn.rows / 2); 
	Mat matRot = getRotationMatrix2D(center, degrees, 1);
	warpAffine(ImgIn, ImgOut, matRot, ImgIn.size());	
	return ImgOut;
}

// xoay trái phải 4 độ
double Rotation(Mat ImgIn)
{
	cout<<"Rotation..."<<endl;
	double result = 0;
	int value = ImgIn.rows;
	// Rotation from -5 degrees to 5 degrees with step = 0.2 degrees
	for (double i = -5; i <= 5; i+=0.2)
	{
		Mat ImgOut1 = skew_d(ImgIn, i - 4.0);
		Mat ImgOut2 = skew_d(ImgIn, i + 4.0);
		if(value > abs(check_line(ImgOut1)-check_line(ImgOut2)))\
		{
			result = i;
			value = abs(check_line(ImgOut1)-check_line(ImgOut2));
		}
	}
	cout<<" Degree = "<< result<<endl;
	return result;
}

// count line method
double Rotation2(Mat ImgIn)
{
	cout<<"Rotation2..."<<endl;
	double result = 0;
	int value = 0;
	// Rotation from -5 degrees to 5 degrees with step = 0.2 degrees
	for (double i = -5; i <= 5; i+=0.2)
	{
		Mat ImgOut = skew_d(ImgIn, i);
		if(value < check_line(ImgOut))
		{
			result = i;
			value = check_line(ImgOut);
		}
	}
	cout<<" Degree = "<< result<<endl;
	return result;
}


double Standard_deviation(Mat ImgIn)
{
	double average = 0;
	double SD;
	int value = 0, deviation, total, count;
	int width = ImgIn.cols; 
	int height = ImgIn.rows;
	for (int i = 0; i < height ; i++)
	{
		total = 0; deviation = 0; count =0;
		
		// Total rows
		for (int j = 0; j < width; j++)
		{
			Scalar intensity = ImgIn.at<uchar>(i,j);
			value = intensity.val[0];
			if (value > 250)
				{
					total += j;
					count++;
				}
		}

		if (count == 0)
			continue;
		else
		{
			average = total / count;
			// Deviation
			for (int j = 0; j < width; j++)
			{
				Scalar intensity = ImgIn.at<uchar>(i,j);
				value = intensity.val[0];
				if(value > 250)
					deviation += pow(j - average,2);
			}
			
			// Standard deviation
			SD += pow(deviation / (width * height), 1/2);
		}
	}
	return SD;
}

// Standard deviation method
double Rotation3(Mat ImgIn)
{
	cout<<"Rotation3..."<<endl;
	double result = 0;
	double value = Standard_deviation(ImgIn);
	// Rotation from -5 degrees to 5 degrees with step = 0.2 degrees
	for (double i = -5; i <= 5; i+=0.2)
	{
		Mat ImgOut = skew_d(ImgIn, i);
		if(value > Standard_deviation(ImgOut))
		{
			result = i;
			value = Standard_deviation(ImgOut);
		}
	}
	cout<<" Degree = "<< result<<endl;
	return result;
}

Mat adaptive_Threshold(Mat ImgIn, int Thresh_binary = 0, int size_filter = 15, int c = 7, int dil = 1)
{
	Mat ImgOut;
	
	// binary adaptive threshold
	if (Thresh_binary == 0)
		adaptiveThreshold(ImgIn, ImgOut, 255, ADAPTIVE_THRESH_GAUSSIAN_C , THRESH_BINARY, size_filter, c); 
	else 
		adaptiveThreshold(ImgIn, ImgOut, 255, ADAPTIVE_THRESH_GAUSSIAN_C , THRESH_BINARY_INV, size_filter, c); 
    

    // smoothing side 3x3
	GaussianBlur(ImgOut, ImgOut, Size(3, 3), 0);
	
	// binary threshold 130
    threshold(ImgOut, ImgOut, 130, 255, THRESH_BINARY);
	
	// dilate 
    dilate(ImgOut, ImgOut, Mat(), Point(-1, -1), dil, 1, 1);
	return ImgOut;
}


void Recto(Mat In, Mat recto)
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

void Separate_the_dot(Mat In, Mat out)
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
    out = In;
}

Mat Separation_Of_Recto(Mat ImgIn)
{
	cout<<"Separation of recto and verso..."<<endl;
	int width = ImgIn.cols;
	int height = ImgIn.rows;   
    Mat rec(height, width, CV_8UC1, Scalar(0));

	// shining
    Mat shining = adaptive_Threshold(ImgIn, 0, 5, -8, 2);
	

	// dark
    Mat dark = adaptive_Threshold(ImgIn, 1, 5, 8, 2);
	

	// 3 Threshold
	dark = dark - 125;
    Mat Img3Thr = shining|dark;

	//Separate the dot 
    Separate_the_dot(Img3Thr, Img3Thr);
	
	//tìm nút recto theo mặt nạ 1x6 (255,255,255,125,125,125)  
    Recto(Img3Thr, rec);
	return rec;
}

void Get(Mat ImgIn, double IMAGE[560])
{
	int M = ImgIn.size().height;	
	int N = ImgIn.size().width;		
	int x, y, i = 0, t;
	for (x = 0; x < N; x++)
		for (y = 0; y < M; y++)
		{
			IMAGE[i] = ImgIn.at<uchar>(y, x);
			i++;
		}
}

double recognize_char(Mat ImgIn)
{
	cv::resize(ImgIn, ImgIn, cv::Size(20,28));
	double IMAGE[560];
	Get(ImgIn, IMAGE);
	double result = Recog_Braille(IMAGE);
	waitKey(0);	
}

String convert_to_char(double num)
{
	string result;
	int temp = num - 1;
	switch (temp)
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
			result = "No_Recognize";
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

int kt_cell(Mat ImgIn)
{
	int result = (ImgIn.rows-36)/2, value;
	for (int x = 0; x < ImgIn.rows-38; x++)
	{
		value = 0;
		for (int y = 0; y < ImgIn.cols; y++)
		{
			Scalar intensity1 = ImgIn.at<uchar>(x, y);
			value += intensity1.val[0];
		}
		if (value > 255)
		{
			result = x - 3;
			break;
		}
		for (int y = 0; y < ImgIn.cols; y++)
		{
			Scalar intensity1 = ImgIn.at<uchar>(ImgIn.rows-x, y);
			value += intensity1.val[0];
		}
		if (value > 255)
		{
			result = ImgIn.rows-(x+32);
			break;
		}
	}
	return result;
}

void data_augment(Mat In, Mat Out, int randx, int randy, int angle)
{
    Point2f center(In.cols / 2, In.rows / 2); // find center
    Mat matRot = getRotationMatrix2D(center, angle, 1);
    warpAffine(In, In, matRot, In.size());
    int width = In.cols;
    int height = In.rows;
    for (int x = 0; x < width; x++)
    	for (int y = 0; y < height; y++)
	{
		Scalar pixel = In.at<uchar>(y, x);
		if (x+randx >= 0 && x+randx < width && y+randy >= 0 && y+randy < height)
		Out.at<uchar>(Point(x+randx, y+randy)) = pixel.val[0]; 
	}
}

void make_sound()
{
	system("espeak -v vi-vn-x-south -f /home/ngo/hk2_4/DetectAndRegconize/build/text.txt");		
}

vector<Mat> Line(Mat ImgIn)
{
	vector<Mat> line;
	int value = 0;
	int width = ImgIn.cols; 
	int height = ImgIn.rows;
	for (int x = 0; x < height ; x++)
	{
		for (int y = 0; y < width; y++)
		{
			Scalar intensity = ImgIn.at<uchar>(x,y);
			value += intensity.val[0];
		}
		if (value > 8*255 && (x+40) < height && (x - 5) > 0)
		{
			line.push_back(ImgIn(cv::Rect(0, x-5, width, 45)));
			x+=45;
		}
		value = 0; 
	}
	return line;
}


int num_cell(Mat ImgIn, int start)
{
	int count = 0;
	cv::CascadeClassifier cascade;
	if (!cascade.load("classifier/cascade.xml")) 
	{
		std::cout << "Error when loading the cascade classfier!" << std::endl;	
		return -1;
	}
	std::vector< Rect >  detections;
	for (int x = start; x < ImgIn.cols - 25; x+=30)
	{
		Mat cell = ImgIn(Rect(x, 0,25,ImgIn.rows));
		cascade.detectMultiScale(cell, detections, 1.05, 3, 0, cv::Size(24, 34), cv::Size(25, 38));
		if (detections.size() >= 1)
			count++;
	}
	return count;
}

vector<Mat> Cell(Mat ImgIn)
{
	vector<Mat> cell;
	int num = 0, temp=0, point_start=0;
	
	// fine point start 
	for (int i =0; i<30;i++)
	{
		num = num_cell(ImgIn,i);
		if (num > temp)
		{
			temp = num;
			point_start = i;
		}
	}
	// cut cell
	for (int x = point_start; x < ImgIn.cols - 25; x=x+30)
	{
		Mat cell_raw = ImgIn(Rect(x, 0, 25, ImgIn.rows));
		Rect windows(x, kt_cell(cell_raw), 25, 35);
		cell.push_back(ImgIn( windows));
		rectangle(ImgIn, windows, Scalar(255), 1, 8, 0);
	}
	return cell;
}

String Detection_recognize(Mat ImgIn)
{
	cout<<"Detection and recognize..."<<endl;
	String results, temp;
	vector<Mat> line;
	vector<Mat> cell;
	line = Line(ImgIn);
	for(int i = 0; i < line.size(); i++)
	{
		cell = Cell(line[i]);
		for(int j = 0; j < cell.size(); j++)
		{
			results += convert_to_char(recognize_char(cell[j]));
		}
		results += "\n";
	}
	return results;
}






