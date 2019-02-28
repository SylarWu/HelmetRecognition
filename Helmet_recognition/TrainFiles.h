#pragma once
#include<fstream>
#include<iostream>
#include<vector>

class TrainFiles 
{
public :
	TrainFiles();
	virtual ~TrainFiles();

public:
	bool Load();

	std::vector<std::string> getPosNames();
	std::vector<std::string> getNegNames();
	std::vector<std::string> getBackNames();
	std::vector<std::string> getHelmetNames();

	std::string getTrainRoot();

private:
	void loadPosNames(std::ifstream & fread);
	void loadNegNames(std::ifstream & fread);
	void loadBackNames(std::ifstream & fread);
	void loadHelmetNames(std::ifstream & fread);
private :
	//训练图片的根目录
	std::string TRAIN_ROOT;
	//保存所有正样本的文件名-> 戴安全帽工人
	std::vector<std::string> pos_names;
	//保存所有负样本的文件名-> 行人
	std::vector<std::string> neg_names;
	//保存所有背景的文件名-> 背景
	std::vector<std::string> back_names;
	//保存所有安全帽样本的文件名-> 安全帽
	std::vector<std::string> helmet_names;
};