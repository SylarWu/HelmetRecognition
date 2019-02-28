#pragma once

#include <math.h>

class PreProcess 
{
public:
	PreProcess();

public:
	bool Load();
	//灰度化同时进行Gamma校准
	cv::Mat GrayAndGammaCoreect(cv::Mat & src);

private :
	double Gamma;
	uchar Gamma_Chart[256];
};
