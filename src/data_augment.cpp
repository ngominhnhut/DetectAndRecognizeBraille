
#include <iostream>
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/tracking.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>

#include <dirent.h> 
#include "Braille.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* const*)
{
/* initialize random seed: */
srand (time(NULL));
for (int f=0 ; f< 54; f++)
{ 
    std::string f2 = std::to_string(f);
    vector<cv::String> fn;
    if (f<10)
       glob("Images/0"+f2+"/*.jpg", fn, false);
    else
       glob("Images/"+f2+"/*.jpg", fn, false);
    vector<Mat> img;
    size_t count = fn.size(); 
    cout << count<<endl;
    for (size_t i=0; i<count; i++)
    {
	
	std::string s = std::to_string(i);
	img.push_back(imread(fn[i],IMREAD_GRAYSCALE));
	//Mat img2 = img[i];
	//Rect r = selectROI(img2);
    	// Crop image
    	//Mat imgs = img2(r);
    	//Mat imgs = (img[i]*2)-170; // tăng độ tương phản
	Mat imgs = img[i];
	int width = imgs.cols;
    	int height = imgs.rows;   
    	Mat recto(height, width, CV_8UC1, Scalar(0));
	for(int j=0; j<5; j++)
	{	
    		/* generate secret number between -2 and 2: */
    		int randx = rand() % 5 -2;
    		int randy = rand() % 5 -2;
		int angle = rand() % 3 -1;
    		cout<<"r_x = "<<randx<<"; r_y = "<<randy<<"; angle = "<< angle<<endl;
		std::string s2 = std::to_string(j);		
		data_augment(imgs, recto, randx, randy, angle);
		cv::resize(recto, recto, cv::Size(20,28));
		if (f<10)
		imwrite("results/Images/0"+f2+"/augment"+s2+"_"+s+".jpg",recto);
		else
		imwrite("results/Images/"+f2+"/augment"+s2+"_"+s+".jpg",recto);
	}

	//cv::resize(imgs, imgs, cv::Size()0.8,0.8);
	
   	//xu_ly_2mat_data(imgs,recto);
	//dilate(recto, recto, Mat(), Point(-1, -1), 1, 1, 1);
	//cv::resize(recto, recto, cv::Size(20,28));
	//recto = xoay_test(recto);
	//Mat xoay =xoay_goc(recto);
	//Mat xoay =recto;

/*
	for (int j =0; j<30;j++)
	{
		m = kt_so_cell(


xoay,j);
		cout << "so cell =" << m<< endl;
		if (m>t)
		{
			t=m;
			t2=j;
		}
	}
	//Mat crop = xoay(Rect(0, 200, xoay.size().width, 100));
	for (int x = t2; x < xoay.cols - 25; x=x+30)
	{
		Rect windows(x, 0, 25, xoay.rows);
		rectangle(xoay, windows, Scalar(255), 1, 8, 0);
	}
*/
   	//imwrite("a/"+fn[i],recto);}
	//imshow("thres", xoay); 
	//waitKey(0);
	}
}
return 0;
}
















