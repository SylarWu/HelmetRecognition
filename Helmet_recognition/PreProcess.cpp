#include "stdafx.h"
#include "PreProcess.h"

PreProcess::PreProcess() 
{
	
}

bool PreProcess::Load()
{
	Gamma = 0.5;
	//初始化Gamma校准表
	for (int i = 0; i < 256; i++) 
	{
		this->Gamma_Chart[i] = cv::saturate_cast<uchar>( pow((double)(i / 255.0), Gamma) * 255 );
	}
	return true;
}

//灰度化同时进行Gamma校准
cv::Mat PreProcess::GrayAndGammaCoreect(cv::Mat & src)
{
	//获取通道数
	int channels = src.channels();
	//获取行数、列数、步长
	int rows = src.rows;
	int cols = src.cols;

	//结果图像，不对原图像进行操作
	cv::Mat result(rows,cols,CV_8UC1);

	if (channels == 1) {
		for (int j = 0; j < rows; j++) {
			//获取源图和目标图行指针
			uchar * result_data = result.ptr<uchar>(j);
			uchar * src_data = src.ptr<uchar>(j);
			for (int i = 0; i < cols; i++) {
				//查表法进行像素转换
				result_data[i] = this->Gamma_Chart[src_data[i]];
			}
		}
	}
	else if (channels == 3) {
		for (int j = 0; j < rows; j++) {
			//获取源图和目标图行指针
			uchar * result_data = result.ptr<uchar>(j);
			uchar * src_data = src.ptr<uchar>(j);
			for (int i = 0; i < cols; i++) {
				//查表法进行像素转换
				result_data[i] = (uchar)(src_data[i * channels + 0]*0.11 + src_data[i * channels + 1]*0.59 + src_data[i * channels + 2]*0.30);
				result_data[i] = this->Gamma_Chart[result_data[i]];
			}
		}
	}
	else {
		//四通道首先进行灰度转换
		cv::cvtColor(src, result, CV_RGBA2GRAY);
		for (int j = 0; j < rows; j++) {
			//获取行指针
			uchar * result_data = result.ptr<uchar>(j);
			for (int i = 0; i < cols; i++) {
				//查表法进行像素转换
				result_data[i] = this->Gamma_Chart[result_data[i]];
			}
		}
	}
	return result;
}

