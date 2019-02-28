#include "stdafx.h"
#include"SVMTraining.h"
#include<time.h>

bool Mysvm::s_bTrain = false;

int Mysvm::get_alpha_count()
{
	return this->sv_total;
}

int Mysvm::get_sv_dim()
{
	return this->var_all;
}

int Mysvm::get_sv_count()
{
	return this->decision_func->sv_count;
}

double* Mysvm::get_alpha()
{
	return this->decision_func->alpha;
}

float** Mysvm::get_sv()
{
	return this->sv;
}

float Mysvm::get_rho()
{
	return this->decision_func->rho;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

SVMTraining::SVMTraining() {
	//默认训练模型xml文件已经加载好，无需训练
	loadPreProcess();
	loadHOGer();
}

SVMTraining::~SVMTraining()
{
	
}
//加载训练样本索引文件，为特征举证开阔空间
void SVMTraining::loadTrainFilesAndExploreSpace() {
	//判断是否需要进行训练
	if (Mysvm::s_bTrain) {
		this->trainfiles.Load();
		//加载样本根目录
		this->Train_File_Root = this->trainfiles.getTrainRoot();
		//加载样本名
		this->PosNames = this->trainfiles.getPosNames();
		this->NegNames = this->trainfiles.getNegNames();
		this->BackNames = this->trainfiles.getBackNames();
		this->HelmetNames = this->trainfiles.getHelmetNames();
		//加载样本数量
		this->Pos_Nums = this->PosNames.size();
		this->Neg_Nums = this->NegNames.size();
		this->Back_Nums = this->BackNames.size();
		this->Helmet_Nums = this->HelmetNames.size();

		//特征：总体
		this->fetureOfSample_General = cv::Mat::zeros(this->Pos_Nums +
			this->Neg_Nums +
			this->Back_Nums +
			this->Helmet_Nums,
			3780,
			CV_32FC1);
		//标签：总体
		this->labelOfSample_General = cv::Mat::zeros(this->Pos_Nums +
			this->Neg_Nums +
			this->Back_Nums +
			this->Helmet_Nums,
			1,
			CV_32FC1);
	}
}
//加载预处理器
void SVMTraining::loadPreProcess() {
	this->preprocess = PreProcess();
}
//加载HOG检测子
void SVMTraining::loadHOGer() {
	this->hogdescriptor = cv::HOGDescriptor(cv::Size(64, 128), cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9);
}


//对训练样本打上标签
void SVMTraining::withdrawFeatureAndLabel(int nums, std::vector<std::string> & filenames, int start, float label) {
	//源图片src和经过预处理后图片temp
	cv::Mat src, temp;
	//临时该图片特征值
	std::vector<float> descriptors;
	//计算处理时间
	double begin_time;
	double end_time;
	double elapse_ms;
	//开始加载样本提取特征
	for (int i = 0; i < nums; i++) {
		//加载图片
		src = cv::imread(this->Train_File_Root + filenames[i]);
		if (src.empty()) {
			std::cout << "文件读取错误：" << this->Train_File_Root + filenames[i] << std::endl;
			continue;
		}
		//计算起始时间
		begin_time = (double)cv::getTickCount();
		//归一化到64，128
		cv::resize(src, src, cv::Size(64, 128));
		//图片预处理
		temp = preprocess.GrayAndGammaCoreect(src);
		//特征值计算
		this->hogdescriptor.compute(temp, descriptors);
		//计算结束时间
		end_time = (double)cv::getTickCount();
		//显示时间
		elapse_ms = (end_time - begin_time) * 1000 / cv::getTickFrequency();
		//保存到特征样本中
		for (int j = 0; j < descriptors.size(); j++) {
			this->fetureOfSample_General.at<float>(start + i, j) = descriptors[j];
		}
		//加上标签
		this->labelOfSample_General.at<float>(start + i, 0) = label;
	}
}
//加载训练样本
void SVMTraining::LoadTrainSamples()
{
	if (Mysvm::s_bTrain) {
		//背景图片打上-1标签
		this->withdrawFeatureAndLabel(this->Back_Nums, this->BackNames, 0, -1.0);
		//戴帽人打上1标签
		this->withdrawFeatureAndLabel(this->Pos_Nums, this->PosNames, this->Back_Nums, 1.0);
		//未戴帽人打上2标签
		this->withdrawFeatureAndLabel(this->Neg_Nums, this->NegNames, this->Back_Nums + this->Pos_Nums, 2.0);
		//帽子打上3标签
		this->withdrawFeatureAndLabel(this->Helmet_Nums, this->HelmetNames, this->Back_Nums + this->Pos_Nums + this->Neg_Nums, 3.0);

		//SVM训练器设置参数
		cv::SVMParams params = this->SVMTrainorInitial();

		//开始训练
		this->svm_general.train(this->fetureOfSample_General, this->labelOfSample_General, cv::Mat(), cv::Mat(), params);
		//保存训练模型
		this->svm_general.save(".\\xml\\Classifier_General.xml");
	}
	else {
		//加载训练模型
		this->svm_general.load(".\\xml\\Classifier_General.xml");
	}
}
//初始化SVM参数
cv::SVMParams & SVMTraining::SVMTrainorInitial()
{
	cv::SVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 2000, FLT_EPSILON);
	params.C = 0.01;
	return params;
}

bool SVMTraining::train() {

	LoadTrainSamples();

	Mysvm svm_temp;
	svm_temp.load(".\\xml\\Classifier_Back_Human.xml");

	int support_vector_num = svm_temp.get_support_vector_count();
	//特征向量维数
	int var_count = svm_temp.get_var_count();
	//支持向量矩阵
	cv::Mat support_vector = cv::Mat::zeros(support_vector_num, var_count, CV_32FC1);
	//alpha
	cv::Mat alpha = cv::Mat::zeros(1, support_vector_num, CV_32FC1);
	//result
	cv::Mat result = cv::Mat::zeros(1, var_count, CV_32FC1);

	//填充support_vector
	for (int i = 0; i < support_vector_num; i++) {
		const float * pSVDATA = svm_temp.get_support_vector(i);
		for (int j = 0; j < var_count; j++) {
			support_vector.at<float>(i, j) = pSVDATA[j];
		}
	}
	//填充alpha
	double * pALPHADATA = svm_temp.get_alpha();
	for (int i = 0; i < support_vector_num; i++) {
		alpha.at<float>(0, i) = (float)pALPHADATA[i];
	}
	//结果
	result = -1 * alpha * support_vector;
	//result = alpha * support_vector;
	//检测子形成
	std::vector<float> mydetector;
	for (int i = 0; i < var_count; i++) {
		mydetector.push_back(result.at<float>(0, i));
	}
	//最后添加偏移量
	mydetector.push_back(svm_temp.get_rho());

	this->hogdescriptor.setSVMDetector(mydetector);

	return true;
}

void SVMTraining::detect(cv::Mat & frame) {
	std::vector<cv::Rect> found;
	cv::Mat temp = preprocess.GrayAndGammaCoreect(frame);
	hogdescriptor.detectMultiScale(temp, found, 0, cv::Size(16, 16));//对图片进行多尺度行人检测
	cv::Mat middle;
	int judge;

	for (int i = 0; i < found.size(); i++) {
		cv::Rect r = found[i];
		judge = -1;
		middle = sampleToConfig(r, temp);
		judge = (int)svm_general.predict(middle);
		cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 0, 255), 1);
		std::cout << judge << std::endl;
		switch (judge)
		{
		case 1:
			cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(255, 255, 255), 1);
			break;
		case 2:
			cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 0, 255), 1);
			break;
		case 3:
			cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 1);
			break;
		default:
			cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 0, 255), 1);
			break;
		}
	}
}

cv::Mat SVMTraining::sampleToConfig(cv::Rect r, cv::Mat src)
{
	cv::Mat ROI = src(r);
	cv::Mat temp;
	cv::resize(ROI, temp, cv::Size(64, 128));
	std::vector<float> descriptors;
	hogdescriptor.compute(temp, descriptors);
	cv::Mat result(1, descriptors.size(), CV_32FC1);
	for (int i = 0; i < descriptors.size(); i++) {
		result.at<float>(0, i) = descriptors[i];
	}
	return result;
}


