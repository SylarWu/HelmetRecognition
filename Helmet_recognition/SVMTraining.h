#pragma once

#include "TrainFiles.h"
#include "PreProcess.h"

class Mysvm : public CvSVM
{
public:
	static bool s_bTrain;

public:
	int get_alpha_count();
	int get_sv_dim();
	int get_sv_count();
	double* get_alpha();
	float** get_sv();
	float get_rho();
};

class SVMTraining 
{
public:
	//svm训练器构造函数，加载样本等
	SVMTraining();

	virtual ~SVMTraining();

public:
	//svm开始训练
	bool train();

	//通过svm进行大概检测
	void detect(cv::Mat & frame);

	//将检测出的特征框转换为特征使用训练好的svm模型进行预测
	cv::Mat sampleToConfig(cv::Rect r, cv::Mat src);

	//加载训练样本索引文件，为特征举证开阔空间
	void loadTrainFilesAndExploreSpace();

	//加载预处理器
	void loadPreProcess();

	//加载HOG检测子
	void loadHOGer();

	//训练时抽出样本特征值，加入待训练样本集中，并打上标签
	void withdrawFeatureAndLabel(int nums, std::vector<std::string> & filenames, int start, float label);

	//装载训训练样本
	void LoadTrainSamples();

	//svm训练器参数初始化
	cv::SVMParams & SVMTrainorInitial();

public:

	//获取svm训练器
	Mysvm & getMysvm() {
		return this->svm_general;
	}

	//获取特征检测子
	cv::HOGDescriptor & getHOGDescriptor() {
		return this->hogdescriptor;
	}

private:
	//训练文件
	TrainFiles trainfiles;

	//正样本数
	int Pos_Nums;
	//负样本数
	int Neg_Nums;
	//背景样本数
	int Back_Nums;
	//帽子样本
	int Helmet_Nums;

	//正样本文件名
	std::vector<std::string> PosNames;
	//负样本文件名
	std::vector<std::string> NegNames;
	//背景样本文件名
	std::vector<std::string> BackNames;
	//帽子样本文件名
	std::vector<std::string> HelmetNames;

	//样本根目录
	std::string Train_File_Root;

	//HOG特征检测器
	cv::HOGDescriptor hogdescriptor;

	//预处理器
	PreProcess preprocess;

	//SVM训练器：总体->背景（-1），戴帽人（1），未戴帽人（2），帽子（3）
	Mysvm svm_general;
	//特征样本：总体
	cv::Mat fetureOfSample_General;
	//标签：总体
	cv::Mat labelOfSample_General;

};
