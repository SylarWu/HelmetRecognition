#include"stdafx.h"
#ifndef CLASS_DETECTOR_CPP
#define CLASS_DETECTOR_CPP
#include"Detector.h"
#include<nonfree\features2d.hpp>


Detector::Detector() {
	
}

void Detector::Load() {
	//加载样本安全帽图片
	this->helmet_yellow = cv::imread(".\\xml\\helmet_yellow");
	this->helmet_red = cv::imread(".\\xml\\helmet_red");
	this->helmet_blue = cv::imread(".\\xml\\helmet_blue");

	//加载预处理器
	this->preprocess.Load();
	//训练或加载训练模型
	this->svm_train_result.train();
	//加载hog检测器
	this->hogdescriptor = this->svm_train_result.getHOGDescriptor();
}
void Detector::HOGScan(cv::Mat & frame){
	//初始化找出结果矩形框
	std::vector<cv::Rect> found;
	//预处理视频帧
	cv::Mat temp = preprocess.GrayAndGammaCoreect(frame);
	//使用hog进行多维度检测
	hogdescriptor.detectMultiScale(temp, found, 0, cv::Size(16, 16));//对图片进行多尺度行人检测
	//识别中间变量
	cv::Mat middle;
	//判断检测值
	int judge;
	//帽子的兴趣区域Mat矩阵
	cv::Mat HelmetROI;
	//帽子的兴趣区域Rect矩阵
	cv::Rect HelmetRect;

	//处理找出结果矩形框
	for (int i = 0; i < found.size(); i++) {
		//初始变量
		cv::Rect r = found[i];
		//初始为-1
		judge = -1;
		//将找出矩阵框转换为svm识别参数
		middle = this->svm_train_result.sampleToConfig(r, temp);
		//得到判断结果
		judge = (int)this->svm_train_result.getMysvm().predict(middle);
		//将找到的行人的头部截取
		HelmetRect.width = r.width;
		HelmetRect.height = r.height*0.3;
		HelmetRect.x = r.x;
		HelmetRect.y = r.y;

		HelmetROI = frame(HelmetRect);

		//进行判断识别
		//白色为戴帽的行人，判断为1
		//在白色框中再进行判断，如果戴帽标出绿色框
		//判断为2，没戴帽的行人
		//判断为3，安全帽
		switch (judge)
		{
		case 1:
			cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(255, 255, 255), 1);

			if (ColorSim(HelmetROI, helmet_yellow) >= 0.6 || ColorSim(HelmetROI, helmet_blue) >= 0.6 || ColorSim(HelmetROI, helmet_red) >= 0.6) {
				cv::rectangle(frame, HelmetRect.tl(), HelmetRect.br(), cv::Scalar(0, 255, 0), 1);
			}

			break;
		case 2:
			cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 0, 255), 1);
			break;
		case 3:
			cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 1);
			break;
		}
		
	}
}



std::vector<cv::Rect> Detector::MoveDetect(cv::Mat frame)
{
	//保存结果，不能对原图片进行操作
	cv::Mat result = frame.clone();
	//背景灰度图
	cv::Mat gray_background;
	//帧灰度图
	cv::Mat gray_frame;
	//灰度转换
	cv::cvtColor(this->background, gray_background, CV_BGR2GRAY);
	cv::cvtColor(frame, gray_frame, CV_BGR2GRAY);
	//二者差别
	cv::Mat diff;
	cv::absdiff(gray_background, gray_frame, diff);
	//3.对差值图diff_thresh进行阈值化处理  
	cv::Mat diff_thresh;
	threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
	//4.腐蚀  
	cv::Mat kernel_erode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat kernel_dilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	erode(diff_thresh, diff_thresh, kernel_erode);
	//5.膨胀  
	cv::dilate(diff_thresh, diff_thresh, kernel_dilate);
	//6.查找轮廓并绘制轮廓  
	std::vector<std::vector<cv::Point>> contours;
	findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//7.查找正外接矩形
	std::vector<cv::Rect> boundRect;
	cv::Rect bufferRect;
	for (int i = 0; i < contours.size(); i++)
	{
		bufferRect = boundingRect(contours[i]);
		if (bufferRect.height / bufferRect.width > 1.5) {
			boundRect.push_back(bufferRect);
		}
	}
	return boundRect;
}

int Detector::Otsu(cv::Mat & src)
{  
	int height=src.rows;  
	int width=src.cols;      
	long size = height * width; 
 
	//histogram  
	float histogram[256] = {0};  
	for(int m=0; m < height; m++)
	{  
		unsigned char* p=(unsigned char*)src.data + src.step * m;  
		for(int n = 0; n < width; n++) 
		{  
			histogram[int(*p++)]++;  
		}  
	}  
 
	int threshold;    
	long sum0 = 0, sum1 = 0; //存储前景的灰度总和和背景灰度总和
	long cnt0 = 0, cnt1 = 0; //前景的总个数和背景的总个数
	double w0 = 0, w1 = 0; //前景和背景所占整幅图像的比例
	double u0 = 0, u1 = 0;  //前景和背景的平均灰度
	double variance = 0; //最大类间方差
	int i, j;
	double u = 0;
	double maxVariance = 0;
	for(i = 1; i < 256; i++) //一次遍历每个像素
	{  
		sum0 = 0;
		sum1 = 0; 
		cnt0 = 0;
		cnt1 = 0;
		w0 = 0;
	    w1 = 0;
		for(j = 0; j < i; j++)
		{
			cnt0 += histogram[j];
			sum0 += j * histogram[j];
		}
 
		u0 = (double)sum0 /  cnt0; 
		w0 = (double)cnt0 / size;
 
		for(j = i ; j <= 255; j++)
		{
			cnt1 += histogram[j];
			sum1 += j * histogram[j];
		}
 
		u1 = (double)sum1 / cnt1;
		w1 = 1 - w0; // (double)cnt1 / size;
 
		u = u0 * w0 + u1 * w1; //图像的平均灰度
		printf("u = %f\n", u);
		//variance =  w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		variance =  w0 * w1 *  (u0 - u1) * (u0 - u1);
		if(variance > maxVariance) 
		{  
			maxVariance = variance;  
			threshold = i;  
		} 
	}  
 
	printf("threshold = %d\n", threshold);
	return threshold;  
} 

double Detector::ColorSim(cv::Mat pic1, cv::Mat pic2) {
	cv::Mat temp1;
	cv::Mat temp2;

	cv::cvtColor(pic1, temp1, cv::COLOR_BGR2HSV);
	cv::cvtColor(pic2, temp2, cv::COLOR_BGR2HSV);

	
	int h_bins = 50, s_bins = 60;

	int histSize[] = { h_bins, s_bins };

	float h_ranges[] = { 0, 180 };

	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	int channels[] = { 0, 1 };

	cv::MatND hist1, hist2;

	cv::calcHist(&temp2, 1, channels, cv::Mat(), hist1, 2, histSize, ranges, true, false);
	cv::normalize(hist1, hist1, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
	cv::calcHist(&temp1, 1, channels, cv::Mat(), hist2, 2, histSize, ranges, true, false);
	cv::normalize(hist2, hist2, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
	double dSimilarity = cv::compareHist(hist1, hist2, CV_COMP_CORREL); //,CV_COMP_CHISQR,CV_COMP_INTERSECT,CV_COMP_BHATTACHARYYA  CV_COMP_CORREL

	return dSimilarity;
}
#endif

