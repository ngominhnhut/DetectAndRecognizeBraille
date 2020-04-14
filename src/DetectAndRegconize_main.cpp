#include <iostream>
#include <stdio.h>
#include <dirent.h>
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

#include "rt_nonfinite.h"
#include "regconize.h"
#include "DetectAndRegconize.h"
#include "nhut.hpp"
#include "regconize_terminate.h"
#include "regconize_initialize.h"

using namespace std;
using namespace cv;

int main(int argc, const char* const*)
{
    vector<cv::String> fn;
    glob("*.jpg", fn, false);
    vector<Mat> img;
    size_t count = fn.size(); //number of png files in images folder
    for (size_t i=0; i<count; i++)
    {
	img.push_back(imread(fn[i],IMREAD_GRAYSCALE));
	Mat imgs = img[i];
	
	//Rect r = selectROI(img2);
    	//Mat imgs = img2(r);
    	
	imgs = (imgs*2)-170; 
	
	int width = imgs.cols;
    	int height = imgs.rows;   
    	Mat recto(height, width, CV_8UC1, Scalar(0));
   	
	xu_ly_2mat(imgs,recto);
	//Mat xoay =xoay_goc(recto);
	Mat xoay = recto;
	crop_hang_DR(xoay);
	imwrite("a/result.jpg",xoay); 
    }
return 0;
}




