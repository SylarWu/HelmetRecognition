#include<iostream>
#include<string>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include"TrainFiles.h"
#include"PreProcess.h"
#include"SVMTraining.h"
int main(){

	SVMTraining svmtraining = SVMTraining();
	svmtraining.train();

	cv::VideoCapture capture;
	capture.open("π§µÿ ”∆µ2.mp4");
	cv::Mat src;
	while (1) {
		capture >> src;
		if (src.empty()) {
			break;
		}

		svmtraining.detect(src);
	
		cv::imshow("°æºÏ≤‚≤‚ ‘°ø", src);

		cv::waitKey(1);
	}

	cv::waitKey(1);

	system("pause");
	return 0;
}