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
#include "Recog_Braille.h"
#include "Recog_Braille_terminate.h"
#include "Recog_Braille_initialize.h"


using namespace std;
using namespace cv;


int main(int argc, const char* const*)
{
    vector<cv::String> fn;
    glob("input.jpg", fn, false);
    vector<Mat> img;
	//number of png files in images folder
    size_t count = fn.size(); 
    for (size_t i=0; i<count; i++)
    {
		img.push_back(imread(fn[i],IMREAD_GRAYSCALE));
		Mat ImgIn = img[i];
		
		// Preprocessing 
		Mat1b Preprocessed = Preprocessing(ImgIn, 1, "Full");
		
		// Separation of recto and verso
		Mat ImgRecto = Separation_Of_Recto(Preprocessed);

		// skew
		Mat ImgSkew = skew_d(ImgRecto, Rotation3(ImgRecto));

		// Detection and Recognize Braille
		String txt = Detection_recognize(ImgSkew);
		cout<<txt<<endl;
		
		// Create file text
		write_txt(txt);

		// make sound
		make_sound();

		cout<<"Successful..."<<endl;
		imshow("result", ImgSkew);
		imshow("Images", ImgIn);
		waitKey(0); 
    }
return 0;
}




