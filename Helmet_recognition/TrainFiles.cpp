#include "stdafx.h"
#include "TrainFiles.h"

TrainFiles::TrainFiles() 
{
	//
}

TrainFiles::~TrainFiles()
{

}

// 这段代码不要放在构造函数中,需要时再调用，另外这个路径需要做成动态的，或者从配置文件中读取。
bool TrainFiles::Load()
{
	//加载根目录
	this->TRAIN_ROOT = "D:\\WalkerDetect\\";
	//读取正样本索引文件
	std::ifstream pos_index_load(this->TRAIN_ROOT + "pos.txt");
	//读取负样本索引文件
	std::ifstream neg_index_load(this->TRAIN_ROOT + "neg.txt");
	//读取背景样本索引文件
	std::ifstream back_index_load(this->TRAIN_ROOT + "back.txt");
	//读取帽子样本索引文件
	std::ifstream helmet_index_load(this->TRAIN_ROOT + "helmet.txt");
	//加载正负样本所有文件名字
	loadPosNames(pos_index_load);
	loadNegNames(neg_index_load);
	loadBackNames(back_index_load);
	loadHelmetNames(helmet_index_load);

	return true;
}


std::vector<std::string> TrainFiles::getPosNames() {
	return this->pos_names;
}
std::vector<std::string> TrainFiles::getNegNames() {
	return this->neg_names;
}
std::vector<std::string> TrainFiles::getBackNames() {
	return this->back_names;
}
std::vector<std::string> TrainFiles::getHelmetNames() {
	return this->helmet_names;
}
std::string TrainFiles::getTrainRoot() {
	return this->TRAIN_ROOT;
}

void TrainFiles::loadPosNames(std::ifstream & fread) {
	//临时文件名
	std::string buffer;
	while (!fread.eof()) {
		std::getline(fread, buffer);
		if (!buffer.empty()) {
			pos_names.push_back(buffer);
		}
	}
}
void TrainFiles::loadNegNames(std::ifstream & fread) {
	//临时文件名
	std::string buffer;
	while (!fread.eof()) {
		std::getline(fread, buffer);
		if (!buffer.empty()) {
			neg_names.push_back(buffer);
		}
	}
}
void TrainFiles::loadBackNames(std::ifstream & fread) {
	//临时文件名
	std::string buffer;
	while (!fread.eof()) {
		std::getline(fread, buffer);
		if (!buffer.empty()) {
			back_names.push_back(buffer);
		}
	}
}
void TrainFiles::loadHelmetNames(std::ifstream & fread) {
	//临时文件名
	std::string buffer;
	while (!fread.eof()) {
		std::getline(fread, buffer);
		if (!buffer.empty()) {
			helmet_names.push_back(buffer);
		}
	}
}
