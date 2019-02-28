#include"stdafx.h"
#ifndef CLASS_DETECTOR_H
#define CLASS_DETECTOR_H
#include"SVMTraining.h"

class Detector {
private:
	//svm模型
	SVMTraining svm_train_result;
	//hog检测器
	cv::HOGDescriptor hogdescriptor;
	//使用背景差分法
	//每一秒更新
	cv::Mat background;
	
	PreProcess preprocess;

	cv::Mat helmet_red;
	cv::Mat helmet_blue;
	cv::Mat helmet_yellow;

public:
	//构造检测器
	Detector();
	~Detector() {

	}
	void Load();
	//使用hog检测器进行普通扫描，标记
	void HOGScan(cv::Mat & frame);

	void SetBackGround(cv::Mat background) {
		this->background = background;
	}

	//背景差分法，检测动态物体
	std::vector<cv::Rect> MoveDetect(cv::Mat frame);
	int Otsu(cv::Mat & src);
	double ColorSim(cv::Mat pic1, cv::Mat pic2);
};



#endif