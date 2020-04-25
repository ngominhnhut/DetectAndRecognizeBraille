//C++
//Opencv4.12
//espeak



Final project (hk2 năm 4)
Thiết Bị Hỗ Trợ Người Khiếm Thị

TIẾN ĐỘ
- Preporcessing
	+ Take a picture.
	+ Tăng độ tương phản.(Done)
	+ Adaptive gaussian thresholding.(Done)
	+ Xoay.(Done)
	+ tách hàng.
- Detect.(done)
	+ Sliding windown theo khoản cách phù hợp.(Done)
	+ Haar cascade detect.(Done)
- Recognize(khá ổn nhung van sai)
- chuyển thành âm thanh(done)
	+ text.(thiếu dấu ',`,?,~,., quy tắc viết)
	+ espeak.(Done)


	Preprocessing: Mat Preprocessed = Preprocessing(Mat ImgIn,int mode,String Select_ROI);
+ Mode: default = 0 ( adjust method. low = 2%, up = 0% ), mode = 1 (g(x)= alpha*f(x) - beta).
+ Select_ROI: default = "Full", Select_ROI = "Select" ( Select ROI).
	
	Separation of recto and verso: Mat Recto = Separation_Of_Recto(Mat1b ImgIn);


	căng dòng: Mat ImgOut = skew_d(Mat ImgIn, Rotation(Mat ImgIn));
+ Rotation: trái phải 4 độ (paper India).
+ Rotation2: đếm line (default).
+ Rotation3: Standard deviation (độ lệch chuẩn)(paper Iran).


