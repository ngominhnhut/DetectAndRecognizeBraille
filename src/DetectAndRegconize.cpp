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

#include "Braille.hpp"
#include "DetectAndRegconize.h"
#include "rt_nonfinite.h"
#include "tiengviet.h"
#include "tiengviet_terminate.h"
#include "tiengviet_initialize.h"


using namespace std;
using namespace cv;

int main(int argc, const char* const*)
{
    vector<cv::String> fn;
    glob("input1.jpg", fn, false);
    vector<Mat> img;
    size_t count = fn.size(); //number of png files in images folder
    for (size_t i=0; i<count; i++)
    {
	img.push_back(imread(fn[i],IMREAD_GRAYSCALE));
	Mat imgs = img[i];
	int name=0;
	//Rect r = selectROI(img2);
    	//Mat imgs = img2(r);
    	
	imgs = (imgs*2)-170; 
	int width = imgs.cols;
    	int height = imgs.rows;   
    	Mat recto(height, width, CV_8UC1, Scalar(0));
	xu_ly_2mat(imgs,recto);
	cout<<"successful preprocessing"<<endl;

	//Mat xoay = xoay_goc(recto);
	Mat xoay = recto;
	crop_hang_DR(xoay);
	am_thanh();
	std::string s = std::to_string(i);
	imwrite("a/result"+s+".jpg",xoay);
	cout<<"Finish"<<endl; 
    }
return 0;
}




